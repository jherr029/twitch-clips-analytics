#include "../headers/jsonParse.h"

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
    int positionOfQ;

    // a not banned user
    if ( url.find("clip") )
    {
        if ( positionOfQ = url.find("?") )
            return url.substr( 24, positionOfQ - 24 );

        else
            return url.substr( 24 );

    }

    else
        return "banned";

}

unordered_map< string, string > twitchJsonParseClip( Document & jsonDoc )
{
    unordered_map< string, string > tempMap;

    Value & tempValue = jsonDoc["data"];

    // TODO: Need to handle when an account is banned
    // Two scenarios I am aware of
    // If this is the first time encourtering the streamer
    // then the program is unware of who the streamer is 
    // maybe the Name can be gathered a different way
    //
    // 2nd, I know who the streamer is, need to change the status as
    // banned. Maybe return a different hashmap with the size of one
    if ( tempValue.Size() == 0 )
    {
        // cout << "NULL" << endl;
        return tempMap;
    }

    // Convert this to Value & to avoid ["data"][0]

    // prettyPrint(jsonDoc);
    tempMap["id"] = jsonDoc["data"][0]["broadcaster_id"].GetString();
    tempMap["title"] = jsonDoc["data"][0]["title"].GetString();
    tempMap["views"] = to_string(jsonDoc["data"][0]["view_count"].GetInt());
    tempMap["game_id"] = jsonDoc["data"][0]["game_id"].GetString();

    string dateTime = jsonDoc["data"][0]["created_at"].GetString();
    vector<string> dateTimeVec = parseTimeDate(dateTime);
    tempMap["date"] = dateTimeVec[0];
    tempMap["time"] = dateTimeVec[1];

    // maybe here do another api call to get the name of the game

    return tempMap;
}

// TODO: Handle special characters for name of the streamer
// https://stackoverflow.com/questions/5906585/how-to-change-the-default-collation-of-a-database
// the solution

unordered_map< string, string > twitchJsonParseChannel( Document & jsonDoc )
{
    unordered_map<string, string> temp;

    string name, game, broadcasterType, dateTime;
    int followersInt, viewsInt;


    name = jsonDoc["display_name"].GetString();
    followersInt = jsonDoc["followers"].GetInt();
    viewsInt = jsonDoc["views"].GetInt();
    dateTime = jsonDoc["updated_at"].GetString();

    // parse date time and convert utc to pacific time

    Value & tempValue = jsonDoc["game"];
    if ( tempValue.IsNull() )   // in case the streamer has not provided what game they are playing
    {
        cout << "value is null for game ";
        game = "n/a";   // the n/a may indicate also that the current channel is not live
    }

    else
        game = jsonDoc["game"].GetString();

    broadcasterType = jsonDoc["broadcaster_type"].GetString();

    if ( broadcasterType.size() == 0 )
        broadcasterType = "normal";

    temp["display_name"] = name;
    temp["followers"] = to_string( followersInt );
    temp["views"] = to_string( viewsInt );
    temp["game"] = game;
    temp["broadcaster_type"] = broadcasterType;

    vector<string> dateTimeParsed = parseTimeDate(dateTime);

    temp["date"] = dateTimeParsed[0];
    temp["time"] = dateTimeParsed[1];

    return temp;
}

// TODO: convert all vectors to array since content is fixed
vector<string> parseTimeDate( string timeDate )
{
    vector<string> temp;

    string date = timeDate.substr(0, 10);
    string timeValUTC = timeDate.substr(11, 8);
    // timeValUTC.erase(timeValUTC.end());

    temp.push_back(date);
    temp.push_back(timeValUTC);

    // cout << date << " - - - " << timeValUTC << endl;

    return temp;
}