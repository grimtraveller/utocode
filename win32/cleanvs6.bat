echo Usage:%0 [projectpath] [projectname]
set prjpath=%1
set prjname=%2
DEL "%prjpath%\%prjname%.aps"
DEL "%prjpath%\%prjname%.ncb"
DEL "%prjpath%\%prjname%.opt"
DEL "%prjpath%\%prjname%.plg"
DEL "%prjpath%\%prjname%.positions"
DEL "%prjpath%\ReadMe.txt"
RMDIR /s /q "%prjpath%\Debug"
RMDIR /s /q "%prjpath%\\Release"
