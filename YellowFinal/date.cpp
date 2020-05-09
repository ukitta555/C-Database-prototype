#include "date.h"

Date ParseDate(std::istream& is)
{
    int year = 0 , month = 0, day = 0;
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

bool operator<(const Date& lhs, const Date& rhs)
{
	if (lhs.getYear() < rhs.getYear()) return true;
	if (lhs.getYear() > rhs.getYear()) return false;
	if (lhs.getYear() == rhs.getYear())
	{
		if (lhs.getMonth() < rhs.getMonth()) return true;
		if (lhs.getMonth() > rhs.getMonth()) return false;
		if (lhs.getMonth() == rhs.getMonth())
		{
			if (lhs.getDay() < rhs.getDay()) return true;
			if (lhs.getDay() >= rhs.getDay()) return false;
		}
	}
	return false;
}


