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

#include "refresh_button.hpp"

using namespace beerja;

namespace{
const auto layout = treeml::read(R"qwertyuiop(
	@refresh{
		id {refresh}
	}
)qwertyuiop");
}

refresh_button::refresh_button(const utki::shared_ref<morda::context>& c, const treeml::forest& desc) :
		morda::widget(std::move(c), desc),
		morda::button(this->context, desc),
		morda::push_button(this->context, desc),
		morda::nine_patch_push_button(this->context, ::layout)
{
	this->refresh_label = this->try_get_widget_as<morda::spinner>("refresh");
}

void refresh_button::set_refreshing(bool refreshing){
	this->set_enabled(!refreshing);
	this->refresh_label->set_active(refreshing);
}
