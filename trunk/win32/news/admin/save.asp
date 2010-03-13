<!-- #include file = "conn.asp" -->
<%
id = request.querystring("id")
title = request.form("title")
name = request.form("name")
content = request.form("content")
set rs = server.createobject("adodb.recordset")
if 0 <> StrComp(id, "") then
	sql = "select * from news where id="+id
else
	sql = "select * from news where id=0"
end if

rs.open sql,db,1,3
if Not (0 <> StrComp(id, "")) then
	rs.addnew
end if
rs("title") = trim(request.form("title"))
rs("name") = trim(request.form("name"))
rs("content") = server.htmlencode(request.form("content")) '字符串转化成html代码
rs("count") = 0
rs("time") = now()
rs.update
rs.close
set rs = nothing
response.redirect "ls.asp"
%>
