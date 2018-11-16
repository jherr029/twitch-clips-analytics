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

bool ifError( unordered_map<string, string> dataMap );

// TODO: Reduce the amount of api calls
// use cache. refer to the db if necessary
// use more then one id within one api call

int main()
{
    sqlConnector sqlcpp;

    vector<string> slugs = redditClipsHandler();
    unordered_map<string, bool> recentAPIcalls;

    for ( int i = 0; i < slugs.size(); i++ )
    {
        unordered_map<string, string> clipMap = createClipMap( slugs[i] );

        if ( ifError( clipMap ) )
            return 0;

        cout << slugs[i] << endl;

        if ( clipMap.size() == 0 )
        {
            cout << "empty map\n" << endl;
            continue;
        }

        else if (!recentAPIcalls[clipMap["id"]])
        {
            recentAPIcalls[clipMap["id"]] = true;

            unordered_map<string, string> channelMap = createChannelMap( clipMap["id"] );

            if ( ifError( channelMap ) )
                return 0;

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
                // sqlcpp.insertToChannelDataTable( channelMap );
                sqlcpp.addToRecentChannelMap( channelMap["display_name"] );
            }
            else
            {
                cout << "skipped" << endl;
            }

            // sqlcpp.insertToSlugDataTable( clipMap );
            // sqlChannelInsertions( channelMap, sqlcpp );
            // sqlClipInsertions( clipMap, sqlcpp );

        }

        else
        {
            string name = sqlcpp.getNameFromID( clipMap["id"] );
            cout << "from " << name << endl;
            clipMap["name"] = name;
            cout << endl;

            // sqlcpp.insertToSlugDataTable( clipMap );

        }
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
    unordered_map< string, string > clipMap;
    string unParsedClipJson = curlGetJsonTwitchClip( slug );

    if ( unParsedClipJson == "error" )
        return clipMap;     // return empty clipMap
    
    else if ( unParsedClipJson == "too many calls" )
    {
        clipMap["error"] = "calls";
        return clipMap;
    }
    
    // convert to json
    Document clipDoc = createDocument( unParsedClipJson );
    // prettyPrint( clipDoc );

    clipMap = twitchJsonParseClip( clipDoc );
    // TODO: delete duplicate ids is the solutioon

    return clipMap;
}

unordered_map<string, string> createChannelMap( string id )
{
    unordered_map<string, string> channelMap;
    string unParsedChannelJson = curlGetJsonTwitchChannel( id );

    if ( unParsedChannelJson == "error" )
        return channelMap;
    else if ( unParsedChannelJson == "too many calls" )
    {
        channelMap["error"] = "calls";
        return channelMap;
    }

    Document channelDoc = createDocument( unParsedChannelJson );
    // prettyPrint( channelDoc );

    channelMap = twitchJsonParseChannel( channelDoc );

    return channelMap;
}

bool ifError( unordered_map<string, string> dataMap )
{
    if ( dataMap["error"] == "calls" )
        return true;
    
    return false;
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
// TODO: get headers to get limit
// TODO: get bearer authorization
// TODO: understand and implement webhooks


// https://curl.haxx.se/libcurl/c/CURLOPT_HEADERFUNCTION.html
// use postman for the mean time in respects to getting headers