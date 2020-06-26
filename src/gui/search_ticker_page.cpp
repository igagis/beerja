#include "search_ticker_page.hpp"

#include <utki/utf8.hpp>

#include <morda/widgets/input/text_input_line.hpp>
#include <morda/widgets/button/push_button.hpp>
#include <morda/widgets/label/busy.hpp>

search_ticker_page::search_ticker_page(std::shared_ptr<morda::context> c, std::shared_ptr<beerja::backend> be) :
		widget(std::move(c), puu::forest()),
		morda::page(this->context, puu::forest()),
		morda::pile(
				this->context,
				puu::read(R"qwertyuiop(
					@column{
						@text{
							text{"Hello beerja, ticker search!"}
						}
						@text_input_line{
							id{query_text_input}

							text{"enter query here"}

							layout{
								dx{max} dy{min}
							}
						}
						@push_button{
							id{query_push_button}
							@text{
								text{"run query"}
							}
						}
						@busy{
							id{busy_spinner}
						}
					}
				)qwertyuiop")
			)
{
	auto spinner = this->try_get_widget_as<morda::busy>("busy_spinner");
	ASSERT(spinner)
	auto line = this->get_widget("query_text_input").try_get_widget<morda::text_input_line>();
	ASSERT(line)
	auto button = this->try_get_widget_as<morda::push_button>("query_push_button");
	ASSERT(button)

	button->click_handler = [button, spinner, line, be](morda::push_button& but){
		spinner->set_active(true);
		button->set_enabled(false);
		line->set_enabled(false);
		TRACE(<< "find ticker" << std::endl)
		auto asop = be->find_ticker(
				utki::to_utf8(line->get_text()),
				[spinner, button, line](beerja::status s, const std::shared_ptr<beerja::async_operation>& asop, std::vector<beerja::ticker>&& ticker_list){
					TRACE(<< ticker_list.size() << " tickers found" << std::endl)
					spinner->context->run_from_ui_thread([spinner, button, line](){
						spinner->set_active(false);
						button->set_enabled(true);
						line->set_enabled(true);
					});
				}
			);
	};
}
