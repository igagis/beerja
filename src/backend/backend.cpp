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

#include "backend.hpp"

#include "../util/date.hpp"

using namespace beerja;

std::chrono::system_clock::time_point backend::get_start_time(
		const std::chrono::system_clock::time_point& end_time,
		granularity gran
	)
{
	std::chrono::system_clock::time_point ret;

	switch(gran){
		case granularity::minute:
			ret = end_time - ::date::days(2);
			break;
		case granularity::five_minutes:
			ret = end_time - ::date::weeks(2);
			break;
		case granularity::fivteen_minutes:
			ret = end_time - ::date::months(1);
			break;
		case granularity::day:
			ret = end_time - ::date::years(10);
			break;
	}

	using ::date::floor;

	return floor<std::chrono::seconds>(ret);
}
