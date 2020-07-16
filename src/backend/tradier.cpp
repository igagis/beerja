#include "tradier.hpp"

#include <utki/string.hpp>

#include <easyhttp/request.hpp>
#include <easyhttp/util.hpp>

#include <jsondom/dom.hpp>

#include "../date/date.h"

const std::string tradier::tag = "tradier";

namespace{
const std::string end_point = "https://sandbox.tradier.com/v1/";
}

namespace{
struct tradier_async_operation : public beerja::async_operation{
	std::weak_ptr<easyhttp::request> http_req;

	bool cancel()override{
		if(auto r = this->http_req.lock()){
			return r->cancel();
		}
		return false; // http request object has been destroyed => http request has finished already => not cancelled
	}
};
}

void tradier::set_config(const puu::forest& config){

	auto i = std::find(config.begin(), config.end(), "access_token");
	if(i != config.end()){
		if(!i->children.empty()){
			this->access_token = i->children.front().value.to_string();
		}
	}

	TRACE(<< "API Key = " << this->access_token << std::endl)
}

std::shared_ptr<beerja::async_operation> tradier::get_exchanges(
			std::function<void(
					beerja::status,
					std::shared_ptr<beerja::async_operation>,
					std::vector<beerja::exchange>&&
				)>&& callback
		)
{
	if(!callback){
		throw std::logic_error("tradier::get_exchanges(): passed in callback is nullptr");
	}

	std::vector<beerja::exchange> ret = {
		{"A", "NYSE MKT" },
		{"B", "NASDAQ OMX BX" },
		{"C", "National Stock Exchange" },
		{"D", "FINRA ADF" },
		{"E", "Market Independent (Generated by Nasdaq SIP)" },
		{"F", "Mutual Funds/Money Markets (NASDAQ)" },
		{"I", "International Securities Exchange" },
		{"J", "Direct Edge A" },
		{"K", "Direct Edge X" },
		{"M", "Chicago Stock Exchange" },
		{"N", "NYSE" },
		{"P", "NYSE Arca" },
		{"Q", "NASDAQ OMX" },
		{"S", "NASDAQ Small Cap" },
		{"T", "NASDAQ Int" },
		{"U", "OTCBB" },
		{"V", "OTC other" },
		{"W", "CBOE" },
		{"X", "NASDAQ OMX PSX" },
		{"G", "GLOBEX" },
		{"Y", "BATS Y-Exchange" },
		{"Z", "BATS" }
	};
	
	auto asop = std::make_shared<tradier_async_operation>();

	callback(beerja::status::ok, asop, std::move(ret));

	return asop;
}

namespace{
std::vector<beerja::ticker> parse_ticker_list(const jsondom::value& json){
	if(!json.is_object()){
		ASSERT(false)
		return std::vector<beerja::ticker>();
	}

	auto& root = json.object();
	auto securities_i = root.find("securities");
	if(securities_i == root.end() || !securities_i->second.is_object()){
		return std::vector<beerja::ticker>();
	}

	auto& securities = securities_i->second.object();
	auto security_i = securities.find("security");
	if(security_i == securities.end() || !security_i->second.is_array()){
		return std::vector<beerja::ticker>();
	}
	
	auto& security = security_i->second.array();

	std::vector<beerja::ticker> ret;

	for(auto& s : security){
		if(!s.is_object()){
			continue;
		}

		auto& o = s.object();

		auto symbol_i = o.find("symbol");
		if(symbol_i == o.end() || !symbol_i->second.is_string()){
			continue;
		}
		auto& symbol = symbol_i->second.string();

		std::string description;
		auto description_i = o.find("description");
		if(description_i != o.end() && description_i->second.is_string()){
			description = description_i->second.string();
		}

		std::string exchange;
		auto exchange_i = o.find("exchange");
		if(exchange_i != o.end() && exchange_i->second.is_string()){
			exchange = exchange_i->second.string();
		}

		ret.push_back(beerja::ticker{
			.id = symbol,
			.name = std::move(description),
			.exchange_id = std::move(exchange)
		});
	}
	
	return ret;
}
}

std::shared_ptr<beerja::async_operation> tradier::find_ticker(
		const std::string& query,
		std::function<void(
				beerja::status,
				const std::shared_ptr<beerja::async_operation>&,
				std::vector<beerja::ticker>&&
			)>&& callback
	)
{
	if(!callback){
		throw std::logic_error("tradier::find_ticker(): passed in callback is nullptr");
	}

	auto asop = std::make_shared<tradier_async_operation>();

	auto r = std::make_shared<easyhttp::request>([callback, asop](easyhttp::request& r){
		auto& resp = r.get_response();
		if(resp.status != easyhttp::status_code::ok || resp.response_code != easyhttp::http_code::ok){
			TRACE(<< "resp.status = " << unsigned(resp.status) << " resp.response_code = " << unsigned(resp.response_code) << std::endl)
			callback(beerja::status::failure, asop, std::vector<beerja::ticker>());
			return;
		}

		try{
			TRACE(<< "BODY = " << utki::make_string(resp.body) << std::endl)
			auto json = jsondom::read(utki::make_span(resp.body));

			callback(beerja::status::ok, asop, parse_ticker_list(json));
		}catch(...){
			callback(beerja::status::failure, asop, std::vector<beerja::ticker>());
		}
	});

	asop->http_req = r;

	r->set_url(end_point + "markets/search?q=" + easyhttp::escape(query) + "&indexes=false");

	r->set_headers({
			{"Authorization", std::string("Bearer ") + this->access_token},
			{"Accept", "application/json"}
		});

	r->start();

	return asop;
}

namespace{
beerja::quote parse_quote(const jsondom::value& json){
	auto& quote = json.object().at("quotes").object().at("quote").object();

	beerja::quote ret;

	ret.last = quote.at("last").number().to_float();
	ret.change = quote.at("change").number().to_float();
	ret.change_percent = quote.at("change_percentage").number().to_float();
	{ // check if trading is not closed yet
		auto& c = quote.at("close");
		if(c.is_null()){
			ret.close = -1;
		}else{
			ret.close = c.number().to_float();
		}
	}
	ret.open = quote.at("open").number().to_float();
	ret.high = quote.at("high").number().to_float();
	ret.low = quote.at("low").number().to_float();
	ret.volume = quote.at("volume").number().to_uint64();
	
	return ret;
}
}

std::shared_ptr<beerja::async_operation> tradier::get_quote(
		const std::string& symbol,
		std::function<void(
				beerja::status,
				const std::shared_ptr<beerja::async_operation>&,
				beerja::quote
			)>&& callback
	)
{
	if(!callback){
		throw std::logic_error("tradier::get_quote(): passed in callback is nullptr");
	}

	auto asop = std::make_shared<tradier_async_operation>();

	auto r = std::make_shared<easyhttp::request>([callback, asop](easyhttp::request& r){
		auto& resp = r.get_response();
		if(resp.status != easyhttp::status_code::ok || resp.response_code != easyhttp::http_code::ok){
			TRACE(<< "resp.status = " << unsigned(resp.status) << " resp.response_code = " << unsigned(resp.response_code) << std::endl)
			callback(beerja::status::failure, asop, beerja::quote());
			return;
		}

		try{
			TRACE(<< "BODY = " << utki::make_string(resp.body) << std::endl)
			auto json = jsondom::read(utki::make_span(resp.body));

			callback(beerja::status::ok, asop, parse_quote(json));
		}catch(...){
			callback(beerja::status::failure, asop, beerja::quote());
		}
	});

	asop->http_req = r;

	r->set_url(end_point + "markets/quotes?symbols=" + easyhttp::escape(symbol) + "&greeks=false");

	r->set_headers({
			{"Authorization", std::string("Bearer ") + this->access_token},
			{"Accept", "application/json"}
		});

	r->start();

	return asop;
}

namespace{
std::vector<beerja::granule> parse_prices(const jsondom::value& json){
	std::vector<beerja::granule> ret;

	// TODO:

	return ret;
}
}

std::shared_ptr<beerja::async_operation> tradier::get_prices(
		const std::string& symbol,
		std::chrono::system_clock::time_point now_time,
		granularity gran,
		std::function<void(
				beerja::status,
				const std::shared_ptr<beerja::async_operation>&,
				std::vector<beerja::granule>&& data
			)>&& callback
	)
{
	if(!callback){
		throw std::logic_error("tradier::get_quote(): passed in callback is nullptr");
	}

	auto asop = std::make_shared<tradier_async_operation>();

	if(gran == granularity::day){
		//TODO:
		ASSERT(false)
		return asop;
	}

	auto r = std::make_shared<easyhttp::request>([callback, asop](easyhttp::request& r){
		auto& resp = r.get_response();
		if(resp.status != easyhttp::status_code::ok || resp.response_code != easyhttp::http_code::ok){
			TRACE(<< "resp.status = " << unsigned(resp.status) << " resp.response_code = " << unsigned(resp.response_code) << std::endl)
			callback(beerja::status::failure, asop, std::vector<beerja::granule>());
			return;
		}

		try{
			TRACE(<< "BODY = " << utki::make_string(resp.body) << std::endl)
			auto json = jsondom::read(utki::make_span(resp.body));

			callback(beerja::status::ok, asop, parse_prices(json));
		}catch(...){
			callback(beerja::status::failure, asop, std::vector<beerja::granule>());
		}
	});

	asop->http_req = r;

	std::string interval;
	switch(gran){
		case granularity::minute:
			{
				interval = "1min";
			}
			break;
		case granularity::five_minutes:
			interval = "5min";
			break;
		case granularity::fivteen_minutes:
			interval = "15min";
			break;
		case granularity::day:
			ASSERT(false)
			break;
	}

	std::string start_time;
	std::string end_time;
	{
		using ::date::operator<<;
		std::stringstream ss;
		ss << now_time;
		end_time = ss.str();
		ss.clear();
		ss << backend::get_start_time(now_time, gran);
		start_time = ss.str();
	}
	
	TRACE(<< "interval = " << interval << std::endl)
	TRACE(<< "start_time = " << start_time << std::endl)
	TRACE(<< "end_time = " << end_time << std::endl)

	r->set_url(end_point + "markets/timesales?symbol=" + easyhttp::escape(symbol) +
			"&session_filter=open&interval=" + interval +
			"&start=" + easyhttp::escape(start_time) +
			"&end=" + easyhttp::escape(end_time)
		);

	r->set_headers({
			{"Authorization", std::string("Bearer ") + this->access_token},
			{"Accept", "application/json"}
		});

	r->start();

	return asop;
}
