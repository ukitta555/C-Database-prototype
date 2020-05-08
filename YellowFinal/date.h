#pragma once

#include <iostream>
#include <iomanip>


class Date
{
public:
	Date(int year, int month, int day);
	int getYear() const;
	int getMonth() const;
	int getDay() const;
private:
	int year;
	int month;
	int day;
};

std::ostream& operator<< (std::ostream& output, const Date& date);
bool operator< (const Date& lhs, const Date& rhs);
Date ParseDate(std::istream& is);