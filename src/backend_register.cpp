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

#include "backend_register.hpp"

using namespace beerja;

void backend_register::register_backend(std::string&& id, std::shared_ptr<beerja::backend> be){
	if(!be){
		throw std::logic_error("backend_register::register_backend(): passed in backend object is nullptr");
	}
	this->id_to_backend_map[id] = std::move(be);
}
