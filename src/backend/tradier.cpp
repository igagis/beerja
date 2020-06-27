#include "tradier.hpp"

#include <utki/string.hpp>

#include <easyhttp/request.hpp>
#include <easyhttp/util.hpp>

#include <jsondom/dom.hpp>

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

	// TRACE(<< "API Key = " << this->access_token << std::endl)
}

void tradier::get_exchanges(std::function<void(beerja::status, std::vector<beerja::exchange>&&)>&& callback){
	// TODO:
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

		ret.push_back(beerja::ticker{
			.id = symbol,
			.name = std::move(description)
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
