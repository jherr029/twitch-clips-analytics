#ifndef JSONPARSE_H_
#define JSONPARSE_H_

#include <iostream>
#include "./rapidjson/document.h"
// #include <rapidjson/document.h>
#include <unordered_map>
#include <vector>

using namespace rapidjson;
using namespace std;

class jsonParse
{
    public:
        jsonParse();
        void prettyPrint();

        void createDocument( string );
        vector<string> redditParse();
        unordered_map<string, string> twitchClipParse();
        unordered_map<string, string> twitchChannelParse();
    
    protected:
        Document jsonDoc;

    
    private:
        string getSlug( string & );
        vector<string> parseTimeDate( string & );

};

// void parseDoc( Document & );
// void prettyPrint( Document & );

// vector<string> redditJsonParse( Document & );
// string getSlug( string );

// vector<string> parseTimeDate(string );

// unordered_map< string, string > twitchJsonParseClip( Document & );
// unordered_map< string, string > twitchJsonParseChannel( Document & );



#endif