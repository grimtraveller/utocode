<!-- #include file = "conn.asp" -->

<%
dim title
dim name
dim content
id = request.querystring("id")
%>
<%
if IsEmpty(id) then
	title = ""
	name = ""
	content = ""
else
	set rs = server.createobject("adodb.recordset")
	rdsql = "select * from news where id = "&id
	rs.open rdsql,db,1,3
	if not rs.eof then
		title = rs("title")
		name = rs("name")
		content = rs("content")
	end if
end if
%>
<html>
<head>
	<title>编辑@最简单的ASP模型-ASP新闻发布系统</title>
</head>
<body>
<table>
<tr>
<form name = "edit" method = "post" action=save.asp?id=<%=id%>>
<td>标题</td><td><input size="50" maxlength = "100" name = "title" id = "title" value=<%=title%>></td>
<tr>
	<td>作者</td>
	<td><input size="50" name = "name" id = "name" value=<%=name%>></td>
	</tr>
	<tr>
	<td valign="top" >内容</td>
	<td>
	<textarea cols="50" rows="20" name = "content" type = "text" id = "content"><%=content%></textarea></td>
	</tr>
</table>
<input type = "submit" name = "submit" value="确定">
<input type = "submit" name = "rest" value = "重置">
</form>
