set prjpath=D:\demo\ctldemo
set prjname=ctldemo
DEL "%prjpath%\%prjname%.ncb"
DEL /a:h "%prjpath%\%prjname%.suo"
RMDIR /s /q "%prjpath%\Debug"
RMDIR /s /q "%prjpath%\\Release"
RMDIR /s /q "%prjpath%\%prjname%\Debug"
RMDIR /s /q "%prjpath%\%prjname%\Release"
DEL "%prjpath%\%prjname%\ctldemo.aps"
