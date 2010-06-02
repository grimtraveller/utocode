#!/bin/sh
##################################
#Name:		macro.sh
#Desc:		print the macro about using in linux c
#Usage:		$macro.sh O_CREAT
#Date:		2009/01/06
#Author:	zuohaitao
###################################
MACRO_NAME=$1
if [ -z ${MACRO_NAME} ]; then
	echo "usage: macro.sh [MACRO]"
	exit
fi
PATTERN="#define "${MACRO_NAME}
#echo ${PATTERN}
grep -r "$PATTERN" /usr/include 2>/dev/null
