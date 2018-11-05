#include "headers/curl.h"
#include "headers/extern.h"

#include <curl/curl.h>
#include <cpr/cpr.h>

#include <fstream>

using namespace std;

namespace 
{
    size_t WriteCallback(
        const char * in,
        size_t size,
        size_t num,
        string * out
    )

    {
        const size_t totalBytes( size * num );
        out->append( in, totalBytes );
        return totalBytes;
    }
}

vector<string> getInfo()
{
    string line;
    vector<string> info;

    ifstream infoFile;
    infoFile.open("apiStuff");

    while ( getline( infoFile, line ) )
    {
        // cout << line << endl;
        info.push_back(line);
    }

    infoFile.close();

    return info;
}

string curlGetJsonReddit()
{

    vector<string> info = getInfo();
    string userInfo = info[0] + ":" + info[1];
    // cout << userInfo << endl;

    CURL * curl = curl_easy_init();
    CURLcode result; // to delete later

    curl_global_init( CURL_GLOBAL_ALL );

    string readBuffer;

    curl_easy_setopt( curl, CURLOPT_URL, redditURL.c_str() );
    curl_easy_setopt( curl, CURLOPT_IPRESOLVE, CURL_IPRESOLVE_V4);
    curl_easy_setopt( curl, CURLOPT_TIMEOUT, 10);
    curl_easy_setopt( curl, CURLOPT_FOLLOWLOCATION, 1L);

    int httpCode(0);

    string strTemp;

    curl_easy_setopt( curl, CURLOPT_USERPWD, userInfo.c_str() );
    curl_easy_setopt( curl, CURLOPT_XOAUTH2_BEARER, info[3].c_str() );
    curl_easy_setopt( curl, CURLOPT_WRITEFUNCTION, WriteCallback );

    curl_easy_setopt( curl, CURLOPT_WRITEDATA, strTemp);

    curl_easy_perform( curl );
    curl_easy_getinfo( curl, CURLINFO_RESPONSE_CODE, &httpCode);
    curl_easy_cleanup( curl );

    if ( httpCode == 200 )
        cout << "succussful get from reddit" << endl;

    else
    {
        cout << "unsuccessful get from reddit" << endl;
        return "error";

    }

    curl_global_cleanup();

    return strTemp;
}


string getInfoTwitch()
{
    ifstream file;
    file.open("twitchApi");

    string line;

    getline( file, line );

    return line;
}

string curlGetJsonTwitch( string slug )
{
    string strTemp2;

    string info = getInfoTwitch();
    struct curl_slist * list = NULL;

    string client = "Client-ID: " + info;
    list = curl_slist_append(list, client.c_str());

    string urlTemp = clipURL + slug;

    CURL * curl = curl_easy_init();
    CURLcode result; // to delete later

    curl_global_init( CURL_GLOBAL_ALL );

    string readBuffer;

    curl_easy_setopt( curl, CURLOPT_URL, urlTemp.c_str() );
    curl_easy_setopt( curl, CURLOPT_IPRESOLVE, CURL_IPRESOLVE_V4);
    curl_easy_setopt( curl, CURLOPT_TIMEOUT, 10);
    curl_easy_setopt( curl, CURLOPT_FOLLOWLOCATION, 1L);

    int httpCode(0);

    curl_easy_setopt( curl, CURLOPT_HTTPHEADER, list );
    // curl_easy_setopt( curl, CURLOPT_XOAUTH2_BEARER, info.c_str() );
    curl_easy_setopt( curl, CURLOPT_WRITEFUNCTION, WriteCallback );

    curl_easy_setopt( curl, CURLOPT_WRITEDATA, strTemp2);

    curl_easy_perform( curl );
    curl_easy_getinfo( curl, CURLINFO_RESPONSE_CODE, &httpCode);
    curl_easy_cleanup( curl );

    if ( httpCode == 200 )
        cout << "succussful get from twitch" << endl;

    else
    {
        cout << httpCode;
        cout << " unsuccessful get from twitch" << endl;
        return "error";
    }

    curl_global_cleanup();
    

    return strTemp2;
}
