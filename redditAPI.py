import praw 
from twitch import TwitchClient
from pathlib import Path

home = str(Path.home())
home = home + '/.tw'

print(home)

cid = open(home)
twID = cid.read()

reddit = praw.Reddit('bot')
twitchC = TwitchClient(twID)

clip = twitchC.clips.get_by_slug('LachrymoseInterestingKaleHoneyBadger')

print(clip)


if (reddit.read_only):
    subreddit = reddit.subreddit('livestreamfail')
    for submission in subreddit.hot(limit=10):
        print(submission.title, '-' ,submission.score)
        print(submission.url)

else:
    print('API is not working')