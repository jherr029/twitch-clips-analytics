#include "../includes/lyticsGather.h"

lyticsGather::lyticsGather()
{
    cout << "lyticsGather created" << endl;
}


//TODO: somehow a empty row was inserted into the db
// investigate why that is so.
int lyticsGather::initiateGathering(char ** argv)
{
    cout << "Initiating Gathering" << endl;

    sqlConnector sqlcpp;

    vector<string> slugs = redditClipsHandler();
    unordered_map<string, bool> recentAPIcalls;


    // TODO: C++ within this loop include if code is good, continue
    // if not thread it out for a minute or two till it gets the answer
    // this will be a 429 handler

    for ( int i = 0; i < slugs.size(); i++ )
    {
        // one approach for error code of to do a while this is while error wait x minutes
        // can be done through a thread
        unordered_map<string, string> clipMap = createClipMap( slugs[i] );
        clipMap["clip"] = slugs[i];

        if ( ifError( clipMap ) )
            return 0;

        cout << "Clip: " << slugs[i] << endl;

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
            {
                cout << "returning 1 - fatal error" << endl;
                return 1;
            }

            cout << "Channel: " << channelMap["display_name"] << endl;

            channelMap["id"] = clipMap["id"];
            clipMap["name"] = channelMap["display_name"];

            // will potentially move this to lyticsGather
            // need to find figure out sqlConnector to work in functions
            // TODO:
            // add something similar to this to avoid doing redundant api calls for recent
            // stuff. This will have to be more specific than the sql version
            if ( sqlcpp.ifRecentChannel( channelMap["display_name"] ) == false )
            {
                cout << "Unique Channel (NEW)" << endl << endl;
                if ( strcmp(argv[1], "prod") == 0 )
                {
                    sqlcpp.insertToChannelTable( channelMap );
                    sqlcpp.insertToChannelDataTable( channelMap );
                    sqlcpp.insertToSlugDataTable( clipMap );

                }

                // Cache name // when not using prod this wont work in certain cases
                sqlcpp.addToRecentChannelMap( channelMap["display_name"] );
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
            cout << "Using cache for ID " << clipMap["id"] << " whom belongs to " << name << endl;
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
        cout << "redditClipsHandler error: " << curlObject.getCode() << " ";
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
        cout << "createClipMap error: " << curlObject.getCode() << " ";
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
        cout << "createChannelMap error: " << curlObject.getCode() << " ";
    }

    return channelMap;
}


bool lyticsGather::ifError( unordered_map<string, string> dataMap )
{
    if ( dataMap["error"] == "calls" || dataMap.size() == 0 )
    {
        cout << "there is an error" << endl;
        return true;
    }
    
    return false;
}