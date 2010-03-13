<!-- #include file = "conn.asp" -->
<%
id = request.querystring("id")
delsql = "delete * from news where id="&id
db.execute(delsql)
response.redirect "ls.asp"
%>
