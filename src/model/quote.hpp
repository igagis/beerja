#pragma once

namespace beerja{

struct quote{
	float last; /// last price in currency
	float change; /// change from day opening in currency
	float change_percent; /// change from day opening in percentage
	uint64_t volume; /// trade volume in pieces (number of shares)
	float open; /// day open price in currency
	float close; /// day close price in currency (makes no sense for current day)
	float high; /// highest price of day in currency
	float low; /// lowest price of day in currency
};

}
