#pragma once

#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <algorithm>
#include <vector>
#include "date.h"

class Database
{
public:
	void Add(const Date& date, const std::string& event);
	void Print();
	std::string DebugPrint();
private:
	std::map<Date, std::vector<std::string>> database;
};