
#include <mordavokne/application.hpp>

class application : public mordavokne::application{
public:
	application() :
			mordavokne::application(
					"stocks",
					[](){
						return mordavokne::window_params(r4::vec2ui(640, 480));
					}()
				)
	{
		this->gui.initStandardWidgets(*this->get_res_file());
		
		this->gui.context->loader.mount_res_pack(*this->get_res_file("res/"));
		
		// this->gui.context->inflater.register_widget<morda::Gauge>("Gauge");
		// this->gui.context->inflater.register_widget<CubeWidget>("CubeWidget");
		
		auto c = this->gui.context->inflater.inflate(
				*this->get_res_file("res/main.gui")
			);

		this->gui.set_root(c);
	}
};

std::unique_ptr<mordavokne::application> mordavokne::create_application(int argc, const char** argv){
	return std::make_unique<::application>();
}
