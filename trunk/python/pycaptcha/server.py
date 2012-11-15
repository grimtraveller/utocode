import BaseHTTPServer
import SimpleHTTPServer
import os
import sys
from bmp24 import *
try:
    from cStringIO import StringIO
except ImportError:
    from StringIO import StringIO
class CaptchaHTTPServer(SimpleHTTPServer.SimpleHTTPRequestHandler):
    def mkCaptcha(self):
        letters_idx = 'abcdefghijklmnopqrstuvwxyz0123456789'
        letters = {}
        for y in range(2):
            for x in range(18):
                letters[letters_idx[y*18+x]] = (x, y, x+1, y+1)
        bmp = Bmp24File()
        bmp.read('letters.bmp')

        letter_width = bmp.raster.width/18
        letter_height = bmp.raster.height/2
        for k in letters.keys():
            byte = ''
            left = letters[k][0] * letter_width
            top = letters[k][1] * letter_height
            right = letters[k][2] * letter_width
            bottom = letters[k][3] * letter_height
            letters[k] = bmp.raster.cut(left, top, right, bottom)
        import random
        captcha_num = 4
        captcha = random.sample(letters_idx, captcha_num)
        raster = Raster()
        for c in captcha:
            raster.combine(letters[c])
        self.send_response(200)
        self.send_header("Content-type", "image/bmp")
        data = str(Bmp24File(raster))
        size = len(data)
        self.send_header("Content-Length", str(size))
        self.end_headers()
        return data
    def do_GET(self):
        print self.path
        
        if self.path == '/captcha':
            f = StringIO()
            f.write(self.mkCaptcha())
            f.seek(0)
            if f:
                self.copyfile(f, self.wfile)
                f.close()
                return
        else:
            SimpleHTTPServer.SimpleHTTPRequestHandler.do_GET(self);
def main(HandlerClass = CaptchaHTTPServer,
         ServerClass = BaseHTTPServer.HTTPServer):
    BaseHTTPServer.test(HandlerClass, ServerClass)

if __name__ == '__main__':
    main()
    
