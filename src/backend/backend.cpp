#include "backend.hpp"

#include "../date/date.h"

using namespace beerja;

std::chrono::system_clock::time_point backend::get_start_time(
		const std::chrono::system_clock::time_point& end_time,
		granularity gran
	)
{
	switch(gran){
		case granularity::minute:
			return end_time - ::date::days(2);
		case granularity::five_minutes:
			return end_time - ::date::weeks(2);
		case granularity::fivteen_minutes:
			return end_time - ::date::months(1);
		case granularity::day:
			return end_time - ::date::years(10);
	}

	return end_time;
}
