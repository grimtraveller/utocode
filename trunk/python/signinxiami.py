# -*- coding: utf-8 -*-
import urllib2
import cookielib
import urllib
import re
import sys
import HTMLParser

post = {}

host = 'http://www.xiami.com'

class Login(HTMLParser.HTMLParser):
    def handle_starttag(self, tag, attrs):
        if tag == 'form':
            for attr in attrs:
                if attr[0] == 'action':
                    post['url'] = attr[1]
        if tag == 'input':
            flag = False
            for attr in attrs:
                if attr[0] == 'name':
                    if attr[1] == 'LoginButton':
                        flag = True
                        break
            if flag == True:
                for attr in attrs:
                    if attr[0] == 'value':
                        post['submit'] = attr[1]

httpHandler = urllib2.HTTPHandler(debuglevel=1)
index = 'http://m.xiami.com'
cj = cookielib.CookieJar()
print '---------------visit-------------------'
opener = urllib2.build_opener(urllib2.HTTPCookieProcessor(cj))
req = urllib2.Request(index)
content = opener.open(req)
html = content.read().decode('utf-8')
parser = Login()
parser.feed(html)
post['url'] = host + post['url']
post['email'] = sys.argv[1]
post['password'] = sys.argv[2]
print post['url']

print '-----------------login-------------------'
data = 'email=' + urllib.quote(post['email']) + '&'
data += 'password=' + urllib.quote(post['password']) + '&'
data += 'LoginButton=' + urllib.quote(post['submit'].encode('utf-8'))
print data
try:
    req = urllib2.Request(post['url'], data)
except urllib2.HTTPError, e:
    print e.code
    exit(0)


content = opener.open(req)

html = content.read()

signin = None
class SignIn(HTMLParser.HTMLParser):
    def handle_starttag(self, tag, attrs):
        global signin
        if tag == 'a':
            flag = False
            for attr in attrs:
                if attr[0] == 'class':
                    if attr[1] == 'check_in':
                        flag = True
                        break
            if flag == True:
                for attr in attrs:
                    if attr[0] == 'href':
                        signin = attr[1]
                        print signin
                        return

print '---------------sign in -------------------'
parser = SignIn()
parser.feed(html)
print signin
if signin is not None:
    try:
        req = urllib2.Request(host+signin)
    except usrllib2.HTTPError, e:
        print e.code
        exit(0)
    req.add_header('Referer', 'http://www.xiami.com/web')
    content = opener.open(req)
    print content.read().decode('utf-8')
else:
    print html.decode('utf-8')

print '--------------log out --------------------'

print host+'/member/logout?from=mobile'
req = urllib2.Request(host+'/member/logout?from=mobile')
rep = opener.open(req)
html =rep.read()
#print html.decode('utf-8')

