#pragma once

#include <string>
#include <sstream>
#include <vector>

namespace Astra
{
	class StringUtils
	{
	public:
		static const std::vector<std::string> Split(std::string string, char delim = ' ', bool rep = true) 
		{
            std::vector<std::string> fields;
            std::string temp;
            std::stringstream check1(string);

            while (std::getline(check1, temp, delim))
            {
                fields.push_back(temp);
            }
            return fields;
		}

        static constexpr unsigned int str2int(const char* const str, int h = 0)
        {
            return !str[h] ? 5381 : (str2int(str, h + 1) * 33) ^ str[h];
        }
	};
}