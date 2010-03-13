<!-- #include file = "conn.asp" -->
<%
sql = "select * from news order by id desc"
set rs = db.execute(sql)
%>
<html>
	<head>
		<title>首页@最简单的ASP模型-ASP新闻发布系统</title>
	</head>
	<body align="center">
	<h1>新闻发布系统</h1>
	<hr />
<ul>
<% do while not rs.eof%>
<li><h4><a href = view.asp?id=<%=rs("id")%> target = "_blank">
<%=rs("title") %>
</a><h4></li>
<h5>作者:<%=rs("name")%> (<%=rs("count")%>)</h5><br>
<%=rs("content")%>
<%
rs.movenext
loop
%>
</ul>
</body>
</html>
