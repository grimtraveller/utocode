#coding=utf-8 
import urllib
import urllib2
import sys
import operator 
import HTMLParser
import datetime
import operator 
import getopt
import os
class XiFaDi(object):
    def feed(self, url):
        req = urllib2.Request(url)
        content = urllib2.urlopen(req)
        html = content.read().decode('gbk')
        price = {}
        class Parser(HTMLParser.HTMLParser):
            def __init__(self):
                self.cur = None
                self.new = False
                self.handle = False
                self.date = None
                HTMLParser.HTMLParser.__init__(self)
            def handle_starttag(self, tag, attrs):
                if tag == 'td':
                    propertys = {}
                    for key,value in attrs:
                        propertys[key] = value
                    if propertys['width'] == '16%' and -1 !=propertys['style'].find('background'):
                        self.new = True
                    elif propertys['width'] == '10%' and -1 !=propertys['style'].find('background'):
                        self.handle = True
                    elif propertys['width'] == '13%' and -1 !=propertys['style'].find('background'):
                        self.handle = True
                    else:
                        pass
            def handle_data(self, data):
                if self.new:
                    price[data] = []
                    if self.cur is not None and self.last is not None:
                        if self.last[-2] != self.cur[-2]:
                            self.date = self.last[-2]
                            raise Exception('finish')
                    self.last = self.cur
                    self.cur = price[data]
                    self.new = False
                if self.handle:
                    self.cur.append(data)
                    self.handle = False
        try:
            p = Parser()
            p.feed(html)
        except Exception, e:
            if str(e) == 'finish':
                return True, price,p.date
            else:
                raise e
        return False, price, p.date
    
    
    def price(self, n = 0, output = sys.stdout):
        vegetable = 1
        fruit = 2
        meatfood = 3
        seafood = 4
        mainfood = 5
        if n == 0:
            showall = True
        else:
            showall = False
        for mid in [vegetable, fruit, meatfood, seafood, mainfood]:
            all = {}
            page = 1 
            while True:
                url = ('http://web.xinfadi.com.cn/price/?mid=%s&page=%s') % (mid, page)
                finish, p, date = self.feed(url)
                
                if finish:
                    all.update(p)
                    break
                if len(all) != 0 and all.values()[0][-2] != p.values()[0][-2]:
                    break
                else:
                    all.update(p)
                    page += 1
            p = {}
            for key, value in all.iteritems():
                if len(value) != 0:
                    p[key] = float(value[1])
            price = sorted(p.iteritems(), key=operator.itemgetter(1), reverse=False)
            if showall:
                n = len(price)
            for i in range(0, n):
                if output == sys.stdout and sys.platform == 'win32':
                    print price[i][0], '=', price[i][1]
                else:
                    output.write(str(price[i][0].encode('utf-8'))+'='.encode('utf-8')+str(price[i][1])+os.linesep)

            output.write('===')
            if date != None:
                output.write(date.encode('utf-8')+os.linesep)

class BaLiQiao(object):
    def snatchFromBJBLQ(self, page_idx, food_type):
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
        return result.split(os.linesep)
    
    def getFoodInfo(self, l):
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
                pos = l[i].find(key)
            else:
                pos = l[i].find(key)
            if pos != -1:
                date_end_pos = l[i].find(date_end_key)
                date = l[i][date_end_pos-10:date_end_pos]
                year = int(date[date.find('-')-4:date.find('-')])
                date = date[date.find('-')+1:]
                month = int(date[0:date.find('-')])
                date = date[date.find('-')+1:]
                day = int(date)
                date = str(year)+'-'+str(month)+'-'+str(day)
                find_count = True
                cur = i + 21
                continue
            if find_count == False:
                continue
            else:
                if sys.platform == 'win32':
                    pos = l[i].find(count_key)
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
    
    def saveFood(self, l, cur, page, count, food_type):
        """save food to a dictionary"""
        d = {}
        if page == 1:
            d = self.parseItems(l, cur, count)
        else:
            for i in xrange(page):
                if i == 0:
                    d.update(self.parseItems(l, cur, 40))
                else:
                    l = self.snatchFromBJBLQ(i+1, food_type)
                    cur, page_tmp, count_tmp, date = self.getFoodInfo(l)
                    d.update(self.parseItems(l, cur, count - 40 * i))
        return d
    
    def parseItems(self, l, cur, count):
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

    def price(self, n = 0, output = sys.stdout):
        if n == 0:
            showall = True
        else:
            showall = False
        """main function"""
        food_type = {}
        food_type['蔬菜'] = 1
        food_type['果品'] = 2
        food_type['畜禽蛋品'] = 4
        food_type['水产'] = 8 
        food_type['粮油饲料'] = 16
        for k in food_type.keys():
            if output == sys.stdout and sys.platform == 'win32':
                print '====='+k.decode('utf8')+'====='
            else:
                output.write('====='+k+'====='+os.linesep)
            html = self.snatchFromBJBLQ(1, food_type[k])
            cur, page, count, date = self.getFoodInfo(html)
            d = self.saveFood(html, cur, page, count, food_type[k])
            sorted_d = sorted(d.iteritems(), key=operator.itemgetter(1), reverse=False)
            food_type[k] = sorted_d
            if showall:
                n = len(food_type[k])
            for i in xrange(n):
                if output == sys.stdout and sys.platform == 'win32':
                    print food_type[k][i][0].decode('utf-8')+':'+str(food_type[k][i][1])
                else:
                    output.write(food_type[k][i][0]+':'+str(food_type[k][i][1])+os.linesep)
        output.write('+++++++++++'+date+'+++++++++++++++'+os.linesep)
def help():
    print ("""\
USEAGE:
%s [-a|--all] [-h|--help][-o <file>|--output=<file>]
    all     Show all kinds of food
    help    Help
    output  output in <file>
""") % (sys.argv[0])
def main():
    showall = False
    output = sys.stdout
    try:
        opts, args = getopt.getopt(sys.argv[1:], "aho:", ["all", "help", "output="])
    except:
        help()
        return 0
    for o, a in opts:
        if o in ('-h', '--help'):
            help()
            return 0
        if o in ('-o', '--output'):
            output = open(a, 'wb')
        if o in ('-a', '--all'):
            showall = True
    if showall:
        output.write('BaLiQiao'+os.linesep) 
        BaLiQiao().price(output=output)
        output.write('XiFaDi'+os.linesep)
        XiFaDi().price(output=output)
    else:
        output.write('BaLiQiao'+os.linesep)
        BaLiQiao().price(n=5, output=output)
        output.write('XiFaDi'+os.linesep)
        XiFaDi().price(n=5, output=output)
    
    
if '__main__' == __name__:
    exit(main())
