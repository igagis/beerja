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

	void set_config(const treeml::forest& config)override;

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
