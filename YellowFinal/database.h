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
	void Print(ostream& output) const;
	template <typename Predicate>
	int RemoveIf(Predicate predicate)
	{
		
		int result = 0;
		for (auto itMap = database.begin(); itMap != database.end();)
		{
			Date key = (*itMap).first;


			auto newEnd = remove_if((*itMap).second.begin(), (*itMap).second.end(),
				[&, predicate, key](const string& event) {
					if (predicate(key, event))
					{
						setChecker[key].erase(event);
						if (setChecker[key].empty()) setChecker.erase(key);
					}
					return predicate(key, event);
				});
			
			result += (*itMap).second.end() - newEnd;


			// clean vector
			(*itMap).second.erase(newEnd, (*itMap).second.end());
			
			// check whether the vector is empty so that we can remove Data entry in Database Map and Set for Dates
			if ((*itMap).second.empty())
			{
				dates.erase(key);
				++itMap;
				database.erase(prev(itMap));
			}
			else ++itMap;
		}
		
		return result;
	}
	

	template <typename Predicate>
	vector<pair<Date, string>> FindIf(Predicate predicate) const
	{
		vector<pair<Date, string>> result;
		for (auto itMap = database.begin(); itMap != database.end(); itMap++)
		{
			Date key = (*itMap).first;

			auto it = (*itMap).second.begin();
			while (it != (*itMap).second.end())
			{
				it = find_if(it, (*itMap).second.end(), [key, predicate](const string& event)
												{ return predicate(key, event);});
				if (it != (*itMap).second.end())
				{
					result.push_back(make_pair(key, *it));
					++it;
				}
			}
		}
		return result;
	}

	pair<Date, string> Last(const Date& date) const;
	string DebugPrint();
private:
	map<Date, vector<string>> database;
	map<Date, set<string>> setChecker;
	set<Date> dates;
};


std::ostream& operator<< (std::ostream& output, const std::pair<Date, std::string>& pair);
bool operator!= (const std::pair<Date, std::string>& lhs, const std::pair<Date, std::string>& rhs);