del article.exe
if NOT EXIST article.exe cl article.c -DWIN32
echo %1
article.exe %1
REM article.exe .\%1 > %1%.tmp
