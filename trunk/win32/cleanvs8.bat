echo Usage:%0 [projectpath] [projectname]
set prjpath=%1
set prjname=%2
DEL "%prjpath%\%prjname%.ncb"
DEL /a:h "%prjpath%\%prjname%.suo"
RMDIR /s /q "%prjpath%\Debug"
RMDIR /s /q "%prjpath%\Release"
RMDIR /s /q "%prjpath%\%prjname%\Debug"
RMDIR /s /q "%prjpath%\%prjname%\Release"
DEL "%prjpath%\%prjname%\ctldemo.aps"
