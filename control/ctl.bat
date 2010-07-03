::
::name:	ctl.bat
::use:	1 control execute(like tshark)'s start and end
::usage:	
::		1. $chmode 0744 ctl.sh
::		2. ctl.sh
::author:zuohaitao
::date:	2008/07/27
::modify:fix
::			if printf nothing information to logtext.txt in shark,
::			add fflush(stdout)
::
:loop
	echo "run tshark"
	start tshark.bat
	echo "sleep 3 seconds"
	ping -n 3 127.1 >nul 2>nul
	echo "kill tshark"
	taskkill /F /IM tshark.exe
	echo "sleep 10 mins"
	ping -n 6 127.1 >nul 2>nul
	goto loop
:end



