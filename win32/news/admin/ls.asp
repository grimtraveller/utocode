<!-- #include file = "conn.asp" -->
<html>
	<head>
		<title>新闻管理@最简单的ASP模型-ASP新闻发布系统</title>
	</head>
	<body>
		<a href="edit.asp">添加</a>
<%
sql = "select * from news order by id desc" 'id降序排列
set rs = db.execute(sql)
%>
<table width=80% border=1>
 <tr>
	<td align = "center">ID</td>
	<td align = "center">标题</td>
	<td align = "center">日期</td>
	<td align = "center">编辑</td>
	<td align = "center">删除</td>
 </tr>
<%do while not rs.eof%>
 <tr>
  <td align = "center"><%=rs("id")%></td>
  <td align = "center"><a href=../view.asp?id=<%=rs("id")%> target = "_blank"><%=rs("title")%></a></td>
  <td align = "center"><%=rs("time")%></td>
  <td align = "center"><a href=edit.asp?id=<%=rs("id")%>>编辑</a></td>
  <td align = "center"><a href=del.asp?id=<%=rs("id")%>>删除</a></td>
 </tr>
<%
rs.movenext
loop
%>
</table>
</body>
</html>
