#ifndef CURL_H_
#define CURL_H_

#include <iostream>
#include <vector>

std::vector<std::string> getInfo();
std::string curlGetJsonReddit();

std::string getInfoTwitch();
std::string curlGetJsonTwitch( std::string );
// std::string requestGetJson();

#endif