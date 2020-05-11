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
				dates.erase((*itMap).first);
				++itMap;
				database.erase(prev(itMap));
			}
			else ++itMap;
		}
		
		return result;
	}
	

	template <typename Predicate>
	std::vector<std::pair<Date, std::string>> FindIf(Predicate predicate)
	{
		vector<pair<Date, string>> result;
		for (const auto& [key, val] : database)
		{
			auto it = val.begin();
			while (it != val.end())
			{
				it = find_if(it, val.end(), [predicate, key](const string& event)
												{ return predicate(key, event);});
				if (it != val.end())
				{
					result.push_back(make_pair(key, *it));
					it++;
				}
			}
		}
		return result;
	}

	std::pair<Date, std::string> Last(const Date& date);
	std::string DebugPrint();
private:
	std::map<Date, vector<string>> database;
	std::map<Date, set<string>> setChecker;
	set<Date> dates;
};


std::ostream& operator<< (std::ostream& output, const std::pair<Date, std::string>& pair);
bool operator!= (const std::pair<Date, std::string>& lhs, const std::pair<Date, std::string>& rhs);