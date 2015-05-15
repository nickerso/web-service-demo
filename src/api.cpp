#include <iostream>
#include <sstream>

#include <map>
#include <math.h>

#include <string>

#include "api.hpp"
#include "utils.hpp"

API::API()
{
    std::cout << "Creating new API for use by the web services demo" << std::endl;
}

API::~API()
{
    std::cout << "Destroying an API" << std::endl;
}

std::string API::executeAPI(const std::string& url, const std::map<std::string, std::string>& argvals)
{
    std::string response;
    std::cout << "executeAPI for URL: \"" << url.c_str() << "\"" << std::endl;

    std::string child = urlChildOf(url, "/sine/");
    if (child.size() > 0)
    {
        double scale = std::stod(child);
        std::stringstream xs, ys;
        xs << "\"x\": [";
        ys << "\"y\": [";
        for (int i=0; i < 100; ++i)
        {
            if (i > 0)
            {
                xs << ", ";
                ys << ", ";
            }
            double x = i * 0.2;
            double sinX = sin(scale*x);
            xs << x;
            ys << sinX;
        }
        xs << "]";
        ys << "]";
        response = "{";
        response += xs.str();
        response += ", ";
        response += ys.str();
        response += "}";
        return response;
    }

    // unhandled API method called
    std::cerr << "Unhandled API method for GMS: " << url.c_str() << std::endl;
    getInvalidResponse(response);
    return response;
}

void API::getInvalidResponse(std::string& response)
{
    response = "Some error in your data ";
}

