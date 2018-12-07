#include <iostream>

#include <string>
#include <cstdint>


#include "../includes/curl.h"
#include "../includes/jsonParse.h"
#include "../includes/sqlConnector.h"
// #include "tests/test.hpp"

// #include <gtest/gtest.h>

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

// TODO: implement enviromental vars

int main(int argc, char **argv)
{
    cout << "starting" << endl;
    sqlConnector sqlcpp;

    cout << argv[1] << endl;

    vector<string> slugs = redditClipsHandler();
    unordered_map<string, bool> recentAPIcalls;

    for ( int i = 0; i < slugs.size(); i++ )
    {
        unordered_map<string, string> clipMap = createClipMap( slugs[i] );

        if ( ifError( clipMap ) )
            return 0;

        cout << slugs[i] << endl;

        // This occurs because the clip may have been deleted 
        // or the streamer that pertains to the clip has been banned
        if (clipMap.size() == 0)
        {
            cout << "empty map\n" << endl;
            continue;
        }

        // New streamer/channel based on ID
        else if (!recentAPIcalls[clipMap["id"]])
        {
            // Cache result
            recentAPIcalls[clipMap["id"]] = true;

            unordered_map<string, string> channelMap = createChannelMap( clipMap["id"] );

            // may no be needed
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
                if ( strcmp(argv[1], "prod") == 0 )
                {
                    sqlcpp.insertToChannelTable( channelMap );
                    sqlcpp.insertToChannelDataTable( channelMap );
                    sqlcpp.insertToSlugDataTable( clipMap );

                    // Cache name
                    sqlcpp.addToRecentChannelMap( channelMap["display_name"] );
                }
            }
            else
            {
                // sqlcpp.insertToChannelDataTable( channelMap );
                // check if this ever occurs
                cout << "skipped" << endl;
            }

            // the bellow may not be needed anymore
            // sqlcpp.insertToSlugDataTable( clipMap );
            // sqlChannelInserions( channelMap, sqlcpp );
            // sqlClipInsertions( clipMap, sqlcpp );

        }

        // Within this else a new clip of streamer x ( where data pertaining to x was already recieved ) is the
        // only object to be placed into the db
        // This avoids making redundant calls to information that is already has been cached
        else
        {
            string name = sqlcpp.getNameFromID( clipMap["id"] );
            cout << "from " << name << endl;
            clipMap["name"] = name;
            cout << endl;

            if ( strcmp(argv[1], "prod") == 0 )
                sqlcpp.insertToSlugDataTable( clipMap );

        }
    }

    return 0;
}


vector<string> redditClipsHandler()
{
    curl curlObject;

    curlObject.curlReddit();

    vector<string> twitchSlugs;

    if ( curlObject.isCallSuccessful() )
    {
        twitchSlugs = curlObject.parseReddit();
    }

    else
    {
        cout << "error: " << curlObject.getCode();
    }

    cout << twitchSlugs.size() << endl;
    cout << endl;

    return twitchSlugs;
}

unordered_map<string, string> createClipMap( string slug )
{
    unordered_map< string, string > clipMap;

    curl curlObject;
    curlObject.curlTwitchClip( slug );

    if ( curlObject.isCallSuccessful() )
    {
        clipMap = curlObject.parseTwitchClip();

    }

    else
    {
        cout << "error: " << curlObject.getCode();
    }

    return clipMap;
}

unordered_map<string, string> createChannelMap( string id )
{
    unordered_map<string, string> channelMap;

    curl curlObject;

    curlObject.curlTwitchChannel(id);

    if ( curlObject.isCallSuccessful() )
    {
        channelMap = curlObject.parseTwitchChannel();
    }

    else
    {
        cout << "error: " << curlObject.getCode();
    }

    return channelMap;
}

bool ifError( unordered_map<string, string> dataMap )
{
    if ( dataMap["error"] == "calls" )
    {
        cout << "there is an error" << endl;
        return true;
    }
    
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