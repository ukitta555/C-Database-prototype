#include "database.h"

void Database::Add(const Date& date, const std::string& event) 
{
	database[date].push_back(event);
}

void Database::DebugPrint() 
{
	for (const auto& [key, val] : database)
	{
		std::cout << key << std::endl;
	}
}