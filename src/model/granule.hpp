#pragma once

#include "../util/date.hpp"

namespace beerja{

struct granule{
	date::sys_seconds timestamp;
	uint64_t volume; /// trade volume in pieces (number of shares)
	float open; /// day open price in currency
	float close; /// day close price in currency (makes no sense for current day)
	float high; /// highest price of day in currency
	float low; /// lowest price of day in currency
	float price; /// volume weighted average price
};

}
