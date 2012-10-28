import json
import urllib
import sys

#http://fanyi.youdao.com/openapi?path=data-mode
#http://sourceforge.net/p/eyoudao/code/3/tree/trunk/pyfanyi/pyfanyi.py#l27
def translateByYouDao(w):
    result = ''
    url = 'http://fanyi.youdao.com/openapi.do?keyfrom=zjdict&key=701234458&type=data&doctype=json&version=1.1&q=' + w
    f = urllib.urlopen(url)
    c = f.read()
    if c != 'no query':
        d = json.loads(c)
        if 'errorCode' in d.keys() and d['errorCode'] == 0:
            if 'translation' in d.keys():
                for i in d['translation']:
                    result += i
    return result

if __name__ == '__main__':
    print translateByYouDao(sys.argv[1])
 

