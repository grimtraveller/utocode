# -*- coding: utf-8 -*-
import os
import urllib
import re
import sys
import time
def ardd(d):
    if d < 60:
        return d - 48
    if d < 91:
        return d - 53
    if d == 94:
        return 10
    else:
        return d - 59

def ard(s, y):
    t = list()
    z = 0
    l = len(y)
    k = list()
    for i in xrange(l):
        k.append(ardd(ord(y[i])))
    i = 0
    j = 0
    while i < len(s):
        d = ord(s[i])
        if z == 0:
            if (d == 94) or (d > 47) and (d < 60) or (d > 64) and (d < 91) or (d > 96) and (d < 123):
                x = ardd(d)^k[j];
                if x == 10:
                    t.append(94)
                elif x < 12:
                    t.append(x+48)
                elif x < 38:
                    t.append(x+53)
                else:
                    t.append(x+59)
            elif (d > 60) and (d < 65):
                x = (d-61)^(k[j]&3)
                z = 1
            else:
                if d == 95:
                    x = 0
                elif d == 91:
                    x = 2
                elif d == 93:
                    x = 7
                elif d > 123:
                    x = d - 110
                else:
                    x = d - 32
                x = x^(k[j]&15)
                z = 2
        else:
            x = (x << 6)+(ardd(d)^k[j])
            if z == 2:
                z = 3
            else:
                t.append(x);
                z = 0

        i += 1
        if j + 1 == l:
            j = 0
        else:
            j += 1
    tt = list()
    for iii in xrange(len(t)):
        tt.append(unichr(t[iii]))
    return ''.join(tt)


def filterContents(l):
    for i in xrange(len(l)):
        if -1 !=l[i].find('<h1>'):
            l[i] = l[i].replace('<h1>', '').replace('</h1>', '').replace('<br />', '\n')
        elif -1 != l[i].find('<p class ="nav">'):
            l[i] = '\n'
        elif -1 != l[i].find('</p><p>'):
            l[i] = '\n'
        elif -1 != l[i].find('<p>'):
            l[i] = '\n'
        elif -1 != l[i].find('</p>'):
            l[i] = '\n'
        elif -1 != l[i].find('<div class="fadup">'):
            l[i] = '\n'
        elif -1 != l[i].find('<div class="fad325">'):
            l[i] = '\n'
        elif -1 != l[i].find('<br'):
            l[i] = '\n'
    return str().join(l)

def filterJS(js):
    js = filter_ard(js)
    js = filter_unescape(js)
    arks = find_arksValue(js)
    return arks

def filter_ard(s):
    while True:
        s = s.replace('\n', '')
        s = s.replace('\r', '')
        reg = re.compile('''^.*ard\("(?P<arg1>[^"]*)"(?P<split>[^"]*)"(?P<arg2>[^"]*)''')
        matches = reg.findall(s)
        match_len = len(matches)
        if match_len != 0:
            for i in xrange(match_len):
                s = s.replace('ard(\"'+matches[i][0]+'\"'+matches[i][1]+'\"'+matches[i][2]+'\")', ard(matches[i][0], matches[i][2]))
        else:
            break
    return s

def filter_unescape(s):
    while True:
        s = s.replace('\n', '')
        s = s.replace('\r', '')
        reg = re.compile('''^.*unescape\("(?P<arg1>[^"]*)"\).*''')
        matches = reg.findall(s)
        match_len = len(matches)
        if match_len != 0:
            for i in xrange(match_len):
                s = s.replace('unescape(\"'+matches[i]+'\")', urllib.unquote(matches[i]))
                s = s.replace(' + ', '')
        else:
            break

    while True:
        reg = re.compile('''^.*unescape\((?P<arg1>[^"]*)\).*''')
        matches = reg.findall(s)
        match_len = len(matches)
        if match_len != 0:
            for i in xrange(match_len):
                s = s.replace('unescape('+matches[i]+')', urllib.unquote(matches[i]))
        else:
            break
    return s

def find_arksValue(s):
    i = 0
    arks = dict()
    while True:
        key = 'ark'+str(i)
        regexpression = '.*var[\s\t]*'+key+'[^=]*="(?P<value>[^"]*)"'
        reg = re.compile(regexpression)
        match = reg.match(s)
        if None != match:
            arks[key] = match.group('value')
        else:
            break
        i += 1
    return arks

def filterawk(s, arks):
    while True:
        reg = re.compile('''^.*\<script type="text\/javascript"\>arw\("(?P<arg1>[^"]*)", (?P<arg2>.*)\);<\/script\>''')
        matches = reg.findall(s)
        match_len = len(matches)
        if match_len != 0:
            for i in xrange(match_len):
                r = ard(matches[i][0], arks[matches[i][1]])
                s = s.replace('<script type="text/javascript">arw("'+matches[i][0]+'", '+matches[i][1]+');</script>', r.encode('utf-8'))
        else:
            break
    return s

def readContentPage(htmls):
    is_content = False
    l = list()
    contents = str()
    js = str()
    lines = htmls.split('\n')
    if len(lines) == 1:
        lines = htmls.split('\r')
    for i in xrange(len(lines)):
        if -1 != lines[i].find('document.write'):
            js = lines[i]
            continue
        elif -1 != lines[i].find('Article Begin'):
            is_content = True
            continue
        elif -1 != lines[i].find('Article End'):
            is_content = False
            continue
        else:
            pass
        #save content
        if is_content == True:
            l.append(lines[i])
    arks = filterJS(js)
    for i in xrange(len(l)):
        l[i] = filterawk(l[i], arks)
    contents = '\xef\xbb\xbf'+filterContents(l)
    return contents

def readBookList(name, books):
    f = open(name)
    for line in f.readlines():
        if line[0] == '#':
            continue
        value, key = line.split('|')
        books[key] = value
    f.close()

def main():
    try:
        os.mkdir('download')
    except:
        pass
    reg = re.compile('''^.*/(?P<dir_name>.*)1\.html''')
    books = dict()
    readBookList('10index.dat', books)
    readBookList('20index.dat', books)
    for key in books.keys():
        url = books[key]
        match = reg.match(url)
        prefix_name = match.group('dir_name')
        dir_name = 'download'+os.sep+prefix_name
        try:
            os.mkdir(dir_name)
        except:
            pass
        url_prefix = url[:url.find('1.html')]
        i = 1
        error_count = 0
        while True:
            if len(sys.argv) == 3:
                if sys.argv[1] == '-t':
                    f = open(sys.argv[2], 'rb')
                    htmls = f.read()
                    f.close()
                    contents = readContentPage(htmls)
            else:
                contents = str()
                file_name = dir_name+os.sep+prefix_name+str(i)+'.txt'
                if False == os.path.isfile(file_name):
                    url = 'http://'+url_prefix + str(i) + '.html'
                    print url
                    response = urllib.urlopen(url)
                    if 404 == response.getcode():
                        print '404['+url+']'
                        if error_count >= 10:
                            break
                        else:
                            error_count += 1
                        print 'error_count='+str(error_count)
                    else:
                        error_count = 0
                        htmls = response.read()
                        try:
                            contents = readContentPage(htmls)
                        except:
                            print 'except'
                            file_name = dir_name+os.sep+prefix_name+str(i)+'except.txt'
                            contents = str().join(htmls.split('\n'))
                        w = open(file_name, 'wb')
                        w.write(contents.replace('\r', ''))
                        w.close()
            i = i+1
if '__main__' == __name__:
    main()
