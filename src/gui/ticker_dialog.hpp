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
			const utki::shared_ref<morda::context>& c,
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
