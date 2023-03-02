import feedparser
import re
import json

def getwords(html):
    # Remove all the html tags
    txt = re.compile(r'<[^>] + >').sub('',html)
    # Split words by all non-alpha characters
    words = re.compile(r'[^A-Z^a-z]+').Split(txt)
    #Convert to lowercase
    return [word.lower() for word in words if word != '']

#Return title and dictionary of word count for an RSS feed
def getWordCounts(url):
    # Parser the feed
    d = feedparser.parse(url)
    wc = {}

    # Loop over all the entries
    for e in d.entries:
        if 'summary' in e: summary = e.summary
        else: summary  = e.description
        # Extract a list of words
        words = getwords(e.title +' ' + summary)
        for word in words:
            wc.setdefault(word,0)
            wc[word] += 1
    return d.feed.title,wc

# print d['feed']['title']
# d = feedparser.parse('https://www.24h.com.vn/bong-da/mu-ruc-ro-rashford-tai-hien-sieu-tuyet-ki-ronaldinho-pogba-lap-cong-c48a1017532.html')
# print d.feed
# str = json.dumps(d.feed)
# fh = open('RSSfile/RSSparse.xml','w+')
# fh.write(str)
# 1/1/2019
# Ignore this cause of can't get RSS, to be continue....

