#include "time.hpp"

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

	// TODO:

	return ret;
}
