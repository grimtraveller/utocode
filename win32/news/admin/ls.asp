<!-- #include file = "conn.asp" -->
<html>
	<head>
		<title>���Ź���@��򵥵�ASPģ��-ASP���ŷ���ϵͳ</title>
	</head>
	<body>
		<a href="edit.asp">���</a>
<%
sql = "select * from news order by id desc" 'id��������
set rs = db.execute(sql)
%>
<table width=80% border=1>
 <tr>
	<td align = "center">ID</td>
	<td align = "center">����</td>
	<td align = "center">����</td>
	<td align = "center">�༭</td>
	<td align = "center">ɾ��</td>
 </tr>
<%do while not rs.eof%>
 <tr>
  <td align = "center"><%=rs("id")%></td>
  <td align = "center"><a href=../view.asp?id=<%=rs("id")%> target = "_blank"><%=rs("title")%></a></td>
  <td align = "center"><%=rs("time")%></td>
  <td align = "center"><a href=edit.asp?id=<%=rs("id")%>>�༭</a></td>
  <td align = "center"><a href=del.asp?id=<%=rs("id")%>>ɾ��</a></td>
 </tr>
<%
rs.movenext
loop
%>
</table>
</body>
</html>
