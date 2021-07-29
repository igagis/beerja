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
