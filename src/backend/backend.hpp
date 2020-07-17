#pragma once

#include <string>
#include <functional>
#include <chrono>

#include <puu/tree.hpp>

#include "../model/exchange.hpp"
#include "../model/ticker.hpp"
#include "../model/quote.hpp"
#include "../model/granule.hpp"

#include <utki/shared.hpp>

namespace beerja{

enum class status{
	ok,
	failure
};

class async_operation : public utki::shared{
public:
	virtual bool cancel() = 0;
};

class backend : public utki::shared{
public:
	virtual ~backend()noexcept{}

	virtual const std::string& id()const = 0;

	virtual void set_config(const puu::forest& config) = 0;

	virtual std::shared_ptr<async_operation> get_exchanges(
			std::function<void(
					status,
					std::shared_ptr<async_operation>,
					std::vector<exchange>&&
				)>&& callback
		) = 0;

	virtual std::shared_ptr<async_operation> find_ticker(
			const std::string& query,
			std::function<void(
					status,
					const std::shared_ptr<async_operation>&,
					std::vector<ticker>&&
				)>&& callback
		) = 0;
	
	virtual std::shared_ptr<async_operation> get_quote(
			const std::string& symbol,
			std::function<void(
					status,
					const std::shared_ptr<async_operation>&,
					beerja::quote
				)>&& callback
		) = 0;

	enum class granularity{
		minute,
		five_minutes,
		fivteen_minutes,
		day
	};

	virtual std::shared_ptr<async_operation> get_prices(
			const std::string& symbol,
			::date::sys_time<std::chrono::minutes> now_time,
			granularity gran,
			std::function<void(
					status,
					const std::shared_ptr<async_operation>&,
					std::vector<beerja::granule>&& data
				)>&& callback
		) = 0;
	
protected:
	static std::chrono::system_clock::time_point get_start_time(
			const std::chrono::system_clock::time_point& end_time,
			granularity gran
		);
};

}
