#pragma once

#include <morda/widgets/group/book.hpp>
#include <morda/widgets/group/pile.hpp>
#include <morda/util/widget_set.hpp>

#include "../backend/backend.hpp"

class search_ticker_page :
		public morda::page,
		private morda::pile
{
public:
	search_ticker_page(std::shared_ptr<morda::context> c, std::shared_ptr<beerja::backend> be);

};
