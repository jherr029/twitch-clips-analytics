#include <iostream>

#include <string>
#include <cstdint>


#include "../headers/curl.h"
#include "../headers/jsonParse.h"
#include "../headers/sqlConnector.h"

using namespace std;

int main()
{
    sqlConnector sqlcpp;

    // sqlcpp.sampleCreate();
    
    string unParsedPageJson = curlGetJsonReddit();
    Document pageDoc = createDocument( unParsedPageJson );
    cout << endl;

    vector<string> twitchSlugs = redditJsonParse( pageDoc );
    cout << endl;

    for ( int i = 0; i < twitchSlugs.size(); i++ )
    {

        if (twitchSlugs[i] == "banned" )
        {
            cout << "this is a banned or missing account " << endl;
            continue;
        }

        string unParsedClipJson = curlGetJsonTwitchClip( twitchSlugs[i] );

        // convert to json
        Document clipDoc = createDocument( unParsedClipJson );
        cout << twitchSlugs[i] << endl;
        // prettyPrint( clipDoc );

        unordered_map< string, string > clipMap = twitchJsonParseClip( clipDoc );

        if ( clipMap.size() == 0 )
        {
            cout << "empty map" << endl;
            continue;
        }

        string unParsedChannelJson = curlGetJsonTwitchChannel( clipMap["id"] );

        Document channelDoc = createDocument( unParsedChannelJson );
        cout << channelDoc["display_name"].GetString() << endl;
        // prettyPrint( channelDoc );
        

        unordered_map<string, string> channelMap = twitchJsonParseChannel( channelDoc );
        channelMap["id"] = clipMap["id"];

        cout << endl;

        sqlcpp.insertToChannelTable(channelMap);
        sqlcpp.insertToChannelDataTable(channelMap);

    }

    return 0;
}