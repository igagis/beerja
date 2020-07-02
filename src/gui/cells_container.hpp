#pragma once

#include <morda/widgets/group/column.hpp>

class cells_container :
        virtual public morda::widget,
        private morda::column
{
    unsigned num_cells_per_row = 3;
public:
    cells_container(std::shared_ptr<morda::context> c, const puu::forest& desc);

    void set_num_cells_per_row(unsigned num);

	void push(std::shared_ptr<morda::widget> w);

	void erase(morda::widget& w);
private:
	void update_arrangement();
	morda::container& push_new_row();
};
