#include <microhttpd.h>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <string>
#include <iostream>
#include <sstream>
#include <map>

#include "libhttpd-utils.hpp"

static void printVersion()
{
    std::cout << "This is Andre's web service demo version: 0.0.1" << std::endl;
}

int main(int argc, char ** argv)
{
    printVersion();
    if (argc < 2)
    {
        printf("%s <PORT>\n", argv[0]);
        return 1;
    }
    startServer(atoi(argv[1]));
    return 0;
}
