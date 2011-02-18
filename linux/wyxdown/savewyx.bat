@echo off
set count=%1
set urlb=%2
set urlm=%3
set urle=.html
set target_dir=%4
mkdir %target_dir%
set i=1
:for_begin
if %i% gtr %count% goto for_end
@echo %i%
set url=%urlb%%urlm%%i%%urle%
curl %urlb%%urlm%%i%%urle% > .\%target_dir%\%urlm%%i%
call html2txt.bat %~dp0%target_dir%\%urlm%%i%
set /a i=%i%+1 
goto for_begin
:for_end

