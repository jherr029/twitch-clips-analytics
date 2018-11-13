#include <iostream>

#include <string>
#include <cstdint>


#include "../headers/curl.h"
#include "../headers/jsonParse.h"
#include "../headers/sqlConnector.h"

using namespace std;

vector<string> redditClipsHandler();
unordered_map<string, string> createClipMap( string slug );
unordered_map<string, string> createChannelMap( string id );
void sqlChannelInsertions( unordered_map<string, string> dataMap, sqlConnector sqlcpp );
void sqlClipInsertions( unordered_map<string, string> dataMap, sqlConnector sqlcpp );

int main()
{
    sqlConnector sqlcpp;

    vector<string> slugs = redditClipsHandler();

    for ( int i = 0; i < slugs.size(); i++ )
    {
        unordered_map<string, string> clipMap = createClipMap( slugs[i] );
        cout << slugs[i] << endl;

        if ( clipMap.size() == 0 )
        {
            cout << "empty map" << endl;
            continue;
        }

        unordered_map<string, string> channelMap = createChannelMap( clipMap["id"] );
        cout << channelMap["display_name"] << endl;


        channelMap["id"] = clipMap["id"];
        clipMap["name"] = channelMap["display_name"];

        cout << endl;

        // need to find figure out sqlConnector to work in functions

        
        // TODO:
        // add something similar to this to avoid doing redundant api calls for recent
        // stuff. This will have to be more specific than the sql version
        if ( sqlcpp.ifRecentChannel( channelMap["display_name"] ) == 0 )
        {
            sqlcpp.insertToChannelTable( channelMap );
            sqlcpp.insertToChannelDataTable( channelMap );
            sqlcpp.addToRecentChannelMap( channelMap["display_name"] );
        }

        sqlcpp.insertToSlugDataTable( clipMap );
        // sqlChannelInsertions( channelMap, sqlcpp );
        // sqlClipInsertions( clipMap, sqlcpp );

    }

    return 0;
}


vector<string> redditClipsHandler()
{
    string unParsedPageJson = curlGetJsonReddit();
    Document pageDoc = createDocument( unParsedPageJson );
    cout << endl;

    vector<string> twitchSlugs = redditJsonParse( pageDoc );
    cout << endl;

    return twitchSlugs;
}

unordered_map<string, string> createClipMap( string slug )
{

    string unParsedClipJson = curlGetJsonTwitchClip( slug );

    // convert to json
    Document clipDoc = createDocument( unParsedClipJson );
    // prettyPrint( clipDoc );

    unordered_map< string, string > clipMap = twitchJsonParseClip( clipDoc );

    return clipMap;
}

unordered_map<string, string> createChannelMap( string id )
{
    string unParsedChannelJson = curlGetJsonTwitchChannel( id );

    Document channelDoc = createDocument( unParsedChannelJson );
    // prettyPrint( channelDoc );

    unordered_map<string, string> channelMap = twitchJsonParseChannel( channelDoc );

    return channelMap;
}

void sqlChannelInsertions( unordered_map<string, string> channelMap, sqlConnector sqlcpp )
{
    sqlcpp.insertToChannelTable( channelMap );
    sqlcpp.insertToChannelDataTable( channelMap );
    
}

void sqlClipInsertions( unordered_map<string, string> clipMap, sqlConnector sqlcpp )
{
    sqlcpp.insertToSlugDataTable( clipMap );
}


// TODO: be consistent with naming convention -> slug or clip. pick one