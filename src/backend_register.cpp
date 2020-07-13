#include "backend_register.hpp"

using namespace beerja;

void backend_register::register_backend(std::string&& id, std::shared_ptr<beerja::backend> be){
	if(!be){
		throw std::logic_error("backend_register::register_backend(): passed in backend object is nullptr");
	}
	this->id_to_backend_map[id] = std::move(be);
}
