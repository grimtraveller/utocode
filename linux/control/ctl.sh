#
#name:	ctl.sh
#use:	1 control execute(like tshark)'s start and end
#usage:	
#		1. $chmode 0744 ctl.sh
#		2. ctl.sh
#author:zuohaitao
#date:	2008/07/27
#modify:fix 如果tshark中使用printf输出信息则保存不到logtext.txt中
#		加入fflush(stdout);
#
#!/bin/sh
while((1))
do
	echo "run tshark"
	tshark >> logtext.txt &
	echo "sleep 3 seconds"
	sleep 3
	echo "kill tshark"
	kill `ps -A|grep "tshark"|awk '{print $1}'`
	echo "sleep 10 mins"
	sleep 600
done
