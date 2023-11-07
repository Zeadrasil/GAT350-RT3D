#pragma once
#include <string>
namespace nc
{
	std::string ToUpper(std::string input);
	std::string ToLower(std::string input);
	bool IsEqualIgnoreCase(std::string first, std::string second);
	extern unsigned int numUniqueStrings;
	std::string createUnique(std::string input);
}

