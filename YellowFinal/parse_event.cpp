#include "parse_event.h"

std::string ParseEvent(std::istream& is)
{
    // implement this function
    while (is.peek() == ' ') is.ignore(1);
    std::string event;
    std::getline(is, event);
    return event;
}