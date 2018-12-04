#include <iostream>

#include <string>
#include <cstdint>


#include "../includes/lyticsGather.h"
// #include "tests/test.hpp"

// #include <gtest/gtest.h>

using namespace std;

int main(int argc, char **argv)
{
    lyticsGather gatherObject;
    int num = gatherObject.initiateGathering(argv);
    
    return 0;
}