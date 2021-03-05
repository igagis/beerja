#include "refresh_button.hpp"

using namespace beerja;

namespace{
const auto layout = treeml::read(R"qwertyuiop(
	@refresh{
		id {refresh}
	}
)qwertyuiop");
}

refresh_button::refresh_button(std::shared_ptr<morda::context> c, const treeml::forest& desc) :
		morda::widget(std::move(c), desc),
		morda::button(this->context, desc),
		morda::push_button(this->context, desc),
		morda::nine_patch_push_button(this->context, layout)
{
	this->refresh_label = this->try_get_widget_as<morda::spinner>("refresh");
}

void refresh_button::set_refreshing(bool refreshing){
	this->set_enabled(!refreshing);
	this->refresh_label->set_active(refreshing);
}
