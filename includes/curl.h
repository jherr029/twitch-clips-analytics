#ifndef CURL_H_
#define CURL_H_

#include <iostream>
#include <stdlib.h>
#include <vector>

std::string curlGetJsonReddit();

std::string curlGetJsonTwitchClip( std::string );
std::string curlGetJsonTwitchChannel( std::string );
// std::string requestGetJson();

#endif