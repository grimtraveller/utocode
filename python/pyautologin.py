# -*- coding: utf-8 -*-
import urllib2
import cookielib
import urllib
cj = cookielib.CookieJar()
print cj
print '---------------visit-------------------'
opener = urllib2.build_opener(urllib2.HTTPCookieProcessor(cj))
url='http://www.example.com/'
req = urllib2.Request(url)
html = opener.open(req)
print html.read()
print cj
print '-----------------login-------------------'
post_data = 'Parameter1=Value1&Parameter2=Value2&Parameter3=Value3'
req = urllib2.Request(url, post_data)
html = opener.open(req)
print html.read()
print cj
