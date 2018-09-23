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
        for submission in subreddit.hot(limit=25):
            logging.info('Title: %s - Score: %s', submission.title, submission.score)

            if 'clips.twitch.tv' in submission.url:
                slug = submission.url[24:]
                logging.info('Slug: %s', slug)

                streamer, sid, title, clipViews, clipDate  = getClipInfo(twitch, slug)
                followers, views = getChannelInfo(twitch, sid)

                d = OrderedDict()
                d['streamer'] = streamer
                d['sid'] = sid
                d['followers'] = followers
                d['views'] = views
                d['slug'] = slug
                d['score'] = submission.score
                d['title'] = title
                d['clip views'] = clipViews
                d['clip date'] = clipDate

                insertTOStreamerTable(db, d)
                vector.append(d)
    
    print("------")

    # for x in vector:
    #     print(x)
    #     print()

    return vector

def getClipInfo(twitch, slug):

    clip = twitch.clips.get_by_slug(slug)

    #print(clip)
    logging.info('Streamer: %s - ID: %s - Title: %s - Clip Views: %s - Datetime: %s', clip.broadcaster['name'], 
                    clip.broadcaster['id'], clip.title, clip.views, clip.created_at)

    return clip.broadcaster['name'], clip.broadcaster['id'], clip.title, clip.views, clip.created_at


def getChannelInfo(twitch, id):
    channel = twitch.channels.get_by_id(id)

    logging.info('Followers: %s - Views: %s\n', channel.followers, channel.views)

    return channel.followers, channel.views

def createStreamerTable(db):
    cursor = db.cursor()

    sqlTable = """CREATE TABLE IF NOT EXISTS channels (
                    CHANNEL VARCHAR(30) NOT NULL,
                    ID          INT NOT NULL,
                    FOLLOWERS   INT NOT NULL,
                    VIEWS       INT NOT NULL )"""
    
    cursor.execute(sqlTable)

    # the following will not work if there are non unique entries in the table

    constraint = "ALTER TABLE channels \
                    ADD CONSTRAINT UC_streamer UNIQUE (channel, id)"

    try:
        cursor.execute(constraint)
        db.commit()
    
    except:
        db.rollback()
        print('Error: Unable to add constraints')

def insertTOStreamerTable(db, d):
    cursor = db.cursor()

    # TODO: has map to check if table exist

    sql = "INSERT INTO channels(CHANNEL, ID, FOLLOWERS, VIEWS) \
            VALUES ( '%s', '%s', '%s', '%s')" % \
            (d['streamer'], d['sid'], d['followers'], d['views'])

    try:
        cursor.execute(sql)
        db.commit()

    except:
        db.rollback()
        #print('Error: Unable to add to streamer table')
    

def duplicateCheck(db):
    cursor = db.cursor()

    sql = "SELECT channel, COUNT(channel) \
            FROM channels \
            GROUP BY channel \
            HAVING COUNT(channel) > 1"

    try:
        cursor.execute(sql)

        results = cursor.fetchall()
        for row in results:
            print(row)
    
    except:
        print('Error: Unable to fetch data')

def clipTables(reddit, twitch, db, dataVector):

    for channel in dataVector:
        streamer = channel['streamer']
        slug = channel['slug']
        clip = channel['title']
        date = channel['clip date']

        createChannelOverviewTable(streamer, db)
        insertChannelOverviewTable(streamer, clip, slug, date, db)

def createChannelOverviewTable(streamer, db):
    cursor = db.cursor()
    # TODO: has map to check if table exist

    sql = """CREATE TABLE IF NOT EXISTS """ + streamer + "(\
                CLIP    VARCHAR(200) NOT NULL UNIQUE, \
                SLUG    VARCHAR(100), \
                DATE    DATE NOT NULL, \
                TIME    TIME NOT NULL )"
    
    try:
        cursor.execute(sql)

    except:
        print('Could not create table')
    
def insertChannelOverviewTable(streamer, clip, slug, date, db):


    cursor = db.cursor()

    clip = clip.replace('\'', '')
    print('Attempting', streamer, '-', clip, date)


    sql = "INSERT INTO " + streamer + "(CLIP, SLUG, DATE, TIME) \
            VALUES ( '%s', '%s', '%s', '%s' )" % \
            (clip, slug, date.date(), date.time())

    try:
        cursor.execute(sql)
        db.commit()
    
    except:
        db.rollback()
        print('Error: Unable to create overview table\n')
        

def main():
    db = connectDB()
    reddit = getRedditAPIAccess()
    twitch = getTwitchAPIAccess()

    createStreamerTable(db)
    dataVector = getFrontPage(reddit, db, twitch)

    clipTables(reddit, twitch, db, dataVector)

    #duplicateCheck(db)

    #print(dataVector[0]['streamer'])

main()
