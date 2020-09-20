#include "line_chart.hpp"

using namespace beerja;

line_chart::line_chart(std::shared_ptr<morda::context> c, const puu::forest& desc) :
		morda::widget(std::move(c), desc),
		morda::color_widget(this->context, desc)
{}

void line_chart::set_points(std::vector<float>&& points){
	this->points = std::move(points);
	
	this->update_path_vba();
}

void line_chart::on_resize(){
	this->update_path_vba();
}

void line_chart::update_path_vba(){
	morda::path p;

	if(this->points.size() < size_t(this->rect().d.x())){
		// TODO:
	}else{
		// TODO:
	}
}

void line_chart::render(const morda::matrix4& matrix)const{
	this->path_vao.render(matrix, this->get_color());
}
