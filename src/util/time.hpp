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

	void set_month(unsigned month);

	/**
	 * @brief Get day of month.
	 * Zero is an invalid value.  Valid values are from 1 to 28/29/30/31, depending on month.
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
	uint8_t hour;
	uint8_t minute;
	uint8_t second;

public:

	unsigned get_hour()const{
		return this->hour;
	}

	void set_hour(unsigned hour){
		// TODO: check validity
		this->hour = hour;
	}

	unsigned get_minute()const{
		return this->minute;
	}

	void set_minute(unsigned minute){
		// TODO: check validity
		this->minute = minute;
	}

	unsigned get_second()const{
		return this->second;
	}

	void set_second(unsigned second){
		// TODO: check validity
		this->second = second;
	}

	std::string to_string()const;

	static datetime parse(const std::string& str);
};

}
