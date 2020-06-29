#pragma once

#include <morda/widgets/group/book.hpp>
#include <morda/widgets/group/pile.hpp>
#include <morda/widgets/group/list.hpp>

#include "../backend/backend.hpp"

class ticker_list_provider : public morda::list::provider{
	std::vector<beerja::ticker> tickers;
public:
	size_t count()const noexcept override{
		return this->tickers.size();
	}

	std::shared_ptr<morda::widget> get_widget(size_t index)override;

	void set_tickers(std::vector<beerja::ticker>&& tickers){
		this->tickers = std::move(tickers);
		this->notify_data_set_changed();
	}
};

class search_ticker_page :
		public morda::page,
		private morda::pile
{
	std::shared_ptr<ticker_list_provider> tickers_provider;
public:
	search_ticker_page(std::shared_ptr<morda::context> c, std::shared_ptr<beerja::backend> be);

};
