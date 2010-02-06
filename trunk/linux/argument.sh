#!/bin/sh
#
#name:		argument.sh
#Desc:		argment in shell
#Usage:		$>argument.sh a b c -d
#Author:	zuohaitao
#Date:		2009-03-12
#
for arg in $*
do
	echo $arg
done
