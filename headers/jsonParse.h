#ifndef JSONPARSE_H_
#define JSONPARSE_H_

#include <iostream>
#include <rapidjson/document.h>
#include <vector>

using namespace rapidjson;
using namespace std;

Document createDocument( string & );
void parseDoc( Document & );
void prettyPrint( Document & );

vector<string> redditJsonParse( Document & );
string getSlug( string );

vector<vector<string> > twitchJsonParse( vector<string> );



#endif