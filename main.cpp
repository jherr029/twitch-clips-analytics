#include <iostream>
#include <string>

#include <cstdint>
#include <memory>
#include <curl/curl.h>
// #include <nlohmann/json.hpp>
// #include <jsoncpp/json/json.h>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/prettywriter.h>

using namespace std;
// using json = nlohmann::json;
using namespace rapidjson;

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

// static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
// {
//     ((std::string*)userp)->append((char*)contents, size * nmemb);
//     return size * nmemb;
// }

int main()
{
    CURL * curl = curl_easy_init();
    CURLcode result; // to delete later

    curl_global_init( CURL_GLOBAL_ALL );

    string url = "https://www.reddit.com/r/livestreamfail/hot.json?limit=10";
    string readBuffer;

    curl_easy_setopt( curl, CURLOPT_URL, url.c_str() );
    curl_easy_setopt( curl, CURLOPT_IPRESOLVE, CURL_IPRESOLVE_V4);
    curl_easy_setopt( curl, CURLOPT_TIMEOUT, 10);
    curl_easy_setopt( curl, CURLOPT_FOLLOWLOCATION, 1L);

    int httpCode(0);

    string strTemp;
    // const char * strTemp;
    // json temp;

    unique_ptr<string> httpData(new string());
    curl_easy_setopt( curl, CURLOPT_WRITEFUNCTION, WriteCallback );

    curl_easy_setopt( curl, CURLOPT_WRITEDATA, strTemp);
    // curl_easy_setopt( curl, CURLOPT_WRITEDATA, httpData.get() );

    curl_easy_perform( curl );
    curl_easy_getinfo( curl, CURLINFO_RESPONSE_CODE, &httpCode);
    curl_easy_cleanup( curl );

    // auto test = json::parse(strTemp);

    Document document;
    document.Parse(strTemp.c_str());

    StringBuffer buffer;
    PrettyWriter<StringBuffer> writer(buffer);
    document.Accept(writer);

    // cout << buffer.GetString() << endl;
    cout << document["data"]["children"][5]["data"]["title"].GetString() << endl;


    // cout << test["data"]["children"][0]["data"]["title"] << endl;
    // cout << test["data"]["children"][1]["data"]["title"] << endl;


    // curl_global_cleanup();
    
    return 0;
}