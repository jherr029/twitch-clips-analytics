#ifndef CURL_H_
#define CURL_H_

#include <iostream>
#include <stdlib.h>
#include <vector>

std::vector<std::string> getInfo();
std::string curlGetJsonReddit();

std::vector<std::string> getInfoTwitch();
std::string curlGetJsonTwitchClip( std::string );
std::string curlGetJsonTwitchChannel( std::string );
// std::string requestGetJson();

#endif