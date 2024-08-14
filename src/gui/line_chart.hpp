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

#include <ruis/widget/base/color_widget.hpp>
#include <ruis/paint/path_vao.hpp>

#include <vector>

namespace beerja{

class line_chart : virtual public ruis::color_widget{
	std::vector<float> points;

	ruis::path_vao path_vao;
public:
	line_chart(const utki::shared_ref<ruis::context>& c, const tml::forest& desc);

	void set_points(std::vector<float>&& points);

	void on_resize()override;

	void render(const ruis::matrix4& matrix)const override;

private:
	void update_path_vba();
};

}
