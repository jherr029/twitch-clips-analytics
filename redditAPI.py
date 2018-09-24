import praw 
import json
import logging
import MySQLdb
import threading
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

def createChannelsDictionary(db):
    cursor = db.cursor()

    sql = "SHOW TABLES"

    cursor.execute(sql)
    channelData = cursor.fetchall()

    idDictionary = {}
    slugDictionary = {}
    channelsDictionary = {}

    for table in channelData: 
        channelsDictionary.update({table[0]:1})
        
        if table[0] != 'channels':
            sql = "select slug from " + table[0]
            cursor.execute(sql)
            dataSlug = cursor.fetchall()

            for slug in dataSlug:
                slugDictionary.update({slug[0]:1})

            sql = "select id from channels"
            cursor.execute(sql)
            idList = cursor.fetchall()

            for id in idList:
                idDictionary.update({id:1})

    return channelsDictionary, slugDictionary, idDictionary

def pollForData(reddit, twitch, db, dictionary, slugDictionary, idDictionary):

    threading.Timer(900.0, pollForData, [reddit, twitch, db, dictionary, slugDictionary, idDictionary]).start()

    print('Working...')

    dataVector, dictionary = getFrontPage(reddit, twitch, db, dictionary)
    dictionary, slugDictionary, newIDs = clipTables(reddit, twitch, db, dataVector, dictionary, slugDictionary, idDictionary)

    updateStats(idDictionary, twitch, db)

    idDictionary = updateIDDictionary(idDictionary, newIDs)
    print('Done...\n')

def getFrontPage(reddit, twitch, db, dictionary):

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

                insertTOStreamerTable(db, d, dictionary)
                vector.append(d)
    
    print("------")

    # for x in vector:
    #     print(x)
    #     print()

    return vector, dictionary

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

def createStreamerTable(db, dictionary):

    if 'channels' not in dictionary:
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
        

def insertTOStreamerTable(db, d, dictionary):
    if d['streamer'] not in dictionary:
        cursor = db.cursor()

        # TODO: has map to check if table exist

        sql = "INSERT INTO channels(CHANNEL, ID, FOLLOWERS, VIEWS) \
                VALUES ( '%s', '%i', '%i', '%i')" % \
                (d['streamer'], int(d['sid']), int(d['followers']), int(d['views']))

        try:
            cursor.execute(sql)
            db.commit()

        except:
            db.rollback()
            print('Error: Unable to add', d['streamer'], 'to channel table')
        
    

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

def clipTables(reddit, twitch, db, dataVector, dictionary, slugDictionary, idDictionary):

    newIDs = {}

    for channel in dataVector:
        streamer = channel['streamer']
        slug = channel['slug']
        clip = channel['title']
        date = channel['clip date']


        if channel['sid'] not in idDictionary:
            newIDs.update({channel['sid']:1})

        dictionary = createChannelOverviewTable(streamer, db, dictionary)
        slugDictionary = insertChannelOverviewTable(streamer, clip, slug, date, db, slugDictionary)
    
    return dictionary, slugDictionary, newIDs

def createChannelOverviewTable(streamer, db, dictionary):
    if streamer not in dictionary:
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
            print('Could not create overview table')
        
        dictionary.update({streamer: 1})
    
    # else:
    #     print(streamer, 'already exists in dictionary')
    
    return dictionary
    

    
def insertChannelOverviewTable(streamer, clip, slug, date, db, slugDictionary):
    if slug not in slugDictionary:
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
            print('Error: Unable to add to', streamer, 'overview table\n')

        slugDictionary.update({slug: 1})
    
    return slugDictionary

def updateStats(channelIDs, twitch, db):
    cursor = db.cursor()

    for idNum in channelIDs:
        channel = twitch.channels.get_by_id(idNum[0])


        sql = "UPDATE channels SET FOLLOWERS = " + str(channel.followers) + \
                ", VIEWS = " + str(channel.views) + \
                " WHERE CHANNEL = '%s'" % (channel.name)
        
        try:
            cursor.execute(sql)
            db.commit()
        except:
            print('Unable to update', channel.name, 'stats' )
            db.rollback()

def updateIDDictionary(currentIDs, newIDs):        
    print()
    for curID in newIDs:
        #print(curID)
        currentIDs.update({curID:1})
    
    return currentIDs

def main():
    db = connectDB()
    channelsDictionary, slugDictionary, idDictionary = createChannelsDictionary(db)
    reddit = getRedditAPIAccess()
    twitch = getTwitchAPIAccess()

    createStreamerTable(db, channelsDictionary)
    pollForData(reddit, twitch, db, channelsDictionary, slugDictionary, idDictionary)

    # dataVector, channelsDictionary = getFrontPage(reddit, twitch, db, channelsDictionary)
    # channelsDictionary, slugDictionary = clipTables(reddit, twitch, db, dataVector, channelsDictionary, slugDictionary)

    #duplicateCheck(db)

    #print(dataVector[0]['streamer'])

main()
