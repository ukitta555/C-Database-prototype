#include "parse_event.h"

std::string ParseEvent(std::istream& is)
{
    // implement this function
    std::string event;
    is.ignore(1);
    std::getline(is, event);
    return event;
}