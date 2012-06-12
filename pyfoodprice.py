#/usr/bin/python 
# -*- coding: utf-8 -*- 
import urllib
import urllib2
import sys
import operator 
def snatchFromBJBLQ(page_idx, food_type):
    """snatch html page from bjblq.com"""
    d = dict()
    url = 'http://www.bjblq.com/price.asp'
    post_data = urllib.urlencode(
            {
                'Page':str(page_idx),
                'PriceType':str(food_type),
            }
    )
    req = urllib2.Request(url, post_data)
    try:
        conn = urllib2.urlopen(req)
    except urllib2.URLError,e: 
        print 'URLError:', e
    except urllib2.HTTPError,e:    
        print 'HTTP Error:', e
    result = conn.read()
    return result.split('\n')

def getFoodInfo(l):
    """parse html to find the begin line of food information, food page number and food count."""
    size = len(l)
    cur = -1
    page = 1
    count = 0
    find_count = False
    date = False
    for i in xrange(size):
        if l[i].find('adform') != -1:
            cur = i + 14    #from the line there is 'adform'.we jump 14 lines to get information
            date = l[i-5].strip() 
            find_count = True
            continue
        if find_count == False:
            continue
        else:
            if l[i].find('共有') != -1:
                s = l[i].strip()
                s = s[48:s.find('</font>')]
                count = int(s)
                if count % 30 != 0:
                    page = count/30 + 1
                else:
                    page = count/30
                break
    return (cur, page, count,date)

def saveFood(l, cur, page, count, food_type):
    """save food to a dictionary"""
    d = {}
    if page == 1:
        d = parseItems(l, cur, count)
    else:
        for i in xrange(page):
            if i == 0:
                d.update(parseItems(l, cur, 30))
            else:
                l = snatchFromBJBLQ(i+1, food_type)
                cur, page_tmp, count_tmp, date = getFoodInfo(l)
                d.update(parseItems(l, cur, count - 30 * i))
    return d

def parseItems(l, cur, count):
    """parse context to get the name and price of every kind of food"""
    d = {}
    for i in xrange(count):
        s = l[cur].strip()
        key = s[36:len(s)-11]
        if sys.platform == 'darwin':
            key = key.decode('gbk').encode('utf-8')
        cur += 4
        s = l[cur].strip()
        value = s[23:len(s)-11]
        d[key] = float(value)
        cur += 5
    return d

def main():
    """main function"""
    food_type = {}
    food_type['蔬菜'] = 1
    food_type['水果'] = 2
#    food_type['粮油'] = 3
#    food_type['水产'] = 4
#    food_type['畜禽蛋品'] = 5
#    food_type['副食调料'] = 6
    for k in food_type.keys():
        print '=====', k, '====='
        html = snatchFromBJBLQ(1, food_type[k])
        cur, page, count, date = getFoodInfo(html)
        d = saveFood(html, cur, page, count, food_type[k])
        sorted_d = sorted(d.iteritems(), key=operator.itemgetter(1), reverse=False)
        food_type[k] = sorted_d 
#        for i in xrange(len(food_type[k])):
        for i in xrange(10):
            print food_type[k][i][0], ':', food_type[k][i][1]
    print '+++++++++++'+ date + '+++++++++++++++'
if '__main__' == __name__:
    main()
