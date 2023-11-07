#include "StringUtils.h"

std::string nc::ToUpper(std::string input)
{
    std::string result = "";
    for (char c : input)
    {
        result.append(1, char(toupper(c)));
    }
    return result;
}

std::string nc::ToLower(std::string input)
{
    std::string result = "";
    for (char c : input)
    {
        result.append(1, char(tolower(c)));
    }
    return result;
}

bool nc::IsEqualIgnoreCase(std::string first, std::string second)
{
    if (first.length() != second.length())
    {
        return false;
    }
    std::string firstLower = ToLower(first), secondLower = ToLower(second);
    return firstLower.compare(secondLower) == 0;
}

unsigned int nc::numUniqueStrings = 0;

std::string nc::createUnique(std::string input)
{
    std::string result = input.append(std::to_string(numUniqueStrings));
    numUniqueStrings++;
    return result;
}
