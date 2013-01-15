#!/bin/python
import sys
from pyftpdlib import ftpserver
class Handler(ftpserver.FTPHandler):
    def on_file_received(self, file):
        print file
        pass
authorizer = ftpserver.DummyAuthorizer()
authorizer.add_user(sys.argv[1], sys.argv[2], sys.argv[3], perm="elradfmw")
handler = Handler
handler.authorizer = authorizer
handler.timeout = 0
address = ("", 21)
ftpd = ftpserver.FTPServer(address, handler)
ftpd.serve_forever()
