#include "headers/jsonParse.h"

#include <rapidjson/stringbuffer.h>
#include <rapidjson/prettywriter.h>
#include <string>
#include <array>

Document createDocument( string & unparsedStr )
{
    Document jsonDocument;
    jsonDocument.Parse( unparsedStr.c_str() );

    return jsonDocument;
}


void parseDoc( Document & jsonDocument )
{
    cout << jsonDocument["data"]["children"][5]["data"]["title"].GetString() << endl;
}

void prettyPrint( Document & jsonDocument )
{
    StringBuffer buffer;
    PrettyWriter<StringBuffer> writer(buffer);
    jsonDocument.Accept(writer);

    cout << buffer.GetString() << endl << endl;
}

vector<string> redditJsonParse( Document & jsonDocument )
{
    vector<string> jsonVec;
    int entries = jsonDocument["data"]["dist"].GetInt();
    // cout << "Number of entries : " << entries << endl; 

    Value & temp = jsonDocument["data"]["children"];
    string url;

    for ( int i = 0; i < entries; i++ )
    {
        url = temp[i]["data"]["url"].GetString();
        // cout << slug << endl;
        if ( url.find( "twitch.tv" ) != string::npos )
            jsonVec.push_back( getSlug( url ) );
    }

    // cout << "added stuff to the vector: " << jsonVec.size() << endl;

    return jsonVec;
}

string getSlug( string url )
{
    // a not banned user
    if ( url.find("clip") )
        return url.substr( 24 );

    else
        return "banned";

}

unordered_map< string, string > twitchJsonParseClip( Document & jsonDoc )
{
    unordered_map< string, string > tempMap;


    prettyPrint(jsonDoc);
    tempMap["id"] = jsonDoc["data"][0]["broadcaster_id"].GetString();
    tempMap["name"] = jsonDoc["data"][0]["title"].GetString();
    tempMap["viewers"] = to_string(jsonDoc["data"][0]["view_count"].GetInt());
    tempMap["game_id"] = jsonDoc["data"][0]["game_id"].GetString();

    // maybe here do another api call to get the name of the game
    // assert(jsonDoc["data"][0]["broadcaster_id"].IsString() );
    // cout << jsonDoc["data"][0]["broadcaster_id"].GetString() << endl;

    return tempMap;
}

unordered_map< string, string > twitchJsonParseChannel( Document & jsonDoc )
{
    unordered_map<string, string> temp;

    string name, game, broadcasterType;
    int followersInt, viewsInt;

    // prettyPrint(jsonDoc);

    name = jsonDoc["display_name"].GetString();
    followersInt = jsonDoc["followers"].GetInt();
    viewsInt = jsonDoc["views"].GetInt();
    game = jsonDoc["game"].GetString();
    broadcasterType = jsonDoc["broadcaster_type"].GetString();

    temp["display_name"] = name;
    temp["followers"] = to_string( followersInt );
    temp["views"] = to_string( viewsInt );
    temp["game"] = game;
    temp["broadcaster_type"] = broadcasterType;
    
    // cout << name << " " << to_string(followersInt) << " " << to_string(viewsInt) << " " << game << " " << broadcasterType << endl;
    // cout << endl;

    return temp;

}