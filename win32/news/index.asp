<!-- #include file = "conn.asp" -->
<%
sql = "select * from news order by id desc"
set rs = db.execute(sql)
%>
<html>
	<head>
		<title>��ҳ@��򵥵�ASPģ��-ASP���ŷ���ϵͳ</title>
	</head>
	<body align="center">
	<h1>���ŷ���ϵͳ</h1>
	<hr />
<ul>
<% do while not rs.eof%>
<li><h4><a href = view.asp?id=<%=rs("id")%> target = "_blank">
<%=rs("title") %>
</a><h4></li>
<h5>����:<%=rs("name")%> (<%=rs("count")%>)</h5><br>
<%=rs("content")%>
<%
rs.movenext
loop
%>
</ul>
</body>
</html>
