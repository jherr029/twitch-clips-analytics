import praw 
import json
import logging
from twitch import TwitchClient
from pathlib import Path


logging.basicConfig(level=logging.INFO)


#TODO: check if api access is true
def getRedditAPIAccess():
    reddit = praw.Reddit('bot')
    return reddit

#TODO: check if api access is true
def getTwitchAPIAccess():
    home = str(Path.home())
    home = home + '/.tw'

    cid = open(home)
    twID = cid.read()

    twitch = TwitchClient(twID)
    return twitch

def getFrontPage(reddit):
    twitch = getTwitchAPIAccess()

    vector = []

    if (reddit.read_only):
        subreddit = reddit.subreddit('livestreamfail')
        for submission in subreddit.hot(limit=10):
            logging.info('Title: %s - Score: %s', submission.title, submission.score)
            slug = submission.url[24:]
            logging.info('Slug: %s', slug)

            streamer, sid, clipViews  = getClipInfo(twitch, slug)
            followers, views = getChannelInfo(twitch, sid)
            vector.append((streamer, sid, followers, views, slug, submission.score, clipViews))
    
    print("------")

    for x in vector:
        print(x)

    return vector

def getClipInfo(twitch, slug):

    clip = twitch.clips.get_by_slug(slug)

    logging.info('Streamer: %s - ID: %s -  Clip Views: %s', clip.broadcaster['name'], clip.broadcaster['id'], clip.views)

    return clip.broadcaster['name'], clip.broadcaster['id'], clip.views 

def getChannelInfo(twitch, id):
    channel = twitch.channels.get_by_id(id)

    logging.info('Followers: %s - Views: %s\n', channel.followers, channel.views)

    return channel.followers, channel.views

def main():
    reddit = getRedditAPIAccess()
    twitch = getTwitchAPIAccess()

    dataVector = getFrontPage(reddit)

main()
