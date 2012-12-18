# -*- coding: utf-8 -*-
import urllib2
import cookielib
import urllib
import re
import base64

def getIp(url='http://192.168.1.1/userRpm/StatusRpm.htm', usr='admin', password='admin'):
    cj = cookielib.CookieJar()
    req = urllib2.Request(url)
    usr_passwd_bas64 = base64.encodestring('%s:%s' % (user,password))[:-1]
    req.add_header("Accept", "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8")
    req.add_header("Accept-Charset","GBK,utf-8;q=0.7,*;q=0.3")
    req.add_header("Accept-Encoding", "gzip,deflate,sdch")
    req.add_header("Accept-Language", "zh-CN,zh;q=0.8")
    req.add_header("Connection", "keep-alive")
    req.add_header("Host", "192.168.1.1")
    req.add_header("Authorization", "Basic %s" % usr_passwd_bas64)
    req.add_header("User-Agent","Mozilla/5.0 (Windows NT 6.1) AppleWebKit/537.1 (KHTML, like Gecko) Chrome/21.0.1180.79 Safari/537.1")
    opener = urllib2.build_opener(urllib2.HTTPCookieProcessor(cj))
    html = opener.open(req)
    response = html.read()
    l = response.split('\n')
    for i in xrange(len(l)):
        if -1 != l[i].find('var wanPara'):
            ll = l[i+1].split(', ')
            return ll[2][1:-1]
    return None

if __name__ == '__main__':
    print getIp();
 

 

