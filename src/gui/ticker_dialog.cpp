#include "ticker_dialog.hpp"

#include <morda/widgets/proxy/mouse_proxy.hpp>
#include <morda/widgets/label/text.hpp>

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

			@text{
				id {title_text}
				text {"Hello beerja!"}
			}
		}
	)qwertyuiop");
}

ticker_dialog::ticker_dialog(std::shared_ptr<morda::context> c, const std::string& ticker_symbol_name) :
		morda::widget(std::move(c), dialog_desc),
		morda::pile(this->context, dialog_desc)
{
	auto& bg_mouse_proxy = this->get_widget_as<morda::mouse_proxy>("bg_mouse_proxy");
	bg_mouse_proxy.mouse_button_handler = [](morda::mouse_proxy&, bool is_down, const morda::vector2&, morda::mouse_button, unsigned){
		return true;
	};

	auto& title_text = this->get_widget_as<morda::text>("title_text");
	title_text.set_text(ticker_symbol_name);
}
