#include "refresh_button.hpp"

using namespace beerja;

namespace{
const auto layout = puu::read(R"qwertyuiop(
	@text{
		id {refresh}
		text {"refresh"}
	}
	@busy{
		id {busy}
	}
)qwertyuiop");
}

refresh_button::refresh_button(std::shared_ptr<morda::context> c, const puu::forest& desc) :
		morda::widget(std::move(c), desc),
		morda::button(this->context, desc),
		morda::push_button(this->context, desc),
		morda::nine_patch_push_button(this->context, layout)
{
	this->refresh_icon = utki::make_shared_from(this->get_widget("refresh"));
	this->busy_indicator = utki::make_shared_from(this->get_widget_as<morda::busy>("busy"));
}

void refresh_button::set_refreshing(bool refreshing){
	this->set_enabled(!refreshing);
	this->refresh_icon->set_visible(!refreshing);
	this->busy_indicator->set_active(refreshing);
}
