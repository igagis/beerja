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

	void set_config(const puu::forest& config)override;

	void get_exchanges(std::function<void(beerja::status, std::vector<beerja::exchange>&&)>&& callback)override;

	std::shared_ptr<beerja::async_operation> find_ticker(
			const std::string& query,
			std::function<void(
					beerja::status,
					const std::shared_ptr<beerja::async_operation>&,
					std::vector<beerja::ticker>&&
				)>&& callback
		)override;
};
