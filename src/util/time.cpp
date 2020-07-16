#include "time.hpp"

#include <sstream>

#include <utki/string.hpp>

using namespace beerja;

bool date::is_leap_year()const{
	if(this->year % 400 == 0){
		return true;
	}

	if(this->year % 100 == 0){
		return false;
	}

	return this->year % 4 == 0;
}

datetime datetime::parse(const std::string& str){
	datetime ret;

	auto p = str.c_str();
	auto e = p + str.size();

	auto check_out_of_range = [&p, &e](){
		if(p >= e){
			throw std::invalid_argument("datetime::parse() malformed ISO date-time string");
		}
	};

	char* pp;

	ret.set_year(std::strtoul(p, &pp, 10));
	p = pp + 1;
	check_out_of_range();
	ret.set_month(std::strtoul(p, &pp, 10));
	p = pp + 1;
	check_out_of_range();
	ret.set_day(std::strtoul(p, &pp, 10));
	p = pp + 1;
	check_out_of_range();
	ret.set_hour(std::strtoul(p, &pp, 10));
	p = pp + 1;
	check_out_of_range();
	ret.set_minute(std::strtoul(p, &pp, 10));
	p = pp + 1;
	check_out_of_range();
	ret.set_second(std::strtoul(p, &pp, 10));

	return ret;
}

void date::set_month(unsigned month){
	if(month == 0 || month > 12){
		throw std::logic_error("date::set_month(): passed in month value is out of range, must be from 1 to 12");
	}
	// TODO: check if currently set day is valid for the given month/year
	this->month = decltype(this->month)(month);
}

namespace{
std::string two_digits_to_string(unsigned n){
	std::array<char, 2> buf;

	snprintf(buf.data(), buf.size(), "%.2u", n);

	return utki::make_string(buf);
}
}

std::string datetime::to_string()const{
	std::stringstream ss;

	ss << std::to_string(this->get_year());
	ss << "-" << two_digits_to_string(this->get_month());
	ss << "-" << two_digits_to_string(this->get_day());
	ss << " " << two_digits_to_string(this->get_hour());
	ss << ":" << two_digits_to_string(this->get_minute());
	ss << ":" << two_digits_to_string(this->get_second());

	return ss.str();
}
