#pragma once

#include <morda/widgets/base/color_widget.hpp>
#include <morda/paint/path_vba.hpp>

#include <vector>

namespace beerja{

class line_chart : virtual public morda::color_widget{
	std::vector<float> points;

	// morda::path_vba path_vba;
public:
	line_chart(std::shared_ptr<morda::context> c, const puu::forest& desc);

	void set_points(std::vector<float>&& points);

	void on_resize()override;

	void render(const morda::matrix4& matrix)const override;

private:
	void update_path_vba();
};

}
