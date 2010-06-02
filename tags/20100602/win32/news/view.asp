<!-- #include file = "conn.asp" -->

<%
'判断传递参数
if not isempty(request.querystring("id")) then
  id = request.querystring("id")
else
 id = 1
end if

function htmltotext(content) 'html转化为字符串
  htmlnr = content
  if content <> "" then
    htmlnr = replace(htmlnr,chr(13),"<br>")
    htmlnr = replace(htmlnr,chr(34),"&quot;")
    htmlnr = replace(htmlnr,chr(32),"&nbsp;")
  end if
end function

set rs = server.createobject("adodb.recordset")
sql = "select * from news where id = "&id
rs.open sql,db,1,3
rs("count") = rs("count") + 1
rs.update
%>
<html>
<head>
<title><%=rs("title")%>@最简单的ASP模型-ASP新闻发布系统</title>
</head>
<h1><%=rs("title")%></h1>
<h5>时间:<%=rs("time")%><br />
该新闻已被浏览过<%=rs("count")%>次<br />
作者:<%=rs("name")%></h5>
<h3><%=(rs("content"))%></h3><br />
<input type="button" name="submit" value="关闭窗口" onclick="javascript:window.close()">
</html>
