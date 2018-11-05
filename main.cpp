#include <iostream>
#include <string>
#include <cstdint>


#include "headers/curl.h"
#include "headers/jsonParse.h"

using namespace std;

int main()
{
    
    string unParsedJson = curlGetJsonReddit();
    Document jsonDoc = createDocument( unParsedJson );
    parseDoc( jsonDoc );
    // // prettyPrint( jsonDoc );
    
    // vector<string> twitchSlugs = redditJsonParse( jsonDoc );
    // getSlug( twitchSlugs );
    // vector<string> strVec = getSlug(temp);

    // for ( const auto & element : temp )
    // {
    //     cout << element << endl;
    // }

    
    return 0;
}