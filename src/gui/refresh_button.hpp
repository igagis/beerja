#pragma once

#include <morda/widgets/button/nine_patch_push_button.hpp>
#include <morda/widgets/label/busy.hpp>

namespace beerja{

class refresh_button :
		virtual public morda::push_button,
		private morda::nine_patch_push_button
{
	std::shared_ptr<morda::widget> refresh_icon;
	std::shared_ptr<morda::busy> busy_indicator;
public:
	refresh_button(std::shared_ptr<morda::context> c, const puu::forest& desc);

	void set_refreshing(bool refreshing);
};

}
