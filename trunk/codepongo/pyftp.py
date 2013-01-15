#!/bin/python
import sys
from pyftpdlib import ftpserver
import markdown2
import os
class Handler(ftpserver.FTPHandler):
    def on_file_received(self, file):
        name, suffix = os.path.splitext(file)
        if '.md' == suffix:
            md = None
            with open(file, 'rb') as r:
                md = r.read()
                r.close()
            with open(name + '.txt', 'wb') as f:
                html = markdown2.markdown(md)
                html = html.replace('<img src="', '<img src="/')
                f.write(html)
                f.write('\n#html')
                f.close()
authorizer = ftpserver.DummyAuthorizer()
authorizer.add_user(sys.argv[1], sys.argv[2], sys.argv[3], perm="elradfmw")
handler = Handler
handler.authorizer = authorizer
handler.timeout = 0
address = ("", 21)
ftpd = ftpserver.FTPServer(address, handler)
ftpd.serve_forever()
