#include "cells_container.hpp"

#include <morda/widgets/group/row.hpp>

cells_container::cells_container(std::shared_ptr<morda::context> c, const puu::forest& desc) :
		morda::widget(std::move(c), desc),
		morda::column(this->context, puu::forest())
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

void cells_container::push(std::shared_ptr<morda::widget> w){
	ASSERT(w)
	if(this->children().empty() || dynamic_cast<morda::row&>(*this->children().back()).children().size() == this->num_cells_per_row){
		this->push_back(std::make_shared<morda::row>(this->context, puu::forest()));
	}
	auto& row = dynamic_cast<morda::row&>(*this->children().back());

	auto& lp = row.get_layout_params_as<morda::linear_container::layout_params>(*w);
	lp.weight = 1;

	row.push_back(std::move(w));
}

void cells_container::erase(morda::widget& w){
	for(auto& r : this->children()){
		auto& row = dynamic_cast<morda::row&>(*r);
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
	
}