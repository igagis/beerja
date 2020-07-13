#pragma once

#include <map>
#include <string>

#include "backend/backend.hpp"

namespace beerja{

class backend_register{
public:
	std::map<std::string, std::shared_ptr<beerja::backend>> id_to_backend_map;

	void register_backend(std::string&& id, std::shared_ptr<beerja::backend> be);
};

}
