<!-- #include file = "conn.asp" -->

<%
'�жϴ��ݲ���
if not isempty(request.querystring("id")) then
  id = request.querystring("id")
else
 id = 1
end if

function htmltotext(content) 'htmlת��Ϊ�ַ���
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
<title><%=rs("title")%>@��򵥵�ASPģ��-ASP���ŷ���ϵͳ</title>
</head>
<h1><%=rs("title")%></h1>
<h5>ʱ��:<%=rs("time")%><br />
�������ѱ������<%=rs("count")%>��<br />
����:<%=rs("name")%></h5>
<h3><%=(rs("content"))%></h3><br />
<input type="button" name="submit" value="�رմ���" onclick="javascript:window.close()">
</html>
