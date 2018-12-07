#include <iostream>

#include <string>
#include <cstdint>
#include <chrono>
#include <thread>


#include "../includes/lyticsGather.h"
#include "../includes/analyzer.h"
// #include "tests/test.hpp"

// #include <gtest/gtest.h>

using namespace std;

int main(int argc, char **argv)
{
    while (true)
    {
        lyticsGather gatherObject;
        int num = gatherObject.initiateGathering(argv);
        this_thread::sleep_for(chrono::minutes(20));
    }

    analyzer analyzerObj;
    
    return 0;
}