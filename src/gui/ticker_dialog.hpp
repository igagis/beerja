#pragma once

#include <morda/widgets/group/pile.hpp>

class ticker_dialog : public morda::pile{
public:
	ticker_dialog(std::shared_ptr<morda::context> c, const std::string& ticker_symbol_name);

	ticker_dialog(const ticker_dialog&) = delete;
	ticker_dialog& operator=(const ticker_dialog&) = delete;


};
