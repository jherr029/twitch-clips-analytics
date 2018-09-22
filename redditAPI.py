import praw 
import json
import logging
import MySQLdb
from twitch import TwitchClient
from pathlib import Path
from collections import OrderedDict


#logging.basicConfig(level=logging.INFO)


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

def connectDB():
    home = str(Path.home())
    home = home + '/.sql'

    mid = open(home)

    vec = []
    for line in mid: 
        vec.append(line)

    # reason for the [:-1] is to remove \n
    mydb = MySQLdb.connect(
        host=vec[0][:-1],
        #user=vec[1],
        passwd=vec[1],
        db='lsf'
    )

    return mydb

def getFrontPage(reddit, db, twitch):

    vector = []

    if (reddit.read_only):
        subreddit = reddit.subreddit('livestreamfail')
        for submission in subreddit.hot(limit=10):
            logging.info('Title: %s - Score: %s', submission.title, submission.score)
            slug = submission.url[24:]
            logging.info('Slug: %s', slug)

            streamer, sid, clipViews  = getClipInfo(twitch, slug)
            followers, views = getChannelInfo(twitch, sid)

            d = OrderedDict()
            d['streamer'] = streamer
            d['sid'] = sid
            d['followers'] = followers
            d['views'] = views
            d['slug'] = slug
            d['score'] = submission.score
            d['clip views'] = clipViews

            insertTOStreamerTable(db, d)
            vector.append(d)
    
    print("------")

    for x in vector:
        print(x)
        print()

    return vector

def getClipInfo(twitch, slug):

    clip = twitch.clips.get_by_slug(slug)

    logging.info('Streamer: %s - ID: %s -  Clip Views: %s', clip.broadcaster['name'], clip.broadcaster['id'], clip.views)

    return clip.broadcaster['name'], clip.broadcaster['id'], clip.views 

def getChannelInfo(twitch, id):
    channel = twitch.channels.get_by_id(id)

    logging.info('Followers: %s - Views: %s\n', channel.followers, channel.views)

    return channel.followers, channel.views

def createStreamerTable(db):
    cursor = db.cursor()

    sqlTable = """CREATE TABLE IF NOT EXISTS STREAMERS (
                    CHANNEL VARCHAR(20) NOT NULL,
                    ID          INT NOT NULL,
                    FOLLOWERS   INT NOT NULL,
                    VIEWS       INT NOT NULL )"""
    
    cursor.execute(sqlTable)

def insertTOStreamerTable(db, d):
    cursor = db.cursor()

    sql = "INSERT INTO STREAMERS(CHANNEL, ID, FOLLOWERS, VIEWS) \
            VALUES ( '%s', '%s', '%s', '%s')" % \
            (d['streamer'], d['sid'], d['followers'], d['views'])

    try:
        cursor.execute(sql)
        db.commit()

    except:
        db.rollback()


def main():
    db = connectDB()
    reddit = getRedditAPIAccess()
    twitch = getTwitchAPIAccess()

    dataVector = getFrontPage(reddit, db, twitch)


    createStreamerTable(db)

    #print(dataVector[0]['streamer'])

main()
