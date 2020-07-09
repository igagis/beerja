#include "search_ticker_widget.hpp"

#include <mordavokne/application.hpp>

#include <utki/unicode.hpp>
#include <utki/string.hpp>

#include <morda/widgets/input/text_input_line.hpp>
#include <morda/widgets/button/push_button.hpp>
#include <morda/widgets/label/busy.hpp>
#include <morda/widgets/label/color.hpp>
#include <morda/widgets/proxy/click_proxy.hpp>
#include <morda/widgets/group/list.hpp>
#include <morda/widgets/group/overlay.hpp>
#include <morda/util/weak_widget_set.hpp>

#include "ticker_dialog.hpp"

namespace{
class ticker_list_provider : public morda::list::provider{
	std::vector<beerja::ticker> tickers;

	std::shared_ptr<beerja::backend> backend;
public:
	ticker_list_provider(std::shared_ptr<beerja::backend> backend) :
			backend(std::move(backend))
	{}

	size_t count()const noexcept override{
		return this->tickers.size();
	}

	std::shared_ptr<morda::widget> get_widget(size_t index)override;

	void set_tickers(std::vector<beerja::ticker>&& tickers){
		this->tickers = std::move(tickers);
		this->notify_data_set_change();
	}
};
}

search_ticker_widget::search_ticker_widget(
		std::shared_ptr<morda::context> c,
		std::shared_ptr<beerja::backend> backend
	) :
		widget(std::move(c), puu::forest()),
		morda::pile(
				this->context,
				puu::read(*mordavokne::inst().get_res_file("res/search_ticker_widget.gui"))
			),
		backend(std::move(backend))
{
	auto spinner = this->try_get_widget_as<morda::busy>("busy_spinner");
	ASSERT(spinner)
	auto line = this->get_widget("query_text_input").try_get_widget<morda::text_input_line>();
	ASSERT(line)
	auto button = this->try_get_widget_as<morda::push_button>("query_push_button");
	ASSERT(button)
	auto list = this->try_get_widget_as<morda::list>("tickers_list");
	ASSERT(list)

	auto query_disable_widgets = std::make_shared<morda::weak_widget_set>();
	query_disable_widgets->add(line);
	query_disable_widgets->add(button);
	query_disable_widgets->add(list);

	auto tickers_provider = std::make_shared<ticker_list_provider>(this->backend);

	// button click handler
	button->click_handler = [this, query_disable_widgets, spinner, line, tickers_provider](morda::push_button& but){
		spinner->set_active(true);

		query_disable_widgets->set_enabled(false);

		TRACE(<< "find ticker" << std::endl)
		ASSERT(!this->search_ticker_operation)
		this->search_ticker_operation = this->backend->find_ticker(
				utki::to_utf8(line->get_text()),
				// backend operation complete handler
				[this, query_disable_widgets, spinner, tickers_provider](beerja::status s, const std::shared_ptr<beerja::async_operation>& asop, std::vector<beerja::ticker>&& ticker_list){
					TRACE(<< ticker_list.size() << " tickers found" << std::endl)
					// run from ui thread
					spinner->context->run_from_ui_thread([this, spinner, query_disable_widgets, ticker_list{std::move(ticker_list)}, tickers_provider]()mutable{
						tickers_provider->set_tickers(std::move(ticker_list));
						spinner->set_active(false);
						query_disable_widgets->set_enabled(true);
						this->search_ticker_operation.reset();
					});
				}
			);
	};

	list->set_provider(tickers_provider);
}

std::shared_ptr<morda::widget> ticker_list_provider::get_widget(size_t index){
	ASSERT(index < this->tickers.size());
	auto& t = this->tickers[index];

	auto gui = utki::make_string(
			R"qwertyuiop(
				@pile{
					layout{
						dx {fill}
					}
					@click_proxy{
						id {click_proxy}
						layout{
							dx {fill}
							dy {fill}
						}
					}
					@color{
						id {bg_color}
						layout{
							dx {fill}
							dy {fill}
						}
						color {0xff000000}
					}
					@column{
						layout{
							dx {fill}
						}
						@left{
							@text{
								text{"%s"}
								color{0xff00ffff}
							}
						}
						@left{
							@text{
								text{"%s"}
								color{0xff808080}
							}
						}
						@color{
							layout{dy{1} dx{fill}}
							color{0xff404040}
						}
					}
				}
			)qwertyuiop",
			t.id.c_str(),
			t.name.c_str()
		);

	auto ret = this->get_list()->context->inflater.inflate(gui);

	auto& bg = ret->get_widget_as<morda::color>("bg_color");
	auto& cp = ret->get_widget_as<morda::click_proxy>("click_proxy");

	cp.press_change_handler = [bg{utki::make_shared_from(bg)}](morda::click_proxy& w) -> bool {
		if(w.is_pressed()){
			bg->set_color(0xff808080);
		}else{
			bg->set_color(0xff000000);
		}
		return true;
	};

	cp.click_handler = [ticker{t}, this](morda::click_proxy& w){
		auto overlay = w.find_ancestor<morda::overlay>();
		if(overlay){
			w.context->run_from_ui_thread([overlay, ticker{ticker}, backend{this->backend}]()mutable{
				overlay->push_back(std::make_shared<ticker_dialog>(
						overlay->context,
						std::move(ticker),
						std::move(backend)
					));
			});
		}
	};

	return ret;
}
