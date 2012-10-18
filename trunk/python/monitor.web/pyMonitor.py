import BaseHTTPServer
import SimpleHTTPServer
import os
import sys
import random
import psutil
try:
    from cStringIO import StringIO
except ImportError:
    from StringIO import StringIO
class MonitorHTTPServer(SimpleHTTPServer.SimpleHTTPRequestHandler):
    def mkDataJSON(self):
        vm = psutil.virtual_memory()
        r = vm.percent
        f = StringIO()
        s = '{\n\t"data":'+str(r)+'\n}'
        print s
        f.write(s)
        length = f.tell()
        f.seek(0)
        self.send_response(200)
        encoding = sys.getfilesystemencoding()
        self.send_header("Content-type", "text/html; charset=utf-8")
        self.send_header("Content-Length", str(length))
        self.end_headers()
        return f
    def do_GET(self):
        print self.path
        
        if self.path[1:len('data.json')+1] == 'data.json':
            f = self.mkDataJSON()
            if f:
                self.copyfile(f, self.wfile)
                f.close()
        else:
            SimpleHTTPServer.SimpleHTTPRequestHandler.do_GET(self);
def main(HandlerClass = MonitorHTTPServer,
         ServerClass = BaseHTTPServer.HTTPServer):
    BaseHTTPServer.test(HandlerClass, ServerClass)

if __name__ == '__main__':
    main()
    
