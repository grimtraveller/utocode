#/usr/bin/python 
# -*- coding: utf-8 -*- 
import urllib
import urllib2
def getPage(page_no, price_type):
    d = dict()
    url = 'http://www.bjblq.com/price.asp'
    post_data = urllib.urlencode(
            {
                'Page':page_no,
                'PriceType':price_type,
            }
    )
    req = urllib2.Request(url, post_data)
    try:
        conn = urllib2.urlopen(req)
    except URLError,e:
        print('URLError:', e.code)
    except HTTPError,e:    
        print('HTTP Error:', e.reason)
    result = conn.read()
    l = result.split('\n')
    size = len(l)
    jump = -1
    for i in xrange(size):
        if l[i].find('adform') != -1:
            jump = 15    #找到包含adform行后跳过五行
        if jump == -1:
            continue
        else:
            jump -= 1
        if jump == 0:
            s = l[i].strip()
            key = s[36:len(s)-11]
            key = key.decode('gbk').encode('utf-8')
            s = l[i+4].strip()
            value = s[23:len(s)-11]
            value = value.decode('gbk').encode('utf-8')
            d[key] = value
            print key,value,str(jump)
    return d


if '__main__' == __name__:
    print getPage('1', '1')
