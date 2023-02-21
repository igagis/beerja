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

#include "search_ticker_widget.hpp"

#include <mordavokne/application.hpp>

#include <utki/unicode.hpp>
#include <utki/string.hpp>

#include <morda/widgets/input/text_input_line.hpp>
#include <morda/widgets/button/push_button.hpp>
#include <morda/widgets/label/busy.hpp>
#include <morda/widgets/label/color.hpp>
#include <morda/widgets/proxy/click_proxy.hpp>
#include <morda/widgets/proxy/key_proxy.hpp>
#include <morda/widgets/group/list.hpp>
#include <morda/widgets/group/overlay.hpp>
#include <morda/util/weak_widget_set.hpp>

#include "ticker_dialog.hpp"

using namespace beerja;

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

	utki::shared_ref<morda::widget> get_widget(size_t index)override;

	void set_tickers(std::vector<beerja::ticker>&& tickers){
		this->tickers = std::move(tickers);
		this->notify_data_set_change();
	}
};
}

namespace{
const auto layout = treeml::read(R"qwertyuiop(
	@column{
		layout{
			dx {max}
			dy {max}
		}
		@row{
			layout{
				dx {max}
			}
			@key_proxy{
				id {input_key_proxy}
				layout{
					dx {0}
					weight {1}
				}
				@text_input_line{
					id {query_text_input}

					text {"enter query here"}

					layout{
						dx {max}
					}
				}
			}
			@push_button{
				id {query_push_button}
				@text{
					text {"run query"}
				}
			}
		}
		@list{
			id {tickers_list}
			layout{
				dy {0}
				dx {fill}
				weight {1}
			}
		}
	}
	@busy{
		id {busy_spinner}
		visible {false}
	}
)qwertyuiop");
}

search_ticker_widget::search_ticker_widget(
		std::shared_ptr<morda::context> c,
		std::shared_ptr<beerja::backend> backend
	) :
		widget(std::move(c), treeml::forest()),
		morda::pile(
				this->context,
				layout
			),
		backend(std::move(backend))
{
	auto spinner = utki::make_shared_from(this->get_widget_as<morda::busy>("busy_spinner"));
	auto line = utki::make_shared_from(this->get_widget("query_text_input").get_widget<morda::text_input_line>());
	auto button = utki::make_shared_from(this->get_widget_as<morda::push_button>("query_push_button"));
	auto list = utki::make_shared_from(this->get_widget_as<morda::list>("tickers_list"));

	auto query_disable_widgets = std::make_shared<morda::weak_widget_set>();
	query_disable_widgets->add(line);
	query_disable_widgets->add(button);
	query_disable_widgets->add(list);

	auto tickers_provider = std::make_shared<ticker_list_provider>(this->backend);

	// button click handler
	button->click_handler = [this, query_disable_widgets, spinner, line, tickers_provider](morda::push_button& but){
		spinner->set_active(true);

		query_disable_widgets->set_enabled(false);

		LOG([](auto&o){o << "find ticker" << std::endl;})
		this->search_ticker_operation = this->backend->find_ticker(
				utki::to_utf8(line->get_text()),
				// backend operation complete handler
				[this, query_disable_widgets, spinner, tickers_provider](beerja::status s, const std::shared_ptr<beerja::async_operation>& asop, std::vector<beerja::ticker>&& ticker_list){
					LOG([&](auto&o){o << ticker_list.size() << " tickers found" << std::endl;})
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

	this->get_widget_as<morda::key_proxy>("input_key_proxy").key_handler = [button](morda::key_proxy&, const morda::key_event& e) -> bool {
		if(e.combo.key == morda::key::enter){
			if(e.is_down){
				button->click_handler(*button);
			}
			return true;
		}
		return false;
	};

	list->set_provider(tickers_provider);
}

search_ticker_widget::~search_ticker_widget(){
	if(auto op = this->search_ticker_operation.lock()){
		op->cancel();
	}
}

utki::shared_ref<morda::widget> ticker_list_provider::get_widget(size_t index){
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
		auto overlay = w.try_get_ancestor<morda::overlay>();
		if(overlay){
			w.context->run_from_ui_thread([overlay, ticker{ticker}, backend{this->backend}]()mutable{
				overlay->push_back(utki::make_shared_ref<ticker_dialog>(
						overlay->context,
						std::move(ticker),
						std::move(backend)
					));
			});
		}
	};

	return ret;
}
