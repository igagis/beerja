#pragma once

#include <string>
#include <functional>

#include <puu/tree.hpp>

#include "../model/exchange.hpp"
#include "../model/ticker.hpp"
#include "../model/quote.hpp"

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
};

}
