#include "ticker_dialog.hpp"

#include <morda/widgets/proxy/mouse_proxy.hpp>
#include <morda/widgets/label/text.hpp>
#include <morda/widgets/button/push_button.hpp>

using namespace beerja;

namespace{
const auto dialog_desc = puu::read(R"qwertyuiop(
		layout{
			dx {max}
			dy {max}
		}
		@mouse_proxy{
			id {bg_mouse_proxy}
			layout{
				dx {fill}
				dy {fill}
			}
		}
		@color{
			layout{
				dx {fill}
				dy {fill}
			}
			color {0xa0000000}
		}
		@nine_patch{
			image {morda_npt_window_bg}

			@column{
				@row{
					@text{
						id {ticker_name_text}
					}

					@text{
						id {price_text}
					}

					@text{
						id {change_percent_text}
					}

					@text{
						id {change_text}
					}

					@push_button{
						id {add_to_grid}
						@text{
							text {"add to grid"}
						}
					}

					@u_refresh_button{
						id {refresh_button}
					}

					@push_button{
						id {close_button}
						@text{
							text{X}
						}
					}
				}

				@pile{
					layout{
						dx {fill}
						dy {min}
					}
					@ratio_proxy{
						layout{
							dx {100}
							dy {min}
						}
						ratio {2}
					}
					@u_line_chart{
						layout{
							dx {fill}
							dy {fill}
						}
					}
				}
			}
		}
	)qwertyuiop");
}

ticker_dialog::ticker_dialog(
		std::shared_ptr<morda::context> c,
		beerja::ticker&& ticker,
		std::shared_ptr<beerja::backend> backend
	) :
		morda::widget(std::move(c), dialog_desc),
		morda::pile(this->context, dialog_desc),
		ticker(std::move(ticker)),
		backend(std::move(backend))
{
	auto& bg_mouse_proxy = this->get_widget_as<morda::mouse_proxy>("bg_mouse_proxy");
	bg_mouse_proxy.mouse_button_handler = [](morda::mouse_proxy&, const morda::mouse_button_event& e){
		return true;
	};

	auto& ticker_name_text = this->get_widget_as<morda::text>("ticker_name_text");
	ticker_name_text.set_text(this->ticker.name);

	this->price_text = utki::make_shared_from(this->get_widget_as<morda::text>("price_text"));	
	this->change_percent_text = utki::make_shared_from(this->get_widget_as<morda::text>("change_percent_text"));
	this->change_text = utki::make_shared_from(this->get_widget_as<morda::text>("change_text"));

	{
		auto& b = this->get_widget_as<beerja::refresh_button>("refresh_button");
		b.click_handler = [this](morda::push_button& button){
			this->refresh();
		};
		this->refresh_button = utki::make_shared_from(b);
	}

	{
		auto& b = this->get_widget_as<morda::push_button>("close_button");
		b.click_handler = [this](morda::push_button& button){
			button.context->run_from_ui_thread([this](){
				this->remove_from_parent();
			});
		};
	}

	this->context->run_from_ui_thread([this](){
		this->refresh();
	});
}

void ticker_dialog::refresh(){
	this->refresh_button->set_refreshing(true);

	this->refresh_operation = this->backend->get_quote(
			this->ticker.id,
			[self{utki::make_shared_from(*this)}, refresh_button{this->refresh_button}](
					beerja::status s,
					const std::shared_ptr<beerja::async_operation>& asop,
					beerja::quote quote
				)
			{
				if(s != beerja::status::ok){
					TRACE(<< "get_quote(): operation failed!" << std::endl)
					self->context->run_from_ui_thread([self, refresh_button](){
						refresh_button->set_refreshing(false);
					});
				}else{
					TRACE(<< "quote.last = " << quote.last << std::endl)
					self->context->run_from_ui_thread([self, refresh_button, quote{std::move(quote)}](){
						refresh_button->set_refreshing(false);
						
						self->price_text->set_text(std::to_string(quote.last));
						self->change_text->set_text(std::to_string(quote.change));
						self->change_percent_text->set_text(std::to_string(quote.change_percent));
					});
				}
			}
		);
	
	using ::date::floor;

	this->get_prices_operation = this->backend->get_prices(
			this->ticker.id,
			floor<std::chrono::minutes>(std::chrono::system_clock::now() - ::date::days(7)),
			floor<std::chrono::minutes>(std::chrono::system_clock::now()),
			beerja::backend::granularity::minute,
			[self{utki::make_weak_from(*this)}](
					beerja::status s,
					const std::shared_ptr<beerja::async_operation> asop,
					std::vector<beerja::granule>&& prices
				)
			{
				// TODO:
			}
		);
}

ticker_dialog::~ticker_dialog(){
	if(auto op = this->refresh_operation.lock()){
		op->cancel();
	}
	if(auto op = this->get_prices_operation.lock()){
		op->cancel();
	}
}
