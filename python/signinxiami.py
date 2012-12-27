# -*- coding: utf-8 -*-
import urllib2
import cookielib
import urllib
import re
import sys
import HTMLParser

post = {}
host = 'http://www.xiami.com'
def index(opener, url):
    """visit http://m.xiami.com"""
    post = {}
    req = urllib2.Request(url)
    html = opener.open(req).read().decode('utf-8')
    class Index(HTMLParser.HTMLParser):
        def handle_starttag(self, tag, attrs):
            if tag == 'form':
                for attr in attrs:
                    if attr[0] == 'action':
                        post['url'] = attr[1]
            if tag == 'input':
                propertys = {}
                for attr in attrs:
                    propertys[attr[0]] = attr[1]
                if 'name' in propertys.keys() and propertys['name'] == 'LoginButton':
                    post['submit'] = propertys['value']
                    return
    Index().feed(html)

    return post

def login(opener, post):
    """login"""
    data = 'email=' + urllib.quote(post['email']) + '&'
    data += 'password=' + urllib.quote(post['password']) + '&'
    data += 'LoginButton=' + urllib.quote(post['submit'].encode('utf-8'))
    try:
        req = urllib2.Request(post['url'], data)
    except urllib2.HTTPError, e:
        print 'login request is failed [errorno = %s]' % e.code
        exit(0)
    html = opener.open(req).read()
    if html.find('退出') != -1:
        return html
    else:
        print 'login is failed'
        exit(0)

def signin(opener, html):
    """sigin"""
    signin = []

    class SignIn(HTMLParser.HTMLParser):
        def handle_starttag(self, tag, attrs):
            if tag == 'a':
                propertys = {}
                for attr in attrs:
                    propertys[attr[0]] = attr[1]
                if 'class' in propertys.keys() and propertys['class'] == 'check_in':
                    signin.append(propertys['href'])
              
    SignIn().feed(html)
    if len(signin) == 0:
        print('the url to signin is not found')
        return ''
    try:
        req = urllib2.Request(host+signin[0])
    except urllib2.HTTPError, e:
        print e.code
        exit(0)
    req.add_header('Referer', 'http://www.xiami.com/web')
    html = opener.open(req).read()
    print html
    pos = html.find('已经连续签到')
    if pos != -1:
        print html[pos:].decode('utf-8')
        

def logout(opener, url):
    """logout"""
    req = urllib2.Request(url)
    html = opener.open(req).read()
    if html.find('退出') == -1:
        print 'logout is failed'
        print 'req=', req
        print 'html=', html
        exit(0)
    return


if __name__ == '__main__':
    emails = ['robot40'+str(i)+'@163.com' for i in range(2)]
    emails.append('learningxna@sina.com')
    httpHandler = urllib2.HTTPHandler(debuglevel=1)
    cj = cookielib.CookieJar()
    opener = urllib2.build_opener(urllib2.HTTPCookieProcessor(cj))
    post = index(opener, 'http://m.xiami.com')
    post['url'] = host + post['url']
    for email in emails:
        post['email'] = email
        post['password'] = raw_input("%s 's password:" % email)
        html = login(opener, post)
        print signin(opener, html)
        logout(opener, host+'/member/logout?from=mobile')
