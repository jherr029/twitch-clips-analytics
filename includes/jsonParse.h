#ifndef JSONPARSE_H_
#define JSONPARSE_H_

#include <iostream>
#include "./rapidjson/document.h"
// #include <rapidjson/document.h>
#include <unordered_map>
#include <vector>

using namespace rapidjson;
using namespace std;

Document createDocument( string & );
void parseDoc( Document & );
void prettyPrint( Document & );

vector<string> redditJsonParse( Document & );
string getSlug( string );

vector<string> parseTimeDate(string );

unordered_map< string, string > twitchJsonParseClip( Document & );
unordered_map< string, string > twitchJsonParseChannel( Document & );



#endif