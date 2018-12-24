#include "../includes/curl.h"
#include "../includes/sqlConnector.h"
#include <gtest/gtest.h>
#include <iostream>
#include <string>

namespace {

TEST(environmental_variables, ifExist)
{
    EXPECT_STRNE("", getenv("REDDIT_LOGIN"));
    EXPECT_STRNE("", getenv("REDDIT_PASS"));
    EXPECT_STRNE("", getenv("REDDIT_KEY"));
    EXPECT_STRNE("", getenv("TWITCH_KEY"));
    EXPECT_STRNE("", getenv("TWITCH_SECRET"));
    EXPECT_STRNE("", getenv("MYSQL_SERVER_REMOTE"));
    EXPECT_STRNE("", getenv("MYSQL_LOGIN"));
    EXPECT_STRNE("", getenv("MYSQL_PASS"));
}

TEST(curl, redditCurl)
{
    curl curlObj;

    curlObj.curlReddit();
    ASSERT_EQ(200, curlObj.getCode());
    ASSERT_STRNE("", curlObj.getCurlResult().c_str());
}


TEST(jsonParse, isVarReddit)
{
    curl curlObj;
    curlObj.curlReddit();

    jsonParse obj;

    obj.createDocument(curlObj.getCurlResult());
    ASSERT_TRUE(obj.redditIsInt());
    ASSERT_TRUE(obj.redditIsArray());
    ASSERT_TRUE(obj.redditIsString());
}

TEST(jsonParse, isVarTwitchClip)
{
    curl curlObj;
    curlObj.curlTwitchClip("AwkwardHelplessSalamanderSwiftRage");

    jsonParse obj;
    obj.createDocument(curlObj.getCurlResult());

    ASSERT_TRUE(obj.twitchClipIsString_id());
    ASSERT_TRUE(obj.twitchClipIsString_title());
    ASSERT_TRUE(obj.twitchClipIsString_gameID());
    ASSERT_TRUE(obj.twitchClipIsString_createdAt());
    ASSERT_TRUE(obj.twitchClipIsInt_viewCount());
}

TEST(jsonParse, isVarTwitchChannel)
{
    curl curlObj;
    curlObj.curlTwitchChannel("67955580");

    jsonParse obj;
    obj.createDocument(curlObj.getCurlResult());

    ASSERT_TRUE(obj.twitchChannelIsString_name());
    ASSERT_TRUE(obj.twitchChannelIsString_updatedAt());
    ASSERT_TRUE(obj.twitchChannelIsString_type());
    ASSERT_TRUE(obj.twitchChannelIsInt_followers());
    ASSERT_TRUE(obj.twitchChannelIsInt_views());
}

TEST(curl, twitchClipCurl)
{
    curl curlObj;

    curlObj.curlTwitchClip("AwkwardHelplessSalamanderSwiftRage");
    ASSERT_EQ(200, curlObj.getCode());
    ASSERT_STRNE("", curlObj.getCurlResult().c_str());

}

TEST(curl, TwitchClipCurlPsuedo400_1)
{
    curl curlObj;

    // A removed clip - this is supposed to be a 400 but a 200 is returned
    curlObj.curlTwitchClip("SpookyPleasantGullFeelsBadMan");
    EXPECT_EQ(200, curlObj.getCode());
    ASSERT_STREQ("{\"data\":[],\"pagination\":{}}", curlObj.getCurlResult().c_str());
}

TEST(curl, TwitchClipCurlPsuedo400_2)
{
    curl curlObj;

    // A removed clip - this is supposed to be a 400 but a 200 is returned
    curlObj.curlTwitchClip("AltruisticPolishedOtterFeelsBadMan");
    EXPECT_EQ(200, curlObj.getCode());
    ASSERT_STREQ("{\"data\":[],\"pagination\":{}}", curlObj.getCurlResult().c_str());
}

TEST(curl, twitchChannelCurl)
{
    curl curlObj;

    curlObj.curlTwitchClip("67955580");
    ASSERT_EQ(200, curlObj.getCode());
    ASSERT_STRNE("", curlObj.getCurlResult().c_str());

}

sqlConnector sqlcpp;
TEST(sqlConnector, connectionSuccesful)
{

    ASSERT_TRUE(sqlcpp.checkConnection());
    // sqlcpp.checkIfTableExist("channels");
    ASSERT_TRUE(sqlcpp.checkIfTableExist("channels"));
    ASSERT_TRUE(sqlcpp.checkIfTableExist("channel_data"));
    ASSERT_TRUE(sqlcpp.checkIfTableExist("slugs_data"));
    ASSERT_FALSE(sqlcpp.checkIfTableExist("channel"));
}

TEST(sqlConnector, tablesExist)
{
    ASSERT_TRUE(sqlcpp.checkIfTableExist("channels"));
    ASSERT_TRUE(sqlcpp.checkIfTableExist("channel_data"));
    ASSERT_TRUE(sqlcpp.checkIfTableExist("slugs_data"));
}

TEST(sqlConnector, tablesDoesNotExist)
{
    ASSERT_FALSE(sqlcpp.checkIfTableExist("channel"));
    ASSERT_FALSE(sqlcpp.checkIfTableExist("channel_dat"));
    ASSERT_FALSE(sqlcpp.checkIfTableExist("slugs data"));
}

}

// TODO:in jenkins use echo $? to get the return value of test. if 1 then test failed else test passed
// so in jenkins modify the pipeline to fail
// look for jenkins plugsins for testing

