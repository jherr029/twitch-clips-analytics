#include "headers/curl.h"
#include "headers/extern.h"

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

string curlGetJson()
{
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