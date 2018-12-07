#include "../includes/curl.h"
#include "../includes/extern.h"

#include <curl/curl.h>

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

curl::curl()
{
    httpResult = 0;
    stringResult = "";
}

void curl::curlReddit()
{

    string userInfo = string(getenv("REDDIT_LOGIN")) + ":" + string(getenv("REDDIT_PASS"));

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
    curl_easy_setopt( curl, CURLOPT_XOAUTH2_BEARER, getenv("REDDIT_KEY") );
    curl_easy_setopt( curl, CURLOPT_WRITEFUNCTION, WriteCallback );

    curl_easy_setopt( curl, CURLOPT_WRITEDATA, strTemp);

    curl_easy_perform( curl );
    curl_easy_getinfo( curl, CURLINFO_RESPONSE_CODE, &httpCode);
    curl_easy_cleanup( curl );

    httpResult = httpCode;

    if ( httpCode == 200 )
    {
        stringResult = strTemp;
        // cout << "succussful get from reddit" << endl;

    }

    else
    {
        errorObj.setErrorInfo(true, __FILE__, __func__, __LINE__ );
        stringResult = "NULL";
        // cout << "error" << endl;
        // cout << "unsuccessful get from reddit" << endl;
    }

    curl_global_cleanup();
}

vector<string> curl::parseReddit()
{
    jsonParse obj;
    obj.createDocument(stringResult);

    return obj.redditParse();
}

// TODO: convert strings to char*
void curl::curlTwitchClip( string slug )
{
    string strTemp2;

    struct curl_slist * list = NULL;

    string client = "Client-ID: " + string(getenv("TWITCH_KEY"));
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

    httpResult = httpCode;

    if ( httpCode == 200 )
    {
        stringResult = strTemp2;
        // cout << "succussful get from twitch clip ";
    }

    else
    {
        errorObj.setErrorInfo(true, __FILE__, __func__, __LINE__ );
        stringResult = "NULL";
        cout << httpCode << " error clip - ";
        // cout << " unsuccessful get from twitch clip" << endl;

        if ( httpCode == 429 )
            cout << "too many calls - ";

    }

    curl_global_cleanup();
    
}

unordered_map<string, string> curl::parseTwitchClip()
{
    jsonParse obj;
    obj.createDocument(stringResult);

    return obj.twitchClipParse();
}

void curl::curlTwitchChannel( string id )
{
    string strTemp2;

    struct curl_slist * list = NULL;

    // string client = "Client-ID: " + info[0];
    string client = "Client-ID: " + string(getenv("TWITCH_KEY"));
    list = curl_slist_append(list, client.c_str());

    string accept = "Accept: application/vnd.twitchtv.v5+json";
    list = curl_slist_append(list, accept.c_str());

    string auth = "Authorization: OAuth " + string(getenv("TWITCH_SECRET"));
    // string auth = "Authorization: OAuth " + info[1];
    list = curl_slist_append(list, auth.c_str());

    string urlTemp = channelURL + id;

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

    httpResult = httpCode;

    if ( httpCode == 200 )
    {
        stringResult = strTemp2;
        // cout << "succussful get from twitch channel ";
    }

    else
    {
        errorObj.setErrorInfo(true, __FILE__, __func__, __LINE__ );
        stringResult = "NULL";
        cout << httpCode << " error - channel  ";
        // cout << " unsuccessful get from twitch channel" << endl;
    }

    curl_global_cleanup();
    
}

unordered_map<string, string> curl::parseTwitchChannel()
{
    jsonParse obj;
    obj.createDocument(stringResult);

    return obj.twitchChannelParse();
}

bool curl::isCallSuccessful()
{
    if ( 200 == httpResult )
        return true;

    return false;
}

int curl::getCode()
{
    return httpResult;
}


string curl::getCurlResult()
{
    return stringResult;
}

bool curl::isError()
{
    return errorObj.errorExist(); 
}