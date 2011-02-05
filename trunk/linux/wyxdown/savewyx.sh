#!/bin/sh
urlb=$2
urlm=$3
urle=".html"
target_dir=$4
mkdir $target_dir
i=1
while [ $i -le $1 ]
do
	if [ ! -e $urlm$i ]
	then
		curl $urlb$urlm$i$urle > $target_dir/$urlm$i
		echo $urlb$urlm$i$urle 
		echo ">"
		echo $target_dir/$urlm$i
	fi
	./html2txt.sh $target_dir/$urlm$i
	i=`expr $i + 1`
done
