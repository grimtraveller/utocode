#coding=utf-8 
import urllib
import urllib2
import sys
import operator 
def snatchFromBJBLQ(page_idx, food_type):
    """snatch html page from bjblq.com"""
    d = dict()
    url = 'http://www.bjblq.com/price.aspx'
    post_data = urllib.urlencode(
            {
                'Page':str(page_idx),
                'PriceType':str(food_type),
            }
    )
    req = urllib2.Request(url+'?'+post_data)
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
    key = '最新发布时间：'
    count_key = '共有'
    date_end_key = '</font>'
    prefix_of_count = '<font color=\'red\'>'
    suffix_of_count = '</font>'
    count_per_page = 40
    size = len(l)
    cur = -1
    page = 1
    count = 0
    find_count = False
    date = False
    for i in xrange(size):
        if sys.platform == 'win32':
            pos = l[i].find(key.decode('gbk').encode('utf8'))
        else:
            pos =  l[i].find(key)
        if pos != -1:
            date_end_pos = l[i].find(date_end_key) 
            date = l[i][date_end_pos-9:date_end_pos]
            find_count = True
            cur = i + 21
            continue
        if find_count == False:
            continue
        else:
            if sys.platform == 'win32':
                pos = l[i].find(count_key.decode('gbk').encode('utf8'))
            else:
                pos = l[i].find(count_key)
            if pos != -1:
                s = l[i].strip()
                s = s[s.find(prefix_of_count)+len(prefix_of_count):s.find(suffix_of_count)]
                count = int(s)
                if count % count_per_page != 0:
                    page = count/count_per_page + 1
                else:
                    page = count/count_per_page
                break
    return (cur, page, count, date)

def saveFood(l, cur, page, count, food_type):
    """save food to a dictionary"""
    d = {}
    if page == 1:
        d = parseItems(l, cur, count)
    else:
        for i in xrange(page):
            if i == 0:
                d.update(parseItems(l, cur, 40))
            else:
                l = snatchFromBJBLQ(i+1, food_type)
                cur, page_tmp, count_tmp, date = getFoodInfo(l)
                d.update(parseItems(l, cur, count - 40 * i))
    return d

def parseItems(l, cur, count):
    """parse context to get the name and price of every kind of food"""
    d = {}
    for i in xrange(count):
        s = l[cur].strip()
        pos = s.find('>')
        s = s[pos+1:]
        key = s[:s.find('<')]
        cur += 4
        s = l[cur].strip()
        s = l[cur].strip()
        pos = s.find('>')
        s = s[pos+len('&nbsp;')+1:]
        value = s[:s.find('<')-len('&nbsp;')]
        d[key] = float(value)
        cur += 4
    return d

def main():
    """main function"""
    food_type = {}
    food_type['蔬菜'] = 1
    food_type['果品'] = 2
    food_type['畜禽蛋品'] = 4
    food_type['水产'] = 8 
    food_type['粮油饲料'] = 16
    for k in food_type.keys():
        print '=====', k, '====='
        html = snatchFromBJBLQ(1, food_type[k])
        cur, page, count, date = getFoodInfo(html)
        d = saveFood(html, cur, page, count, food_type[k])
        sorted_d = sorted(d.iteritems(), key=operator.itemgetter(1), reverse=False)
        food_type[k] = sorted_d
        for i in xrange(len(food_type[k])):
            if sys.platform == 'win32':
                print food_type[k][i][0].decode('utf8'), ':', food_type[k][i][1]
            else:
                print food_type[k][i][0], ':', food_type[k][i][1]
    print '+++++++++++'+ date + '+++++++++++++++'
if '__main__' == __name__:
    main()


