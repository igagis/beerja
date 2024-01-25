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

#pragma once

#include <ruis/widgets/button/nine_patch_push_button.hpp>
#include <ruis/widgets/label/spinner.hpp>

namespace beerja{

class refresh_button : public ruis::nine_patch_push_button
{
	std::shared_ptr<ruis::spinner> refresh_label;
public:
	refresh_button(const utki::shared_ref<ruis::context>& c, const treeml::forest& desc);

	void set_refreshing(bool refreshing);
};

}
