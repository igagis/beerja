#pragma once

#include <morda/widgets/base/color_widget.hpp>
#include <morda/paint/path_vao.hpp>

#include <vector>

namespace beerja{

class line_chart : virtual public morda::color_widget{
	std::vector<float> points;

	morda::path_vao path_vao;
public:
	line_chart(std::shared_ptr<morda::context> c, const treeml::forest& desc);

	void set_points(std::vector<float>&& points);

	void on_resize()override;

	void render(const morda::matrix4& matrix)const override;

private:
	void update_path_vba();
};

}
