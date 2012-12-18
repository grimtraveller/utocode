::::
:: ms vs build batch file
::::
@echo off
call "C:\Program Files\Microsoft Visual Studio 9.0\VC\vcvarsall.bat"
"C:\Program Files\Microsoft Visual Studio 9.0\Common7\IDE\devenv.com" project.sln /Rebuild "Debug" /Out log.txt
"C:\Program Files\Microsoft Visual Studio 9.0\Common7\IDE\devenv.com" project.sln /Rebuild "Release" /Out log.txt
echo ************
pause
