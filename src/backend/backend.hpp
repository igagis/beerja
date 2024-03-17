/*
beerja - stock screener

Copyright (C) 2020-2021  Ivan Gagis <igagis@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

/* ================ LICENSE END ================ */

#pragma once

#include <string>
#include <functional>
#include <chrono>

#include <tml/tree.hpp>

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

	virtual void set_config(const tml::forest& config) = 0;

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
			::date::sys_time<std::chrono::minutes> from,
			::date::sys_time<std::chrono::minutes> to,
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
