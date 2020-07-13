#pragma once

#include <morda/widgets/group/pile.hpp>

#include "../backend/backend.hpp"

namespace beerja{

class search_ticker_widget :
		virtual public morda::widget,
		private morda::pile
{
	std::shared_ptr<beerja::backend> backend;

	std::shared_ptr<beerja::async_operation> search_ticker_operation;
public:
	search_ticker_widget(
			std::shared_ptr<morda::context> c,
			std::shared_ptr<beerja::backend> backend
		);

};

}
