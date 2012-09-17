#!/bin/sh
for name in `ls -R|grep "[^:]$"`
do

	lowname=`echo $name | tr [A-Z] [a-z]`
	mv $name $lowname
done
