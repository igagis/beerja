#pragma once

#include <morda/widgets/button/nine_patch_push_button.hpp>
#include <morda/widgets/label/spinner.hpp>

namespace beerja{

class refresh_button :
		virtual public morda::push_button,
		private morda::nine_patch_push_button
{
	std::shared_ptr<morda::spinner> refresh_label;
public:
	refresh_button(std::shared_ptr<morda::context> c, const puu::forest& desc);

	void set_refreshing(bool refreshing);
};

}
