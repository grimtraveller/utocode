import sys
import os
import xmlrpclib
class MetaWeblog:
    def __init__(self, url, usr, passwd):
        self.user = usr
        self.password = passwd
        self.server = xmlrpclib.ServerProxy(url)
        self.userid = self.getUserBlogs()[0]['blogid']

    def getUserId(self):
        return self.userid
    def newPost(self, title, description, category='default'):
        blog = self.getRecentPosts('1')[0]
        blog['title'] = title
        blog['content'] = description
        blog['categories'][0] = category
        return self.server.metaWeblog.newPost(self.userid, self.user, self.password, blog, True)
    def getPost(self, postid=''):
        return self.server.metaWeblog.getPost(postid, self.user, self.password)
    def getRecentPosts(self, count='9999'):
        return self.server.metaWeblog.getRecentPosts('', self.user, self.password, count)
    def getUserBlogs(self):
        return self.server.blogger.getUsersBlogs('', self.user, self.password)
if __name__ == '__main__':
    if len(sys.argv) != 4:
        print 'usage:', sys.argv[0], 'username password file'
        exit(0)
    blog = MetaWeblog('http://blog.chinaunix.net/xmlrpc.php', sys.argv[1], sys.argv[2])
    if os.path.isfile(sys.argv[3]):
        title = sys.argv[3]
        content = open(sys.argv[3], 'rb').read()
        content = content.replace('\r', '')
        content = content.replace('\n', '<br>')
        content = content.replace(' ', '&nbsp;')
        content = content.replace('\t', '&nbsp;&nbsp;&nbsp;&nbsp;')
        if sys.platform == 'win32':
            blog.newPost(title, content.decode('gbk').encode('utf8'))
    else:
        print sys.argv[3], 'is not exist'
