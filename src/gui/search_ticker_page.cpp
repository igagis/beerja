#include "search_ticker_page.hpp"

#include <mordavokne/application.hpp>

#include <utki/utf8.hpp>
#include <utki/string.hpp>

#include <morda/widgets/input/text_input_line.hpp>
#include <morda/widgets/button/push_button.hpp>
#include <morda/widgets/label/busy.hpp>
#include <morda/widgets/group/list.hpp>
#include <morda/util/widget_set.hpp>

search_ticker_page::search_ticker_page(std::shared_ptr<morda::context> c, std::shared_ptr<beerja::backend> be) :
		widget(std::move(c), puu::forest()),
		morda::page(this->context, puu::forest()),
		morda::pile(
				this->context,
				puu::read(*mordavokne::inst().get_res_file("res/search_ticker_page.gui"))
			),
		tickers_provider(std::make_shared<ticker_list_provider>())
{
	auto spinner = this->try_get_widget_as<morda::busy>("busy_spinner");
	ASSERT(spinner)
	auto line = this->get_widget("query_text_input").try_get_widget<morda::text_input_line>();
	ASSERT(line)
	auto button = this->try_get_widget_as<morda::push_button>("query_push_button");
	ASSERT(button)
	auto list = this->try_get_widget_as<morda::list>("tickers_list");
	ASSERT(list)

	auto query_disable_widgets = std::make_shared<morda::widget_set>();
	query_disable_widgets->add(line);
	query_disable_widgets->add(button);
	query_disable_widgets->add(list);

	auto tickers_provider = this->tickers_provider;

	// button click handler
	button->click_handler = [query_disable_widgets, spinner, line, be, tickers_provider](morda::push_button& but){
		spinner->set_active(true);

		query_disable_widgets->set_enabled(false);

		TRACE(<< "find ticker" << std::endl)
		auto asop = be->find_ticker(
				utki::to_utf8(line->get_text()),
				// backend operation complete handler
				[query_disable_widgets, spinner, tickers_provider](beerja::status s, const std::shared_ptr<beerja::async_operation>& asop, std::vector<beerja::ticker>&& ticker_list){
					TRACE(<< ticker_list.size() << " tickers found" << std::endl)
					// run from ui thread
					spinner->context->run_from_ui_thread([spinner, query_disable_widgets, ticker_list{std::move(ticker_list)}, tickers_provider]()mutable{
						tickers_provider->set_tickers(std::move(ticker_list));
						spinner->set_active(false);
						query_disable_widgets->set_enabled(true);
					});
				}
			);
	};

	list->set_provider(this->tickers_provider);
}

std::shared_ptr<morda::widget> ticker_list_provider::get_widget(size_t index){
	ASSERT(index < this->tickers.size());
	auto& t = this->tickers[index];

	auto gui = utki::make_string(
			R"qwertyuiop(
				@column{
					layout{dx{fill}}
					@left{
						@text{
							text{"%s"}
							color{0xffffff00}
						}
					}
					@left{
						@text{
							text{"%s"}
							color{0xffaaaaaa}
						}
					}
					@color{
						layout{dy{1} dx{fill}}
						color{0xff00ff00}
					}
				}
			)qwertyuiop",
			t.id.c_str(),
			t.name.c_str()
		);

	return this->get_list()->context->inflater.inflate(gui.c_str()); // TODO: remove .c_str()
}
