
#include <mordavokne/application.hpp>

#include <utki/debug.hpp>

#include <papki/fs_file.hpp>

#include <httpc/init_guard.hpp>

#include <morda/widgets/group/book.hpp>

#include "backend_register.hpp"

#include "backend/tradier.hpp"

#include "gui/refresh_button.hpp"
#include "gui/cells_container.hpp"
#include "gui/search_ticker_widget.hpp"
#include "gui/line_chart.hpp"

class application : public mordavokne::application{
	httpc::init_guard httpc_guard;
public:
	beerja::backend_register backends;

	application() :
			mordavokne::application(
					"beerja",
					[](){
						return mordavokne::window_params(r4::vector2<unsigned>(1200, 600));
					}()
				)
	{
		this->gui.initStandardWidgets(*this->get_res_file());
		
		this->gui.context->loader.mount_res_pack(*this->get_res_file("res/"));

		{
			auto be = std::make_shared<tradier>();
			papki::fs_file cfg_file(this->storage_dir + "backend/" + be->id() + ".conf");
			if(cfg_file.exists()){
				be->set_config(treeml::read(cfg_file));
			}
			ASSERT(be)
			std::string id(be->id());
			this->backends.register_backend(std::move(id), std::move(be));
		}

		this->gui.context->inflater.register_widget<beerja::refresh_button>("u_refresh_button");
		this->gui.context->inflater.register_widget<beerja::cells_container>("u_cells_container");
		this->gui.context->inflater.register_widget<beerja::line_chart>("u_line_chart");

		auto c = this->gui.context->inflater.inflate(
				*this->get_res_file("res/main.gui")
			);
		ASSERT(c)

		c->get_widget("search_ticker_stub").replace_by(
				std::make_shared<beerja::search_ticker_widget>(
						this->gui.context,
						backends.id_to_backend_map[tradier::tag]
					)
			);

		this->gui.set_root(c);
	}
};

mordavokne::application_factory app_fac([](auto args){
	return std::make_unique<::application>();
});
