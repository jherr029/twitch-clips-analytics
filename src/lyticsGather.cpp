#include "../includes/lyticsGather.h"

lyticsGather::lyticsGather()
{
    cout << "lyticsGather created" << endl;
}


int lyticsGather::initiateGathering(char ** argv)
{
    cout << "Initiating Gathering" << endl;

    sqlConnector sqlcpp;

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
        else if ( !recentClipsRequest[ clipMap["id"] ] )
        {
            // Cache result
            recentClipsRequest[ clipMap["id"] ] = true;

            unordered_map<string, string> channelMap = createChannelMap( clipMap["id"] );

            // may no be needed
            if ( ifError( channelMap ) )
                return 0;

            cout << channelMap["display_name"] << endl;

            channelMap["id"] = clipMap["id"];
            clipMap["name"] = channelMap["display_name"];
            cout << endl;

            // will potentially move this to lyticsGather
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

}

vector<string> lyticsGather::redditClipsHandler()
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

unordered_map<string, string> lyticsGather::createClipMap( string slug )
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

unordered_map<string, string> lyticsGather::createChannelMap( string id )
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


bool lyticsGather::ifError( unordered_map<string, string> dataMap )
{
    if ( dataMap["error"] == "calls" )
    {
        cout << "there is an error" << endl;
        return true;
    }
    
    return false;
}