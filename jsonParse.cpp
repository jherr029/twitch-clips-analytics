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

vector<vector<string> > twitchJsonParse( vector<string> slugs )
{
    // maybe use a vector of hasmhmaps
    vector<vector<string> > vectorOfVec;
}