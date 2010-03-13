<%
set db=server.createobject("adodb.connection")
db.open "DBQ="&server.mappath("date\news.mdb")&";driver={microsoft access driver (*.mdb)}"
%>
