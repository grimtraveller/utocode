# -*- coding:utf-8 -*-
import sys
import xlrd
import operator
def load(fname):
    d = list()
    bk = xlrd.open_workbook(fname)
    sheet_name = bk.sheet_names()[0]
    sh = bk.sheet_by_index(0)
    nrow, ncols = sh.nrows, sh.ncols
    for row in range(nrow):
        issue = sh.cell_value(row, 0)
        if not isinstance(issue, float):
            continue
        row_data = {}
        row_data['issue'] = int(issue)
        row_data['date'] = sh.cell_value(row, 1)
        row_data['red'] = [];
        for i in range(2,8):
            row_data['red'].append(int(sh.cell_value(row, i)))
        row_data['blue'] = int(sh.cell_value(row, 8))
        d.append(row_data)
    return d

def queryCountByIssue(data, begin, end):
    red_balls = {}
    blue_balls = {}
    for d in data:
        if begin <= d['issue'] and d ['issue'] <= end:
            for i in range(len(d['red'])):
                    ball = d['red'][i]
                    if red_balls.has_key(ball):
                        red_balls[ball] += 1
                    else:
                        red_balls[ball] = 1
            blue_ball = d['blue']
            if blue_balls.has_key(blue_ball):
                blue_balls[blue_ball] += 1
            else:
                blue_balls[blue_ball] = 1
    return red_balls, blue_balls

def sortDict(d):
    return sorted(d.iteritems(), key=operator.itemgetter(1), reverse=False)

if __name__ == '__main__':
    data = load(sys.argv[1])
    if len(sys.argv) != 3:
        b = '2003001'
        e = '2012034'
    else:
        b = sys.argv[2]
        e = sys.argv[3]
    red_balls, blue_balls = queryCountByIssue(data, int(b), int(e))
    
    count = 0
    for key in red_balls.keys():
        count += red_balls[key]
    for key in red_balls.keys():
        print '%2d %d %.2f' % (key, red_balls[key], red_balls[key]*100.0/count)

    count = 0
    for key in blue_balls.keys():
        count += blue_balls[key]
    for key in blue_balls.keys():
        print '%2d,%d,%.2f' % (key, blue_balls[key], blue_balls[key]*100.0/count)
