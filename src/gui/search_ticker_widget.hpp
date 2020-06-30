#pragma once

#include <morda/widgets/group/pile.hpp>

#include "../backend/backend.hpp"

class search_ticker_widget :
		virtual public morda::widget,
		private morda::pile
{
public:
	search_ticker_widget(std::shared_ptr<morda::context> c, std::shared_ptr<beerja::backend> be);

};
