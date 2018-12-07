#ifndef LYTICSGATHER_H_
#define LYTICSGATHER_H_

#include <iostream>
#include <cstdint>

#include "curl.h"
// #include "jsonParse.h"
#include "sqlConnector.h"

using namespace std;

class lyticsGather 
{
    public:
        lyticsGather();

        int initiateGathering(char **);


    private:
        unordered_map<string, bool> recentClipsRequest;
        // Maybe create those class object here
        // curl curlObject;

        vector<string> redditClipsHandler();
        unordered_map<string, string> createClipMap( string );
        unordered_map<string, string> createChannelMap( string );

        bool ifError( unordered_map<string, string> );

};

#endif