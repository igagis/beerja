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

#include "line_chart.hpp"

#include <ruis/context.hpp>

using namespace beerja;

line_chart::line_chart(const utki::shared_ref<ruis::context>& c, const tml::forest& desc) :
		ruis::widget(c, desc),
		ruis::color_widget(this->context, desc),
		path_vao(this->context.get().renderer)
{}

void line_chart::set_points(std::vector<float>&& points){
	this->points = std::move(points);
	
	this->update_path_vba();
}

void line_chart::on_resize(){
	this->update_path_vba();
}

void line_chart::update_path_vba(){
	ruis::path p;

	if(this->points.size() < size_t(this->rect().d.x())){
		// TODO:
	}else{
		// TODO:
	}
}

void line_chart::render(const ruis::matrix4& matrix)const{
	this->path_vao.render(matrix, this->get_color());
}
