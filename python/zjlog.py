import sys
import inspect
import datetime
def log(s, frame, fd=sys.stdout):
    file = inspect.getframeinfo(frame).filename
    func = inspect.getframeinfo(frame).function
    line =  inspect.getframeinfo(frame).lineno
    s = ('[%s %s(%d)%s]:%s') % (datetime.datetime.now(), file, line, func, s)
    fd.write(s)

def f():
    log('hello log', inspect.currentframe())

if '__main__' == __name__:
   f() 
