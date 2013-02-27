#coding=utf-8 
import urllib2
import os
import sys

c = ''
while True:
    c = raw_input('PRESS Y TO EXIT\n')
    if c == 'y' or c == 'Y':
        break
    print '--- visit start ---'
    if len(sys.argv) < 2:
        url = 'http://www.baidu.com'
    else:
        url = sys.argv[1]
    req = urllib2.Request(url)
    content = urllib2.urlopen(req)
    html = content.read()
    try:
        print html.decode('gbk')
    except:
        try:
            print html.decode('utf-8')
        except:
            print html
    print '--- visit end ---'
