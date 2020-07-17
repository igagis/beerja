#pragma once

#include "../backend/backend.hpp"

#include <morda/widgets/group/pile.hpp>
#include <morda/widgets/label/text.hpp>
#include <morda/widgets/label/busy.hpp>

#include "refresh_button.hpp"

namespace beerja{

class ticker_dialog : public morda::pile{
	beerja::ticker ticker;

	std::shared_ptr<beerja::backend> backend;

	std::shared_ptr<morda::text> price_text;
	std::shared_ptr<morda::text> change_percent_text;
	std::shared_ptr<morda::text> change_text;

	std::shared_ptr<beerja::refresh_button> refresh_button;

	std::weak_ptr<beerja::async_operation> refresh_operation;
	std::weak_ptr<beerja::async_operation> get_prices_operation;
public:
	ticker_dialog(
			std::shared_ptr<morda::context> c,
			beerja::ticker&& ticker,
			std::shared_ptr<beerja::backend> backend
		);

	~ticker_dialog();

	ticker_dialog(const ticker_dialog&) = delete;
	ticker_dialog& operator=(const ticker_dialog&) = delete;

private:
	void refresh();
};

}
