#pragma once

#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <algorithm>
#include <vector>
#include "date.h"

using namespace std;

class Database
{
public:
	void Add(const Date& date, const std::string& event);
	void Print(std::ostream& output);
	
	template <typename Predicate>
	int RemoveIf(Predicate predicate)
	{
		
		int result = 0;
		for (auto itMap = database.begin(); itMap != database.end();)
		{
			
			Date key = (*itMap).first;
			auto newEnd = remove_if((*itMap).second.begin(), (*itMap).second.end(),
				[predicate, key](const string& event) {
					return predicate(key, event);
				});
			result += (*itMap).second.end() - newEnd;
			(*itMap).second.erase(newEnd, (*itMap).second.end());
			if ((*itMap).second.empty())
			{
				itMap++;
				database.erase(prev(itMap));
			}
			else itMap++;
		}
		
		return result;
	}
	

	std::string DebugPrint();
private:
	std::map<Date, std::vector<std::string>> database;
};