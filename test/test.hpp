#include "../includes/curl.h"
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

TEST(curlGetJsonReddit, successful)
{
    ASSERT_STRNE("", curlGetJsonReddit().c_str());
}

}

