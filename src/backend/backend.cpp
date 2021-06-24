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
