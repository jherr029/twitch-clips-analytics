#ifndef EXTERN_H_
#define EXTERN_H_

const std::string redditURL("https://www.reddit.com/r/livestreamfail/hot.json?limit=25");
const std::string clipURL("https://api.twitch.tv/helix/clips?id=");
const std::string channelURL("https://api.twitch.tv/kraken/channels/");
const std::string streamURL("https://api.twitch.tv/helix/streams?id=");

// I may have been using extern wrongly

struct rawChannelStats
{
    std::string name;

    unsigned int views;
    unsigned int followers;

    std::string dateUpdated;
    std::string timeUpdated;

};

#endif