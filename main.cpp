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
    cout << endl;

    // parseDoc( jsonDoc );

    // prettyPrint( jsonDoc );
    
    vector<string> twitchSlugs = redditJsonParse( jsonDoc );
    cout << endl;

    for ( int i = 0; i < twitchSlugs.size(); i++ )
    {
        string unParsedJson2 = curlGetJsonTwitch( twitchSlugs[i] );
        Document clipJson = createDocument( unParsedJson2 );
        prettyPrint( clipJson );

    }

    return 0;
}