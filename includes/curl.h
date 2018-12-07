#ifndef CURL_H_
#define CURL_H_

#include "jsonParse.h"
#include "errorFlag.h"

#include <iostream>
#include <stdlib.h>
#include <vector>
#include <unordered_map>

using namespace std;

class curl : public jsonParse
{
    public:
        curl();

        void curlReddit();
        void curlTwitchClip( string );
        void curlTwitchChannel( string );

        vector<string> parseReddit();
        unordered_map<string, string> parseTwitchClip();
        unordered_map<string, string> parseTwitchChannel();


        int getCode();
        string getCurlResult();
        bool isCallSuccessful();

        bool isError();
    
    private:
        errorFlag errorObj;

        int httpResult;
        string stringResult;


};

// std::string curlGetJsonReddit();

// std::string curlGetJsonTwitchClip( std::string );
// std::string curlGetJsonTwitchChannel( std::string );
// std::string requestGetJson();

#endif