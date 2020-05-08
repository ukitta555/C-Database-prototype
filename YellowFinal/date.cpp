#include "date.h"

Date ParseDate(std::istream& is)
{
    int year, month, day;
    is >> year;
    is.ignore(1);
    is >> month;
    is.ignore(1);
    is >> day;
    return { year, month, day };
};


Date::Date(int year, int month, int day) : year(year), month(month), day(day){};

int Date::getYear() const { return year; }
int Date::getMonth() const { return month; }
int Date::getDay() const { return day; }


std::ostream& operator << (std::ostream& output, const Date& date)
{
	output << std::setfill('0') << std::setw(4) << date.getYear() << "-" 
		   << std::setfill('0') << std::setw(2) << date.getMonth() << "-" 
		   << std::setfill('0') << std::setw(2) << date.getDay();
	return output;
};




