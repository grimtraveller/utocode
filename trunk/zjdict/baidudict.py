import urllib
import urllib2
import re

def translateByBaiDu(w):
    url = 'http://dict.baidu.com/s?wd=' + urllib.quote(w)
    conn = urllib2.urlopen(url)
    rep = conn.read()
    for l in rep.split('\r'):
        if  -1 !=l.find('tab en-simple-means dict-en-simplemeans-'):
            re_h=re.compile('</?\w+[^>]*>')
            return re_h.sub('', l.strip())
    return None

if __name__ == '__main__':
    import sys
    w = sys.argv[1].decode(sys.stdin.encoding).encode('gbk')
    t = translateByBaiDu(w)
    if t:
        print t.decode('utf-8')
