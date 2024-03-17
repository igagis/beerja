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

#include "backend.hpp"

#include "../model/exchange.hpp"

class tradier : public beerja::backend{
	std::string access_token;
public:
	static const std::string tag;

	const std::string& id()const override{
		return tag;
	}

	void set_config(const tml::forest& config)override;

	virtual std::shared_ptr<beerja::async_operation> get_exchanges(
			std::function<void(
					beerja::status,
					std::shared_ptr<beerja::async_operation>,
					std::vector<beerja::exchange>&&
				)>&& callback
		)override;

	std::shared_ptr<beerja::async_operation> find_ticker(
			const std::string& query,
			std::function<void(
					beerja::status,
					const std::shared_ptr<beerja::async_operation>&,
					std::vector<beerja::ticker>&&
				)>&& callback
		)override;

	virtual std::shared_ptr<beerja::async_operation> get_quote(
			const std::string& symbol,
			std::function<void(
					beerja::status,
					const std::shared_ptr<beerja::async_operation>&,
					beerja::quote
				)>&& callback
		)override;

	virtual std::shared_ptr<beerja::async_operation> get_prices(
			const std::string& symbol,
			::date::sys_time<std::chrono::minutes> from,
			::date::sys_time<std::chrono::minutes> to,
			granularity gran,
			std::function<void(
					beerja::status,
					const std::shared_ptr<beerja::async_operation>&,
					std::vector<beerja::granule>&& data
				)>&& callback
		)override;
};
