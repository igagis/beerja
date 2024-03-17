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

#include "cells_container.hpp"

#include <ruis/layouts/layout.hpp>

using namespace beerja;

cells_container::cells_container(utki::shared_ref<ruis::context> c, const tml::forest& desc) :
		ruis::widget(c, desc),
		ruis::container(this->context, tml::forest(), ruis::layout::column)
{}

void cells_container::set_num_cells_per_row(unsigned num){
	if(num == 0){
		throw std::logic_error("cells_container::set_num_cells_per_row(): passed in argument is 0, minimum number of cells per row is 1");
	}
	if(this->num_cells_per_row == num){
		return;
	}
	this->num_cells_per_row = num;
	this->update_arrangement();
}

ruis::container& cells_container::push_new_row(){
	auto r = utki::make_shared<ruis::container>(this->context, tml::forest(), ruis::layout::row);
	this->push_back(r);
	this->children().back().get().get_layout_params().dims.x() = ruis::lp::max;
	return r.get();
}

void cells_container::push(utki::shared_ref<ruis::widget> w){
	if(this->children().empty() || dynamic_cast<ruis::container&>(this->children().back().get()).children().size() == this->num_cells_per_row){
		this->push_new_row();
	}
	auto& row = dynamic_cast<ruis::container&>(this->children().back().get());

	auto& lp = w.get().get_layout_params();
	lp.weight = 1;
	lp.dims.x() = ruis::lp::fill;

	row.push_back(w);
}

void cells_container::erase(ruis::widget& w){
	for(auto& r : this->children()){
		auto& row = dynamic_cast<ruis::container&>(r.get());
		auto i = row.find(w);
		if(i != row.children().end()){
			row.erase(i);
			this->update_arrangement();
			return;
		}
	}
	throw std::logic_error("cells_container::erase(): could not find requested widget");
}

void cells_container::update_arrangement(){
	std::vector<utki::shared_ref<ruis::widget>> widgets;
	for(auto& r : this->children()){
		auto& row = dynamic_cast<ruis::container&>(r.get());
		for(auto& c : row.children()){
			widgets.push_back(c);
		}
	}
	this->clear();

	for(auto i = widgets.begin(); i != widgets.end();){
		auto& row = this->push_new_row();

		for(unsigned n = 0; n != this->num_cells_per_row && i != widgets.end(); ++n, ++i){
			row.push_back(*i);
		}
	}
}
