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


#include <ruisapp/application.hpp>

#include <utki/debug.hpp>

#include <papki/fs_file.hpp>

#include <httpc/init_guard.hpp>

#include <ruis/widget/group/book.hpp>

#include "backend_register.hpp"

#include "backend/tradier.hpp"

#include "gui/refresh_button.hpp"
#include "gui/cells_container.hpp"
#include "gui/search_ticker_widget.hpp"
#include "gui/line_chart.hpp"

class application : public ruisapp::application{
	httpc::init_guard httpc_guard;
public:
	beerja::backend_register backends;

	application() :
			ruisapp::application(
					"beerja",
					[](){
						return ruisapp::window_params(r4::vector2<unsigned>(1200, 600));
					}()
				)
	{
		this->gui.init_standard_widgets(*this->get_res_file());
		
		this->gui.context.get().loader.mount_res_pack(*this->get_res_file("res/"));

		{
			auto be = std::make_shared<tradier>();
			papki::fs_file cfg_file(utki::cat(this->directory.config, "backend/", be->id(), ".conf"));
			if(cfg_file.exists()){
				be->set_config(tml::read(cfg_file));
			}
			ASSERT(be)
			std::string id(be->id());
			this->backends.register_backend(std::move(id), std::move(be));
		}

		this->gui.context.get().inflater.register_widget<beerja::refresh_button>("u_refresh_button");
		this->gui.context.get().inflater.register_widget<beerja::cells_container>("u_cells_container");
		this->gui.context.get().inflater.register_widget<beerja::line_chart>("u_line_chart");

		auto c = this->gui.context.get().inflater.inflate(
				*this->get_res_file("res/main.gui")
			);

		c.get().get_widget("search_ticker_stub").replace_by(
				utki::make_shared<beerja::search_ticker_widget>(
						this->gui.context,
						backends.id_to_backend_map[tradier::tag]
					)
			);

		this->gui.set_root(c);
	}
};

ruisapp::application_factory app_fac([](auto executbale, auto args){
	return std::make_unique<::application>();
});
