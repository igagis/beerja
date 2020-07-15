#pragma once

#include <string>
#include <stdexcept>

namespace beerja{

class date{
	int year;
	uint8_t month;
	uint8_t day;

public:
	bool is_leap_year()const;

	int get_year()const{
		return this->year;
	}

	void set_year(int year){
		this->year = year;
	}

	unsigned get_month()const{
		return this->month;
	}

	void set_month(unsigned month){
		if(month >= 12){
			throw std::logic_error("date::set_month(): passed in month value is out of range, must be from 0 to 11");
		}
		// TODO: check if currently set day is valid for the given month/year
		this->month = decltype(this->month)(month);
	}

	/**
	 * @brief Get day of month.
	 * @return day of month.
	 */
	unsigned get_day()const{
		return this->day;
	}

	void set_day(unsigned day){
		// TODO: check day for validity for given year/month
		this->day = decltype(this->day)(day);
	}
};

class datetime : public date{
	unsigned hour;
	unsigned minute;
	unsigned second;

public:
	static datetime parse(const std::string& str);
};

}
