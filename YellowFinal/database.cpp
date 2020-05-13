#include "database.h"

void Database::Add(const Date& date, const std::string& event) 
{
	if (setChecker.count(date) == 0 || setChecker.at(date).count(event) == 0)
	{
		setChecker[date].insert(event);
		database[date].push_back(event);
		dates.insert(date);
	}
}

void Database::Print(std::ostream& output) const
{
	for (const auto& [key, val] : database)
	{
		for (const auto& event : val)
		{
			output << key << ' ' << event << std::endl;
		}
	}
}

std::string Database::DebugPrint()
{
	std::stringstream ss;
	std::string result;
	for (const auto& [key, val] : database)
	{
		for (const auto& event : val)
		{
			std::string tmp;
			ss << key;
			ss << " " + event + "\n";
			getline(ss, tmp);
			result += tmp;
		}
	}
	return result;
}

std::pair<Date, std::string> Database::Last(const Date& date) const
{	
	if (dates.begin() == dates.end()) throw invalid_argument("no entries in DB!");
	if (date < *(dates.begin())) throw invalid_argument("wrong date!");
	
	auto it = dates.lower_bound(date);
	if (it == dates.end() || date != (*it)) --it;
	pair<Date, string> result = make_pair((*it), database.at((*it)).back());
	return result;
}



std::ostream& operator<< (std::ostream& output, const std::pair<Date, std::string>& pair)
{
	output << pair.first << ' ' << pair.second;
	return output;
}

bool operator!= (const std::pair<Date, std::string>& lhs, const std::pair<Date, std::string>& rhs)
{
	return !(lhs.first == rhs.first && lhs.second == rhs.second);
}