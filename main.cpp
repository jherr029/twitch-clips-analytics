#include <iostream>

#include <string>
#include <cstdint>


#include "headers/curl.h"
#include "headers/jsonParse.h"
#include "headers/sqlConnector.h"

using namespace std;

int main()
{
    connectToDB();
    
    // string unParsedPageJson = curlGetJsonReddit();
    // Document pageDoc = createDocument( unParsedPageJson );
    // cout << endl;

    // vector<string> twitchSlugs = redditJsonParse( pageDoc );
    // cout << endl;

    // for ( int i = 0; i < twitchSlugs.size(); i++ )
    // {

    //     // cout << i + 1 << " - " << twitchSlugs[i] << endl;
    //     if (twitchSlugs[i] == "banned" )
    //     {
    //         cout << "this is a banned or missing account " << endl;
    //         continue;
    //     }

    //     string unParsedClipJson = curlGetJsonTwitchClip( twitchSlugs[i] );


    //     // convert to json
    //     Document clipDoc = createDocument( unParsedClipJson );
    //     // prettyPrint( clipDoc );

    //     // get id
    //     unordered_map< string, string > clipMap = twitchJsonParseClip( clipDoc );
    //     // cout << channelID  << " " << twitchSlugs[i] << endl;
    //     string unParsedChannelJson = curlGetJsonTwitchChannel( clipMap["id"] );

    //     Document channelDoc = createDocument( unParsedChannelJson );
    //     // prettyPrint( channelDoc );

    //     cout << "parse channel" << endl;
    //     unordered_map<string, string> channelMap = twitchJsonParseChannel( channelDoc );
    //     channelMap["id"] = clipMap["id"];

    // }

    return 0;
}