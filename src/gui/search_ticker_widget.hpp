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

#include <ruis/widgets/container.hpp>

#include "../backend/backend.hpp"

namespace beerja{

class search_ticker_widget :
		virtual public ruis::widget,
		private ruis::container
{
	std::shared_ptr<beerja::backend> backend;

	std::weak_ptr<beerja::async_operation> search_ticker_operation;
public:
	search_ticker_widget(
			const utki::shared_ref<ruis::context>& c,
			std::shared_ptr<beerja::backend> backend
		);

	~search_ticker_widget();
};

}
