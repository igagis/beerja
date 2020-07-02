#include "cells_container.hpp"

cells_container::cells_container(std::shared_ptr<morda::context> c, const puu::forest& desc) :
		morda::widget(std::move(c), desc),
		morda::column(this->context, puu::forest())
{

}

void cells_container::set_num_cells_per_row(unsigned num){
	this->num_cells_per_row = num;
}
