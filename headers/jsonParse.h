#ifndef JSONPARSE_H_
#define JSONPARSE_H_

#include <iostream>
#include <rapidjson/document.h>
#include <unordered_map>
#include <vector>

using namespace rapidjson;
using namespace std;

Document createDocument( string & );
void parseDoc( Document & );
void prettyPrint( Document & );

vector<string> redditJsonParse( Document & );
string getSlug( string );

string twitchJsonParseID( Document & );
unordered_map< string, string > twitchJsonParseChannel( Document & );



#endif