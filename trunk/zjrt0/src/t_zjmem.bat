@echo off
set "count=0"
:loop
	if %count% == %1 goto end
	echo %count%
	echo ========================================== >> test.txt
	..\prj\jzrt0\debug\zjmem_t.exe >> test.txt
	echo ========================================== >> test.txt
	set /a count+=1
	goto loop
:end
