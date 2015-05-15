#include <string>
#include <vector>
#include <sstream>
#include <iostream>

#include "utils.hpp"

std::vector<std::string>& splitString(const std::string &s, char delim, std::vector<std::string>& elems)
{
    std::stringstream ss(s);
    std::string item;
    while(std::getline(ss, item, delim))
    {
        elems.push_back(item);
    }
    return elems;
}

std::string urlChildOf(const std::string& url, const std::string& base)
{
    std::string result;
    size_t found = url.find(base);
    if (found == 0)
    {
        result = url.substr(base.size());
    }
    return result;
}

