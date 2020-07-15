#pragma once

#include "granule.hpp"

namespace beerja{

struct quote : public granule{
	float last; /// last price in currency
	float change; /// change from day opening in currency
	float change_percent; /// change from day opening in percentage
};

}
