#!/usr/bin/env python
# -*- coding:utf-8 -*-
"""

Kukkaisvoima a lightweight weblog system.

Copyright (C) 2006-2012 Petteri Klemola

Kukkaisvoima is free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License version 3
as published by the Free Software Foundation.

Kukkaisvoima is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public
License along with Kukkaisvoima.  If not, see
<http://www.gnu.org/licenses/>.

"""

import cgi
import pickle
import os
from urllib import quote_plus, unquote_plus
from time import localtime, strptime, strftime
from sets import Set
from datetime import datetime, timedelta
import cgitb; cgitb.enable()
import smtplib
from email.MIMEText import MIMEText
import re
import locale
import random
import zlib
import logging
import sys
# kludge to get md5 hexdigest working on all python versions. Function
# md5fun should be used only to get ascii like this
# md5fun("kukkaisvoima").hexdigest()
try:
    from hashlib import md5 as md5fun
except ImportError: # older python (older than 2.5) does not hashlib
    import md5
    md5fun = md5.new

from blog_settings import *

# Language variables
l_archives = '日志'
l_categories = ' 标签'
l_comments = '评论'
l_comments2 = '评论'
l_date = '日期'
l_nextpage = '下一页'
l_previouspage = '上一页'
l_leave_reply = '留言'
l_no_comments_allowed = '不允许评论'
l_no_comments = '无评论'
l_name_needed = '姓名 (必填)'
l_email_needed = 'Email (必填)'
l_webpage = '网页'
l_no_html = '回复不允许使用html标签'
l_nospam_question = '2+3=?'
l_delete_comment = '删除评论'
l_passwd = '管理员密码'
l_admin = '管理'
l_admin_comments = '管理评论'
l_do_you_delete = '确定要删除评论吗?'
# new in version 8
l_search = "搜索"
l_search2 = "没有搜索到任何信息"
# new in version 10
l_recent_comments = "最近评论"
l_notify_comments= "通过电子邮件通知我有后续评论."
# new in version 11
l_read_more = "<p>更多...</p>"
# new in version 12
l_toggle = "点击年份显示月份"
#ZADD
l_no_comments_yet = "暂无评论"
l_submit = '提交'
l_name_must_be_filled_in = '姓名不能为空'
l_email_must_be_filled_in_and_must_be_valid = 'Email不能为空,且必须是有效的'
l_wrong_answer = '答案错误'
l_comment_cannot_be_empty = '回复内容不能为空'
l_show_more_categories = '更多标签'
l_home = '主页'
l_application = '应用'
l_about_me = '关于我'
l_what_is_my_name = '我的姓名(全拼)'

# version
version = '15b3TA'+'(addon by codepongo)'

# for date collisions
dates = {} #dates[datetime] = filename
datenow = datetime.now()
datenow_date = datenow.date()

def timeAgo(date):
    day_str = ""
    edate = date.date()
    if edate < datenow_date:
        days = (datenow_date - edate).days
        if days > 0:
            day_str += ", "
            years = days/365
            if years > 0:
                days = days%365
                if years == 1:
                    day_str += "1 year"
                else:
                    day_str += "%d years" % years
                if days > 0:
                    day_str += " and "
            if days == 1:
                day_str += "1 day"
            elif days > 1:
                day_str += "%d days" % days
            day_str += " ago"
    return day_str

def dateToString(date):
    return "%s%s" % (strftime(dateformat, date.timetuple()), timeAgo(date))

def generateDate(fileName):
    """generate file time"""
    """the time format is YYYY-MM-DD HH:MM:SS"""
    """date is from filename first"""
    """if time in filename is incrorrect, get the time from file status"""
    name, date, categories = fileName[:-4].split(delimiter)
    mode, ino, dev, nlink, uid, gid, size, atime, mtime, ctime = os.stat(fileName)
    filedate= datetime(*localtime(mtime)[0:6])
    date = "%s %s:%s:%s" % (date,
                            filedate.hour,
                            filedate.minute,
                            filedate.second)
    try:
        date = datetime(*strptime(
                date,'%Y-%m-%d %H:%M:%S')[0:6])
    except:
        date = filedate
    # if date collision happens add seconds to date
    if dates.has_key(date) and not dates[date] == fileName:
        while dates.has_key(date):
            date += timedelta(seconds=1)
    dates[date] = fileName
    return date

def sendEmail(to, subject, message):
    #need sendmail service is supported by server.
    msg = MIMEText(_text=wrapEmail(message), _charset='%s' % encoding)
    msg['subject'] = subject
    sender = '%s blog (%s) <%s>' % (blogname, baseurl, blogemail)
    s = smtplib.SMTP()
    s.connect()
    s.sendmail(sender, to, msg.as_string())
    s.close()

def wrapEmail(text):
    """Wrap some lines. Long words with no spaces are preserved."""
    lines = text.splitlines()
    newlines = list()
    for line in lines:
        if line == '':
            newlines.append('')
        while len(line) > 0:
            if len(line) < 73:
                newlines.append(line)
                line = ''
            else:
                nline = line.rfind(' ',0,72)
                if nline == -1:
                    newlines.append(line)
                    line = ''
                else:
                    nline = nline+1
                    newlines.append(line[:nline])
                    line = line[nline:]
    return '\n'.join(newlines)


def removeHtmlTags(line):
    """Removes html tags from line, works also for partial tags, so
    all < > will be removed.
    """
    while line.find("<") > -1 or line.find(">") > -1:
        # there are still tags
        start = line.find("<")
        end = line.find(">")
        if start > -1:
            # found start tag. Search for end
            tmpline = line[start+1:]
            end = tmpline.find(">")
            if end > -1:
                # found end, remove in between
                line = line[:start] + line[start+end+2:]
            else:
                # no end found remove until end of line
                line = line[:end]
        elif end > -1:
            # found > without < tag is open. remove start of the line
            line = line[end+1:]
    return line


def search(pattern, lines):
    matchlines = list()
    linenumber = 0
    for line in lines:
        m = pattern.search(line)
        # we don't want to process every line, so remove html
        # from only those lines that match our search
        if m:
            line = removeHtmlTags(line)
            # match again since the line has changed
            line = line.strip()
            m = pattern.search(line)
            if not m:
                continue
            # even the line out with ..starting match ending...
            linelength = 74
            startline = line[:m.start()]
            middleline = line[m.start():m.end()]
            endline = line[m.end():].rstrip()
            tokenlength = (linelength - len(middleline))/2

            if len(startline) >= tokenlength and len(endline) >= tokenlength:
                startline = startline[-tokenlength:]
                endline = endline[:tokenlength]
            elif len(startline) < tokenlength and len(endline) < tokenlength:
                pass
            elif len(startline) < tokenlength:
                endline = endline[:tokenlength + (tokenlength - len(startline))]
            elif len(endline) < tokenlength:
                actual_le = tokenlength + (tokenlength - len(endline))
                startline = startline[-actual_le:]

            startline = "..." + startline.lstrip()
            endline = endline.rstrip() + "..."

            matchlines.append("%04d: %s<div id=\"hit\">%s</div>%s\n" %(
                    linenumber,
                    startline,
                    middleline,
                    endline))
        linenumber += 1
    return matchlines


def genShortUrl(fileName):
    # With crc32 collasions will happen. When they do, just update the
    # fileName of the entry file manually. 0xffffffff is there to get
    # unsigned numbers
    num = (zlib.crc32(fileName) & 0xffffffff)
    # convert num to base 62 for the short url
    alp = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
    base = len(alp)
    buf = list()
    while (num > 0):
        buf.append(alp[num % base])
        num /= base
    buf.reverse()
    return "".join(buf)


shorturls = dict()
def getFileFromShortUrl(surl):
    global shorturls
    if len(shorturls) == 0:
        shorturlindex = open(os.path.join(indexdir,'shorturl.index'), 'rb')
        shorturls = pickle.load(shorturlindex)
        shorturlindex.close()
    return shorturls[surl]

class Comment:
    urlre = re.compile('(http|https|ftp)://([A-Za-z0-9/:@_%~#=&\.\-\?\+]+)')
    def __init__(self, author, email, url, comment, subscribe):
        self.author = author
        self.email = email
        self.url = url
        self.comment = comment
        self.date = datenow
        self.subscribe = subscribe
        random.seed()
        self.id = "%016x" % random.getrandbits(128)

    def getUrl(self):
        url = self.url
        if not url:
            return None
        if not url.startswith('http://'):
            url = 'http://%s' % url
        return url

    def getAuthorLink(self):
        url = self.getUrl()
        if url is None:
            return "%s" % self.author
        else:
            return "<a href=\"%s\"  rel=\"external nofollow\">%s</a>"\
                % (url, self.author)

    def getText(self):
        comment = str(self.comment)
        comment = self.comment.replace('\r\n','<br />')
        comment = self.urlre.sub(r'<a href="\1://\2">\1://\2</a>',
                                 comment)
        return comment

    def getFirstLine(self):
        return self.getText().split("<br />")[0]

    def getEmailMd5Sum(self):
        return md5fun(self.email.lower()).hexdigest()

    def getSubEmail(self):
        try:
            if self.subscribe is None:
                return None
            else:
                return self.email
        except:
            return None

    def getId(self):
        try:
            return self.id
        except:
            return None


def pickleComment(author, email, url, comment, filename, indexdir, subscribe):
    filename = filename.replace('/', '').replace('\\', '')
    filename = "%s.txt" % filename
    # read the old comments
    comments = list()
    try:
        oldcommentsfile = open(os.path.join(indexdir,'comment-%s' % filename), 'rb')
        comments = pickle.load(oldcommentsfile)
        oldcommentsfile.close()
    except:
        pass
    comm = Comment(author, email, url, comment, subscribe)
    comments.append(comm)
    commentfile = open(os.path.join(indexdir,'comment-%s' % filename), 'wb')
    pickle.dump(comments, commentfile)
    commentfile.close()
    updateCommentList()
    return comm


def getComments(filename):
    comments = list()
    comm_file = os.path.join(indexdir,'comment-%s' % filename)
    if os.path.exists(comm_file) is False:
        return comments
    try:
        oldcommentsfile = open(comm_file, 'rb')
        comments = pickle.load(oldcommentsfile)
        oldcommentsfile.close()
    except:
        pass
    return comments


def deleteComment(filename, commentnum):
    comments = getComments(filename)
    comments.pop(commentnum-1)
    commentfile = open(os.path.join(indexdir,'comment-%s' % filename), 'wb')
    pickle.dump(comments, commentfile)
    commentfile.close()
    updateCommentList()
    return


def unsubscribeComments(filename, unsubscribe_id):
    comments = getComments(filename)
    for comment in comments:
        if unsubscribe_id == comment.getId():
            comment.subscribe = None
    commentfile = open(os.path.join(indexdir,'comment-%s' % filename), 'wb')
    pickle.dump(comments, commentfile)
    commentfile.close()


def getCommentList():
    """Gets list of comments from the comment index"""
    commentlist = list()
    updated = False

    # generate list of comments if it does not exist
    if os.path.exists(os.path.join(indexdir,'recent_comments.index')) is False:
        updateCommentList()
        updated = True
    try:
        comindex = open(os.path.join(indexdir,'recent_comments.index'), 'rb')
        commentlist = pickle.load(comindex)
        comindex.close()
    except:
        pass

    # For shorturls force recent_comments.index update if there is no
    # shorturl generated for comment
    if not updated and shorturl and \
            len(commentlist) > 0 and commentlist[0].has_key("shorturl") is False:
        try:
            updateCommentList()
            comindex = open(os.path.join(indexdir,'recent_comments.index'), 'rb')
            commentlist = pickle.load(comindex)
            comindex.close()
        except:
            pass

    return commentlist


def updateCommentList():
    """Updates latest comments list"""
    commentlist = list()
    commentlist_tmp = list()

    for cfile in [x for x in os.listdir(indexdir) if x.startswith("comment-")]:
        cfile = cfile.replace("comment-", "", 1)
        num = 1
        comments = list()
        for cm in getComments(cfile):
            comments.append((cfile, cm, num))
            num += 1
        commentlist_tmp += comments
        # sort and leave 10 latests
        commentlist_tmp.sort(key=lambda com: com[1].date, reverse=True)
        commentlist_tmp = commentlist_tmp[:10]

    for c in commentlist_tmp:
        # get subject from commented entry
        entry = Entry(c[0], datadir)
        commentlist.append({"authorlink" : c[1].getAuthorLink(),
                            "file" : c[0],
                            "shorturl" : genShortUrl(c[0]),
                            "num" : c[2],
                            "author" : c[1].author,
                            "subject" : entry.headline})

    commentfile = open(os.path.join(indexdir,'recent_comments.index'), 'wb')
    pickle.dump(commentlist, commentfile)
    commentfile.close()


def getSubscribedEmails(comments):
    """Get list of email subscriptions from comment list. No
       duplicate email"""
    emails = dict()
    for com in comments:
        email = com.getSubEmail()
        if email:
            emails[email] = com.getId()
    return emails


def handleIncomingComment(fs):
    """Handles incoming comment and returns redirect location when
       successful and None in case of an error"""
    author = fs.getvalue('author')
    email = fs.getvalue('email')
    url = fs.getvalue('url')
    comment = fs.getvalue('comment')
    name = fs.getvalue('name')
    commentnum = fs.getvalue('commentnum')
    headline = fs.getvalue('headline')
    nospam = fs.getvalue('nospam')
    subscribe = fs.getvalue('subscribe')
    filename = "%s.txt" % name
    comments_for_entry = getComments(filename)

    # validate comment
    if not author:
        return None
    if not email:
        return None
    if not comment:
        return None
    if not name:
        return None
    if maxcomments == -1: # no comments allowed
        return None
    if len(comments_for_entry) >= maxcomments: # no more comments allowed
        return None
    if nospam != nospamanswer: # wrong answer
        return None

    # remove html tags
    comment = comment.replace('<','[')
    comment = comment.replace('>',']')

    # only one subscription per email address
    if subscribe and \
            email in getSubscribedEmails(comments_for_entry).keys():
        subscribe = None

    new_comment = \
        pickleComment(author, email, url, comment, name, indexdir, subscribe)

    comm_url = new_comment.getUrl()
    if comm_url:
        comm_url = "\nWebsite: %s" % comm_url
    else:
        comm_url = ""

    email_subject = 'New comment in %s' % headline
    email_body = 'Name: %s%s\n\n%s\n\nlink:\n%s/%s#comment-%s' \
        % (author, comm_url, comment, baseurl, name, commentnum)

    # notify blog owner and comment subscribers about the
    # new comment. Email sending may fail for some reason,
    # so try sending one email at time.
    try:
        email_body_admin = email_body
        if subscribe:
            email_body_admin += \
                "\n\nNote: commenter subscribed (%s) to follow-up comments" \
                % email
        sendEmail(blogemail, email_subject, email_body_admin)
    except:
        pass # TODO log errors, for now just fail silently

    # add disclaimer for subscribers
    email_body += \
        "\n\n******\nYou are receiving this because you have signed up for email notifications. "

    email_and_id = getSubscribedEmails(comments_for_entry)
    for subscribe_email in email_and_id.iterkeys():
        comm_id = email_and_id[subscribe_email]
        try:
            email_body_comment = email_body
            email_body_comment += \
                "Click here to unsubscribe instantly: %s/%s?unsubscribe=%s\n" \
                % (baseurl, name, comm_id)
            sendEmail(subscribe_email, email_subject, email_body_comment)
        except:
            pass # TODO log errors, for now just fail silently

    # redirect
    return 'Location: %s/%s#comment-%s\n' % (baseurl, name, commentnum)

class Entry:
    firstpre = re.compile("<p.*?(</p>|<p>)", re.DOTALL | re.IGNORECASE)
    whitespacere = re.compile("\s")
    def __init__(self, requestUrl, datadir):
        self.shorturl = False
        if shorturl and (len(requestUrl) == 5 or len(requestUrl) == 6):
            self.shorturl = requestUrl
            self.fileName = getFileFromShortUrl(requestUrl)
        elif requestUrl.endswith(".txt"):
            self.fileName = requestUrl
        else:
            self.fileName = "%s.txt" % requestUrl
        if shorturl and self.shorturl == False:
            self.shorturl = genShortUrl(self.fileName)
        self.fullPath = os.path.join(datadir, self.fileName)
        self.text = open(self.fullPath).readlines()
        # ZADD:  add filetype for no remove html tag flag
        #        if 'html' flag at end of file
        #        the html tag in file content is not removed.
        #        or the html tags are removed.
        self.filetype = self.text[-1][1:5]
        if self.filetype != 'html':
            self.filetype = 'txt'
        self.text = [line for line in self.text if not line.startswith('#')]
        self.headline = self.text[0]
        self.text = self.text[1:]
        self.author = defaultauthor
        self.cat = ''
        name, date, categories = self.fileName[:-4].split(delimiter)
        self.cat = categories.split(',')
        self.date = generateDate(self.fullPath)
        self.comments = getComments(self.fileName)
        if shorturl:
            self.url = "%s/%s" % (baseurl, self.shorturl)
        else:
            self.url = "%s/%s" % (baseurl,
                                  quote_plus(self.fileName[:-4]))

    def getFirstParagraph(self):
        # look for <p>
        text_as_str = "".join(self.text)
        m = self.firstpre.search(text_as_str)
        if m is not None:
            there_is_more = False
            there_is_more_str = "<a href=\"%s\">%s</a>" % \
                (self.url, l_read_more)
            first_paragraph = m.group().split("\n")
            text_as_str = re.sub(self.whitespacere, '', text_as_str)
            first_paragraph_as_string = re.sub(self.whitespacere, '', m.group())
            lastline = first_paragraph.pop()
            lastline.strip()
            if len(text_as_str) > len(first_paragraph_as_string):
                there_is_more = True
            if there_is_more and lastline.lower().endswith("</p>"):
                lastline = lastline[:-4] # removes the </p>
                there_is_more_str = there_is_more_str + "</p>"
            if there_is_more: # and tip that there is still more to this entry
                lastline = lastline + there_is_more_str
            first_paragraph.append(lastline)
            return first_paragraph
        else:
            return self.text

    def getText(self, summary=False):
        if summary == True:
            return self.getFirstParagraph()
        else:
            return self.text


class Entries:
    def __init__(self, indexdir):
        self.date = {}
        self.categories = {}
        self.indexdir = indexdir

    def add(self, entry):
        self.date[entry.date] = entry
        for cat in entry.cat:
            if self.categories.has_key(cat):
                self.categories[cat][entry.date] = entry
            else:
                self.categories[cat] = {}
                self.categories[cat][entry.date] = entry

    def getOne(self, name):
        x = list()
        x.append(Entry(name, datadir))
        return x

    def getMany(self, pagenumber=0, cat=None):
        indexfile = 'main.index'
        ents = list()
        if cat is not None:
            indexfile = '%s.index' % cat
        try:
            indexindexfile = open(os.path.join(self.indexdir, indexfile), 'rb')
            indexindex = pickle.load(indexindexfile)
            indexindexfile.close()
        except:
            return ents
        # load the files
        swd = indexindex.keys()
        swd.sort()
        swd.reverse()
        if pagenumber == -1: # no limit
            pass
        elif pagenumber > 0:
            sindex = numberofentriesperpage*pagenumber
            eindex = (numberofentriesperpage*pagenumber)+numberofentriesperpage
            swd = swd[sindex:eindex]
        else:
            swd = swd[:numberofentriesperpage]
        for key in swd:
            ents.append(Entry(indexindex[key], datadir))
        return ents

def renderHtmlFooter():
    print "<hr />"
    print "<div id=\"footer\">"
    print "Powered by <a href=\"http://23.fi/kukkaisvoima\">Kukkaisvoima</a> version %s" % version
    print "<br />"
    print "Hosting by <a href=\"http://bu3w.net\">Mencius</a>"
    print "</div>"
    print "</div>" # content1
    print "</body>"
    print "</html>"

def renderHtmlHeader(title=None, links=[]):
    print  "Content-Type: text/html; charset=%s\n" % encoding
    print doctype
    print "<html xmlns=\"http://www.w3.org/1999/xhtml\" xml:lang=\"%(lang)s\" lang=\"%(lang)s\">" % {'lang':language}
    print "<head>"
    if title:
        print "<title>%s | %s - %s</title>" % (title, blogname, slogan)
    else:
        print "<title>%s - %s </title>" % (blogname, slogan)
    print "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=%s\" />" % encoding
    print "<link rel=\"stylesheet\" href=\"%s\" type=\"text/css\" />" % stylesheet
    print "<link rel=\"shortcut icon\" href=\"%s\"/>" % favicon
    print "<link rel=\"alternate\" type=\"application/rss+xml\" title=\"%s RSS Feed\" href=\"%s/feed/\" />" % (blogname, baseurl)

    # print additional links
    for i in links:
        print i

    # Javascript. Used to validate comment form, nice eh :P
    print """
          <script type="text/javascript">
          /* <![CDATA[ */

          function toggle_years(id)
          {
              var elem = document.getElementById(id);
              if (elem.style.display != 'none')
              {
                  elem.style.display = 'none';
              }
              else
              {
                  elem.style.display = '';
              }
          }

          function toggle_categories(classname)
          {
              var elems = document.getElementsByClassName(classname);
              for (var i = 0; i < elems.length; i++)
              {
                  var elem = elems[i];
                  if (elem.style.display != 'none')
                  {
                      elem.style.display = 'none';
                  }
                  else
                  {
                      elem.style.display = '';
                  }
              }
          }

          function validate_not_null(field, msg)
          {
              if (field.value == null || field.value == "")
              {
                  alert(msg);
                  return false;
              }
              return true;
          }

          function validate_email(field, msg)
          {
              at_index = field.value.indexOf("@");
              if ((at_index > 1) && (field.value.lastIndexOf(".") > at_index))
              {
                  return true;
              }
              alert(msg);
              return false;
          }

          function validate_nospam(field, msg)
          {
              if (field.value == "%s")
              {
                  return true;
              }
              alert(msg);
              return false;
          }

          function validate_form(thisform)
          {
              with (thisform)
              {
                  if (validate_not_null(author, "%s") == false)
                  {
                      author.focus();
                      return false;
                  }
                  if (validate_email(email,"%s") == false)
                  {
                      email.focus();
                      return false;
                  }
                  if (validate_nospam(nospam, "%s") == false)
                  {
                      nospam.focus();
                      return false;
                  }
                  if (validate_not_null(comment, "%s") == false)
                  {
                      comment.focus();
                      return false;
                  }
              }
          }
          /* ]]> */
          </script>
    """ % (nospamanswer,
           l_name_must_be_filled_in,
           l_email_must_be_filled_in_and_must_be_valid,
           l_wrong_answer,
           l_comment_cannot_be_empty)
    print "</head>"
    print "<body>"

    print "<div id=\"content1\">"
    print "<div id=\"header\">"
    print "<h1><a href=\"%s\">%s</a></h1>" % (baseurl, blogname)
    print "<div id=\"slogan\">%s</div>" % slogan
    print '<div style="float:right;bottom:0">'
    print '<a href="%s">%s</a>|' % (baseurl, l_home)
    print '<a href="%s">%s</a>|' % (baseurl+'/application/', l_application)
    print '<a href="%s">%s</a>'  % (baseurl+'/aboutme', l_about_me)
    print "</div>"
    print "</div>" #header

def renderComment(entry, comment, numofcomment,
                  admin=False, pretext=False):
    print "<li>"
    if gravatarsupport:
        print "<img style=\"padding-right:5px;\""
        print "src=\"http://gravatar.com/avatar/%s?s=40&d=identicon\" align=\"left\"/>" % (
            comment.getEmailMd5Sum())
    print "<cite>%s</cite>:" % comment.getAuthorLink()
    print "<br />"
    delcom = ""
    if admin:
        delcom = "<a href=\"%s/%s/?delcomment=%s\">(%s)</a>" % \
            (baseurl,
             quote_plus(entry.fileName[:-4]),
             numofcomment,
             l_delete_comment)
    print "<small><a name =\"comment-%s\" href=\"%s#comment-%s\">%s</a> %s </small>" % \
        (numofcomment,
         entry.url,
         numofcomment,
         dateToString(comment.date),
         delcom)
    if pretext:
        print pretext
    else:
        print "<p>%s</p>" % comment.getText()
    print "</li>"


def renderEntryLinks(entries, text=None, comment_tuple_list=None):
    # renders also some comments for search results
    for entry in entries:
        link = "<li><a href=\"%s\">%s</a>" % (
            entry.url, entry.headline)
        index = 1
        for cat in entry.cat:
            link += "%s" % cat
            if index != len(entry.cat):
                link += ", "
            index += 1
        link += " (%s)" % entry.date
        if text:
            link += "<br /><pre>%s</pre>" % text
        print link
        if comment_tuple_list:
            print "<ol style=\"list-style-type:none;\">"
            numofcomment = 0
            for comment, ctext, author in comment_tuple_list:
                numofcomment = numofcomment +1
                if len(ctext) == 0 and len(author) == 0:
                    continue
                if len(ctext) == 0 and len(author) > 0:
                    comm_text = comment.getFirstLine()
                    comm_text = removeHtmlTags(comm_text)
                    three_dots = ""
                    if len(comm_text) > (60):
                        three_dots = "..."
                    ctext = "<p>%s%s</p>" % (comm_text[:60], three_dots)
                elif len(ctext) > 0:
                    ctext = "<pre>%s</pre>" % ctext
                renderComment(entry, comment, numofcomment, False, ctext)
            print "</ol>"
        print "</li>"


def renderCategories(catelist, ent, path):
    renderHtmlHeader("archive")
    print "<div id=\"content3\">"

    if len(path) == 1 and path[0] == "categories":
        sortedcat = catelist.keys()
        try:
            sortedcat.sort(key=locale.strxfrm)
        except: # python < 2.4 fails
            sortedcat.sort()
        print "<h2>%s</h2>" % l_categories
        print "<ul>"

        for cat in sortedcat:
            print "<li><a href=\"%s/%s\">%s</a> (%s)</li>" % (
                baseurl, quote_plus(cat), cat, len(catelist[cat]))
            print "<ul>"
            renderEntryLinks(ent.getMany(-1, cat))
            print "</ul>"

        print "</ul>"
    elif len(path) == 2 and path[1] in catelist.keys():
            print "<h2>%s</h2>" % path[1]
            renderEntryLinks(ent.getMany(-1, path[1]))

    print "</div>" # content3
    renderHtmlFooter()
    return


def renderArchive(ent):
    entries = ent.getMany(-1)
    renderHtmlHeader(l_archives)
    print "<div id=\"content3\">"

    print "<h2>%s (%d)</h2>" % (l_archives, len(entries))
    print "<ul>"
    renderEntryLinks(entries)
    print "</ul>"

    print "</div>" # content3
    renderHtmlFooter()
    return


def renderSearch(entries, searchstring):
    renderHtmlHeader(l_search)
    print "<div id=\"content3\">"

    # Remove some special character so that one don't exhaust the web
    # host with stupid .*? searches
    for i in ".^$*+?{[]\|()":
        searchstring = searchstring.replace(i,"")

    pattern = re.compile(searchstring, re.IGNORECASE)
    matchedfiles = {}
    for entry in entries:
        # first search in the entry
        matchedfiles[entry] = dict()
        matchedfiles[entry]["lines"] = search(pattern, entry.getText())
        matchedfiles[entry]["headline"] = search(pattern, [entry.headline])
        # then from the entry's comments
        matchedfiles[entry]["comments"] = dict()
        comments_matches = False
        for comment in entry.comments:
            mlines = search(pattern, comment.comment.splitlines())
            author = search(pattern, comment.author.splitlines())
            if len(mlines) > 0 or len(author) > 0:
                comments_matches = True
            matchedfiles[entry]["comments"][comment] = dict()
            matchedfiles[entry]["comments"][comment]["lines"] = mlines
            matchedfiles[entry]["comments"][comment]["author"] = author
        if len(matchedfiles[entry]["lines"]) == 0 and \
                len(matchedfiles[entry]["headline"]) == 0 and \
                comments_matches == False:
            # remove entries with no matches in text or in comments
            del(matchedfiles[entry])

    for entry in matchedfiles.iterkeys():
        com_list = list()
        for comment in matchedfiles[entry]["comments"].iterkeys():
            pline = ""
            for line in matchedfiles[entry]["comments"][comment]["lines"]:
                pline += line
            com_list.append((comment, pline,
                             matchedfiles[entry]["comments"][comment]["author"]))
        pline = ""
        for line in matchedfiles[entry]["lines"]:
            pline += line
        renderEntryLinks([entry], pline, com_list)

    if len(matchedfiles) == 0: # no matches
        print l_search2

    print "</div>" # content3
    renderHtmlFooter()
    return

def renderDeleteComments(entry, commentnum):
    renderHtmlHeader("comments")
    print "<div id=\"content3\">"
    comments = entry.comments

    if len(comments) < commentnum:
        print "<p>No comment</p>"
        print "</body></html>"
        return
    comment = comments[commentnum-1]
    print "<ol>"
    print "<li>"
    print "<cite>%s</cite>:" % comment.getAuthorLink()

    print "<br />"
    print "<small>%s</small>" % (dateToString(comment.date))
    print "<p>%s</p>" % comment.getText()
    print "</li>"
    print "</ol>"

    print "<p>%s</p>" % l_do_you_delete
    print "<form action=\"%s/%s/?deletecomment\" method=\"post\" id=\"deleteform\">" % (baseurl,
                                                                                         quote_plus(entry.fileName[:-4]))
    print "<input type=\"hidden\" name=\"commentnum\" id=\"commentnum\" value=\"%s\"/>" % (commentnum)
    print "<input type=\"hidden\" name=\"name\" id=\"name\" value=\"%s\"/>" % entry.fileName[:-4]
    print "<p><input type=\"password\" name=\"password\" id=\"password\" size=\"22\" tabindex=\"1\" />"
    print "<label for=\"password\"><small>%s</small></label></p>" % l_passwd
    print "<p><input name=\"submit\" type=\"submit\" id=\"submit\" tabindex=\"5\" value=\"%s\" />" % (l_submit)
    print "</p></form>"
    print "</div>" # content3
    renderHtmlFooter()
    return

def renderSidebarAlipay():
    print "<br />"
    print "<a href='http://me.alipay.com/codepongo'><img src='https://img.alipay.com/sys/personalprod/style/mc/btn-index.png' /></a>"
    print "<br />"
def renderSidebarCategories(catelist, rss_categories):
    categories = catelist.keys()
    try:
        categories.sort(key=locale.strxfrm)
    except: # python < 2.4 fails
        categories.sort()
    print "<h2><a href=\"%s/categories\">%s</a></h2>" % (baseurl, l_categories)

    hide_cat_str = ""
    topcategories = list()
    if len(categories) > 5:
        print "<a href=\"#\" onclick=\"toggle_categories('tcategory'); return false;\">%s</a>" % (l_show_more_categories)
        topcategories = categories[:]
        topcategories.sort(key=lambda cat: len(catelist[cat]), reverse=True)
        topcategories = topcategories[:5]
        for cat in rss_categories:
            if cat not in topcategories:
                topcategories.append(cat)
        hide_cat_str = " class=\"tcategory\" style=\"display:none;\""

    print "<ul>"
    for cat in categories:
        add_str = ""
        if len(topcategories) > 0 and cat not in topcategories:
            add_str = hide_cat_str
        print "<li%s><a href=\"%s/%s\">%s</a> (%s)" % (
            add_str, baseurl, quote_plus(cat), cat, len(catelist[cat]))
        if cat in rss_categories:
            print "<a href=\"%s/%s/feed\"><img alt=\"RSS Feed Icon\" src=\"%s\" style=\"vertical-align:top; border:none;\"/></a>" % \
                (baseurl, cat, feedicon)
        print "</li>"
    print "</ul>"

def renderSidebarSearch():
    print "<h2>%s</h2>" % l_search
    print "<form action=\"%s\" method=\"get\" id=\"searchform\">" % baseurl
    print "<input type=\"text\" name=\"search\" id=\"search\" size=\"15\" /><br />"
    print "<input type=\"submit\" value=\"%s\" />" % l_search
    print "</form>"

def renderSidebarCommments():
    if sidebarcomments:
        print "<h2>%s</h2>" % l_recent_comments
        comlist = getCommentList()
        if len(comlist) == 0:
            print l_no_comments_yet
        else:
            print "<ul>"
            for com in comlist:
                if shorturl:
                    entryurl = com["shorturl"]
                else:
                    entryurl = quote_plus(com["file"][:-4])
                print "<li>%s on <a href=\"%s/%s#comment-%d\">%s</a>"\
                    % (com["author"], baseurl, entryurl,
                       com["num"], com["subject"])
                print "</li>"
            print "</ul>"

def renderSidebarArchive(arclist):
    print "<h2><a href=\"%s/archive\">%s</a> (%d)</h2>" % \
        (baseurl, l_archives,
         # total number of entries
         sum([len(l) for l in [i for i in arclist.itervalues()]]))
    print l_toggle
    print "<ul>"
    sortedarc = arclist.keys()
    sortedarc.sort()
    sortedarc.reverse()

    # get years from archive and sort them
    years = dict()
    for d in sortedarc:
        year = d.split("-", 1)[0]
        if years.has_key(year) is False:
            years[year] = list()
        years[year].append(d)
    years_keys = years.keys()
    years_keys.sort()
    years_keys.reverse()

    # display each year at top lovel and if visiability is toggled
    # then show months
    for year in years_keys:
        print "<li><a href=\"#\" onclick=\"toggle_years('con-year-%s'); return false;\">%s</a> (%d)" %\
            (year, year,
             # number of entries per year
             sum([len(arclist[dat]) for dat in years[year]]))

        print "<ul id=\"con-year-%s\" style=\"display:none;\">" % year
        for dat in years[year]:
            print "<li><a href=\"%s/%s\">%s</a> (%s)</li>" % (
                baseurl, dat, dat, len(arclist[dat]))
        print "</ul></li>"
    print "</ul>"

def renderSidebarAdmin(entries):
    if len(entries) == 1:
        print "<h2>%s</h2>" % l_admin
        print "<ul>"
        print "<li><a href=\"%s/%s/?admin\" rel=\"nofollow\">%s</a>" % \
            (baseurl,
             quote_plus(entries[0].fileName[:-4]),
             l_admin_comments)
        print "</ul>"

def renderHtml(entries, path, catelist, arclist, admin, page):
    """Render the blog. Some template stuff might be nice :D"""
    categories = list()
    if len(path) >= 1 and path[0] in catelist.keys():
        categories.append(path[0])
    elif len(entries) == 1:
        categories = entries[0].cat

    # title
    title = None
    summary = False
    if len(entries) == 1:
        title = entries[0].headline
    elif len(categories) == 1:
        title = categories[0]

    if len(entries) > 1:
        summary = entrysummary

    rss = list()
    # additional rss feeds
    for cat in categories:
        rss.append("<link rel=\"alternate\" type=\"application/rss+xml\" title=\"%s: %s RSS Feed\" href=\"%s/%s/feed/\" />" % \
                       (blogname,cat,baseurl,quote_plus(cat)))

    renderHtmlHeader(title, rss)
    print "<div id=\"content2\">"
    for entry in entries:
        print "<h2><a href=\"%s\">%s</a></h2>" % (
            entry.url,
            entry.headline)
        print "<div class=\"post\">"
        for line in entry.getText(summary):
            if entry.filetype != 'html':
                print line.replace('&', '&amp;').replace(' ', '&nbsp;').replace('<', '&lt;').replace('>', '&gt;').replace('"', '&quot;').replace('\n', '<br />')
            else:
                print line
        print "</div>"

        if len(entries) > 1 and maxcomments > -1:
            nc = len(entry.comments)
            if nc > 0:
                print "<div class=\"comlink\">%s <a href=\"%s#comments\">%s</a></div>" % (
                    nc,
                    entry.url,
                    l_comments2)
            else:
                print "<div class=\"comlink\"><a href=\"%s#leave_acomment\">%s</a></div>" % (
                    entry.url,
                    l_no_comments)
        print "<div class=\"categories\">%s:" % l_categories
        num = 0
        for cat in entry.cat:
            num=num+1
            comma = ''
            if len(entry.cat) > num:
                comma = ', '
            print "<a href=\"%s/%s\">%s</a>%s" % (baseurl, quote_plus(cat), cat, comma)
        print "</div>"
        print "<div class=\"date\">%s: %s</div>" % \
            (l_date, dateToString(entry.date))

        # comments
        if len(entries) == 1:
            numofcomment = 0
            if len(entry.comments) > 0 and maxcomments > -1:
                print "<h3><a name=\"comments\"></a>%s</h3>" % l_comments
                print "<ol style=\"list-style-type:none;\">"
                for comment in entry.comments:
                    numofcomment = numofcomment +1
                    renderComment(entry, comment, numofcomment, admin)
                print "</ol>"
            if maxcomments == -1 or len(entry.comments) >= maxcomments:
                print "<h3>%s</h3>" % l_no_comments_allowed
            else:
                print "<h3><a name=\"leave_acomment\"></a>%s</h3>" % l_leave_reply
                print "<form action=\"%s/%s/?postcomment\" method=\"post\"" % (
                    baseurl,
                    entry.fileName[:-4])
                print "id=\"commentform\" onsubmit=\"return validate_form(this)\">" # form
                print "<input type=\"hidden\" name=\"name\" id=\"name\" value=\"%s\"/>" % entry.fileName[:-4]
                print "<input type=\"hidden\" name=\"headline\" id=\"headline\" value=\"%s\"/>" % entry.headline
                print "<input type=\"hidden\" name=\"commentnum\" id=\"commentnum\" value=\"%s\"/>" % (numofcomment+1)
                print "<p><input type=\"text\" name=\"author\" id=\"author\" size=\"22\" tabindex=\"1\" />"
                print "<label for=\"author\"><small>%s</small></label></p>" % l_name_needed
                print "<p><input type=\"text\" name=\"email\" id=\"email\" size=\"22\" tabindex=\"2\" />"
                print "<label for=\"email\"><small>%s</small></label></p>" % l_email_needed
                print "<p><input type=\"text\" name=\"url\" id=\"url\" size=\"22\" tabindex=\"3\" />"
                print "<label for=\"url\"><small>%s</small></label></p>" % l_webpage
                print "<p><input type=\"text\" name=\"nospam\" id=\"nospam\" size=\"22\" tabindex=\"4\" />"
                print "<label for=\"nospam\"><small>%s</small></label></p>" % l_nospam_question
                print "<p>%s</p>" % l_no_html
                print "<p><textarea name=\"comment\" id=\"comment\" cols=\"40\" rows=\"7\" tabindex=\"4\"></textarea></p>"
                print "<p><input name=\"submit\" type=\"submit\" id=\"submit\" tabindex=\"5\" value=\"%s\" />" % (l_submit)
                print "<input type=\"hidden\" name=\"comment_post_ID\" value=\"11\" />"
                print "</p>"
                print "<p><input type=\"checkbox\" name=\"subscribe\" id=\"subscribe\" tabindex=\"6\" value=\"subscribe\">%s</label></p>" % l_notify_comments
                print "</form>"

    if len(entries) > 1:
        print "<div class=\"navi\">"
        if page > 0:
            print "<a href=\"%s/%s?page=%s\">%s</a>" % (
                baseurl,
                '/'.join(path),
                page-1,
                l_previouspage
                )
        if len(entries) == numberofentriesperpage:
            print "<a href=\"%s/%s?page=%s\">%s</a>" % (
                baseurl,
                '/'.join(path),
                page+1,
                l_nextpage
                )
        print "</div>"
    print "</div>" # content2

    # sidebar
    print "<div id=\"sidebar\">"
    print "<a href=\"%s/feed\">RSS <img alt=\"RSS Feed Icon\" src=\"%s\" style=\"vertical-align:top; border:none;\"/></a>" % \
        (baseurl, feedicon)

    renderSidebarCategories(catelist, categories)
    renderSidebarSearch()
    renderSidebarCommments()
    renderSidebarArchive(arclist)
    renderSidebarAdmin(entries)
    renderSidebarAlipay()

    print "</div>" # sidebar

    renderHtmlFooter()

def renderFeed(entries, path, categorieslist):
    rfc822time = "%a, %d %b %Y %H:%M:%S +0200"
    print "Content-Type: text/xml; charset=%s\n" % encoding
    print "<?xml version=\"1.0\" encoding=\"%s\"?>" % encoding
    print "<!-- generator=\"Kukkaisvoima version %s\" -->" % version
    print "<rss version=\"2.0\""
    print "xmlns:content=\"http://purl.org/rss/1.0/modules/content/\""
    print "xmlns:wfw=\"http://wellformedweb.org/CommentAPI/\""
    print "xmlns:dc=\"http://purl.org/dc/elements/1.1/\""
    print ">"
    print "<channel>"
    if len(path) >= 1 and path[0] in categorieslist.keys():
        print "<title>%s: %s</title>" % (blogname, path[0])
    else:
        print "<title>%s</title>" % blogname
    print "<link>%s</link>" % baseurl
    print "<description>%s</description>" % description
    print "<pubDate>%s</pubDate>" % strftime(rfc822time, entries[0].date.timetuple())
    print "<lastBuildDate>%s</lastBuildDate>" % strftime(rfc822time, entries[0].date.timetuple())
    print "<generator>http://23.fi/kukkaisvoima/</generator>"
    print "<language>%s</language>" % language

    # print entries
    for entry in entries:
        print "<item>"
        print "<title>%s</title>" % entry.headline
        print "<link>%s</link>" % entry.url
        print "<comments>%s#comments</comments>" % entry.url
        print "<pubDate>%s</pubDate>" % strftime(rfc822time, entry.date.timetuple())
        print "<dc:creator>%s</dc:creator>" % entry.author
        for cat in entry.cat:
            print "<category>%s</category>" % cat
        print "<guid isPermaLink=\"false\">%s/</guid>" % entry.url
        print "<description><![CDATA[ %s [...]]]></description>" % entry.text[0]
        print "<content:encoded><![CDATA["
        for line in entry.text:
            print line,
        print "]]></content:encoded>"
        print "<wfw:commentRss>%s/feed/</wfw:commentRss>" % entry.url
        print "</item>"
    print "</channel>"
    print "</rss>"

def renderResume():
    renderHtmlHeader("resume")
    print "<div id=\"content3\">"
    text = open(datadir+os.sep+'resume.txt').readlines()
    for line in text:
        print line
    print "</div>" # content3
    renderHtmlFooter()
    return

def renderResumeCheck():
    renderHtmlHeader("resume")
    print "<div id=\"content3\">"
    print "<form action=\"%s/resume\" method=\"post\" id=\"deleteform\">" % (baseurl)
    print "<p><label for=\"password\"><small>%s</small></label>" % l_what_is_my_name
    print "<p><input type=\"password\" name=\"password\" id=\"password\" size=\"22\" tabindex=\"1\" />"
    print "<p><input name=\"submit\" type=\"submit\" id=\"submit\" tabindex=\"5\" value=\"%s\" />" % (l_submit)
    print "</p></form>"
    print "</div>" # content3
    renderHtmlFooter()
    return

def renderWhatsMyUserAgent():
    renderHtmlHeader("what's my user-agent?")
    if os.environ.has_key('HTTP_USER_AGENT'):
        print '<div id="content3">'
        print os.environ['HTTP_USER_AGENT']
        print '</div>' # content3
    renderHtmlFooter()
def renderAboutMe():
    renderHtmlHeader('about me')
    print "<div id=\"content3\">"
    text = open(datadir+os.sep+'aboutme.txt').readlines()
    for line in text:
        print line
    print "</div>" # content3
    renderHtmlFooter()

def generateShortUrlIndex(filelist):
    # Pickle the shorturl index
    shortdict = dict()
    for f in filelist:
        sfile = filelist[f]
        shortdict[genShortUrl(sfile)] = sfile
    shortindex = open(os.path.join(indexdir,'shorturl.index'), 'wb')
    pickle.dump(shortdict, shortindex)
    shortindex.close()
# main program starts here
def main():
    path = ['']
    #directory
    #http://codepongo.com/blog/archive
    #"/archive" is in os.environ
    if os.environ.has_key('PATH_INFO'):
        path = os.environ['PATH_INFO'].split('/')[1:]
        path = [p for p in path if p != '']
    page = 0
    admin = False
    delcomment = 0
    postcomment = False
    deletecomment = False
    search = False
    searchstring = ""
    unsubscribe = False
    unsubscribe_id = ""

    # parse http request
    if os.environ.has_key('QUERY_STRING'):
        querystr = os.environ['QUERY_STRING'].split('=')
        #http://codepongo.com/blog/xxx?page=0
        if len(querystr) == 2 and querystr[0] == 'page':
            try:
                page = int(querystr[1])
            except:
                page = 0
        #http://codepongo.com/blog/xxx?admin
        elif querystr[0] == 'admin':
            admin = True
        #http://codepongo.com/blog/xxx?postcomment
        elif querystr[0] == 'postcomment':
            postcomment = True
        #http://codepongo.com/blog/xxx?deletecomment
        elif querystr[0] == 'deletecomment':
            deletecomment = True
        #http://codepongo.com/blog/xxx/?delcomment=0
        elif len(querystr) == 2 and querystr[0] == 'delcomment':
            try:
                delcomment = int(querystr[1])
            except:
                delcomment = 0
        #http://codepongo.com/blog?search=xxx
        elif len(querystr) == 2 and querystr[0] == 'search':
            search = True
            searchstring = querystr[1]
        #http://codepongo.com/blog/xxx/?unsubscrib=0
        elif len(querystr) == 2 and querystr[0] == 'unsubscribe':
            unsubscribe = True
            unsubscribe_id = querystr[1]

    # entries
    files = os.listdir(datadir)
    # read and validate the txt files
    entries = list()    #article list
    for entry in files:
        if not entry.endswith(".txt"):
            continue
        if not len(entry.split(delimiter)) == 3:
            continue
        try:
            year, month, day = entry.split(delimiter)[1].split("-")
            if int(year) == 0 or \
                    (int(month) < 1 or int(month) > 12) or \
                    (int(day) < 1 or int(day) > 31):
                continue
        except:
            continue
        entries.append(entry)

    filelist = {}   #article dictionary filelist[date] = article
    for file in entries:
        filelist[generateDate(os.path.join(datadir,file))] = file

    # Read the main index
    indexold = list()
    try:
        indexoldfile = open(os.path.join(indexdir,'main.index'), 'rb')
        indexoldd = pickle.load(indexoldfile)
        indexoldfile.close()
        indexold = indexoldd.values()
    except:
        pass

    if shorturl and \
            os.path.exists(os.path.join(indexdir,'shorturl.index')) is False:
        generateShortUrlIndex(filelist)

    # generate categorieslist and archivelist
    categorieslist = {}
    archivelist = {}
    for file in filelist:
        name, date, categories = filelist[file][:-4].split(delimiter)
        adate = date[:7] #YYYY-MM
        if adate.endswith('-'):
            adate =  "%s-0%s" % (adate[:4], adate[5])
        date = file
        categories = categories.split(',')
        for cat in categories:
            if categorieslist.has_key(cat):
                categorieslist[cat][date] = filelist[file]
            else:
                categorieslist[cat] = {}
                categorieslist[cat][date] = filelist[file]
        if archivelist.has_key(adate):
            archivelist[adate][date] = filelist[file]
        else:
            archivelist[adate] = {}
            archivelist[adate][date] = filelist[file]

    # Compare the index
    newarticles = Set(entries)^Set(indexold)
    if len(newarticles) > 0:
        # Pickle the categories
        for cat in categorieslist.keys():
            oldcategorieslist = None
            try:
                oldcatindex = open(os.path.join(indexdir,'%s.index' %cat), 'rb')
                oldcategorieslist = pickle.load(oldcatindex)
                oldcatindex.close()
            except:
                pass # :P
            # No old index or new articles in category, update the index
            if not oldcategorieslist or \
                    (oldcategorieslist and \
                         len(Set(oldcategorieslist.values())\
                                 ^Set(categorieslist[cat].values())) > 0):
                catindex = open(os.path.join(indexdir,'%s.index' %cat), 'wb')
                pickle.dump(categorieslist[cat], catindex)
                catindex.close()

        # Pickle the date archives
        for arc in archivelist.keys():
            oldarchivelist = None
            try:
                oldarcindex = open(os.path.join(indexdir,'%s.index' %arc), 'rb')
                oldarchivelist = pickle.load(oldarcindex)
                oldarcindex.close()
            except:
                pass # :P
            if not oldarchivelist or \
                    (oldarchivelist and \
                         len(Set(oldarchivelist.values())\
                                 ^Set(archivelist[arc].values())) > 0):
                arcindex = open(os.path.join(indexdir,'%s.index' %arc), 'wb')
                pickle.dump(archivelist[arc], arcindex)
                arcindex.close()
        # Pickle the main index
        index = open(os.path.join(indexdir,'main.index'), 'wb')
        pickle.dump(filelist, index)
        index.close()
       # Pickle the shorturl index
        generateShortUrlIndex(filelist)

    feed = False
    #http://codepongo.com/blog/feed
    #http://codepongo.com/blog/xxx/feed
    if len(path) > 0 and path[len(path)-1] == 'feed':
        feed = True
        numberofentriesperpage = 10 # feed always has 10
        page = 0

    ent = Entries(indexdir)

    #http://codepongo.com/blog/archive
    if len(path) == 1 and path[0] == "archive":
        return renderArchive(ent)
    #http://codepongo.com/blog/categories
    #http://codepongo.com/blog/categories/xxx
    if len(path) >= 1 and path[0] == "categories":
        return renderCategories(categorieslist, ent, path)
    #http://codepongo.com/blog?search=xxx
    elif len(path) == 1 and search == True and searchstring != "":
        return renderSearch(ent.getMany(-1), unquote_plus(searchstring))
    #http://codepongo.com/blog/xxx xxx in categories
    elif len(path) >= 1 and path[0] in categorieslist.keys():
        try:
            entries = ent.getMany(page, path[0])
        except:
            entries = ent.getMany(page)
    #http://codepongo.com/blog/xxx xxx in archive
    elif len(path) == 1 and path[0] in archivelist.keys():
        try:
            entries = ent.getMany(page, path[0])
        except:
            entries = ent.getMany(page)
    #http://codepongo.com/blog/xxx/?postcomment
    elif len(path) == 1 and postcomment:
        try:
            redirect = handleIncomingComment(cgi.FieldStorage(keep_blank_values=1))
            if redirect:
                print redirect
                return
            else:
                entries = ent.getOne(unquote_plus(path[0]))
        except:
            entries = ent.getMany(page)
    #http://codepongo.com/blog/xxx?deletecomment
    elif len(path) == 1 and deletecomment:
        # check if this is incoming comment
        fs = cgi.FieldStorage(keep_blank_values=1)
        commentnum = int(fs.getvalue('commentnum'))
        password = fs.getvalue('password')
        name = fs.getvalue('name')
        filename = "%s.txt" % name
        if commentnum and name and password == passwd and passwd != 'password':
            deleteComment(filename, commentnum)
        print 'Location: %s/%s\n' % (baseurl, name)
    #http://codepongo.com/blog/xxx/?unsubscrib=0
    elif len(path) == 1 and unsubscribe and unsubscribe_id:
        name = unquote_plus(path[0])
        filename = "%s.txt" % name
        unsubscribeComments(filename, unsubscribe_id)
        print 'Location: %s/%s#comments\n' % (baseurl, name)
    elif len(path) == 1 and path[0] == 'aboutme':
        return renderAboutMe()
    elif len(path) == 1 and path[0] == 'whatsmyuseragent':
        return renderWhatsMyUserAgent()
    elif len(path) == 1 and path[0] == 'resume':
        if querystr[0] == 'check':
            return renderResumeCheck()
        else:
            fs = cgi.FieldStorage(keep_blank_values=1)
            password = fs.getvalue('password')
            if password == 'zuohaitao':
                return renderResume()
            else:
                return renderResumeCheck()
    #http://codepongo.com/blog/xxx
    elif len(path) == 1:
        try:
            entries = ent.getOne(unquote_plus(path[0]))
        except:
            entries = ent.getMany(page)
    else:
        entries = ent.getMany(page)

    #http://codepongo.com/blog/xxx/?delcomment=0
    if delcomment > 0 and len(entries) == 1:
        renderDeleteComments(entries[0], delcomment)
    #http://codepongo.com/blog/feed
    #http://codepongo.com/blog/xxx/feed
    elif feed:
        renderFeed(entries, path, categorieslist)
    else:
        renderHtml(entries, path, categorieslist, archivelist, admin, page)
if __name__ == "__main__":
    main()

