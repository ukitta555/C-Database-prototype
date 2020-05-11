#include "database.h"

void Database::Add(const Date& date, const std::string& event) 
{
	std::vector<std::string> eventsForDate = database[date];
	if (find(eventsForDate.begin(), eventsForDate.end(), event) == eventsForDate.end())	database[date].push_back(event);
}

void Database::Print(std::ostream& output) 
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

