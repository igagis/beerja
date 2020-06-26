
#include <mordavokne/application.hpp>

#include <utki/debug.hpp>

#include <papki/fs_file.hpp>

#include <easyhttp/init_guard.hpp>

#include <morda/widgets/group/book.hpp>

#include "backend_register.hpp"

#include "backend/tradier.hpp"

#include "gui/search_ticker_page.hpp"

class application : public mordavokne::application{
	easyhttp::init_guard easyhttp_guard;
public:
	backend_register backends;

	application() :
			mordavokne::application(
					"beerja",
					[](){
						return mordavokne::window_params(r4::vec2ui(640, 480));
					}()
				)
	{
		this->gui.initStandardWidgets(*this->get_res_file());
		
		this->gui.context->loader.mount_res_pack(*this->get_res_file("res/"));

		{
			auto be = std::make_shared<tradier>();
			papki::fs_file cfg_file(this->storage_dir + "backend/" + be->id() + ".puu");
			if(cfg_file.exists()){
				be->set_config(puu::read(cfg_file));
			}
			ASSERT(be)
			std::string id(be->id());
			this->backends.register_backend(std::move(id), std::move(be));
		}

		auto c = this->gui.context->inflater.inflate(
				*this->get_res_file("res/main.gui")
			);
		ASSERT(c)

		c->get_widget_as<morda::book>("main_book").push(std::make_shared<search_ticker_page>(this->gui.context, backends.id_to_backend_map[tradier::tag]));

		this->gui.set_root(c);
	}
};

std::unique_ptr<mordavokne::application> mordavokne::create_application(int argc, const char** argv){
	return std::make_unique<::application>();
}