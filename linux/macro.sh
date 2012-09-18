#!/bin/sh
##################################
#Name:		macro.sh
#Desc:		print the macro about using in linux c
#Usage:		$macro.sh O_CREAT
#Date:		2009/01/06
#Author:	zuohaitao
#Date:		2012/09/18
#			add GNUStep environment
###################################
MACRO_NAME=$1
if [ -z ${MACRO_NAME} ]; then
	echo "usage: macro.sh [MACRO]"
	exit
fi
PATTERN="#define[ 	]*"${MACRO_NAME}
#echo ${PATTERN}
grep -r "$PATTERN" /usr/include 2>/dev/null
grep -r "$PATTERN" /c/GNUstep/include 2>/dev/null
grep -r "$PATTERN" /c/GNUstep/msys/1.0/include 2>/dev/null
grep -r "$PATTERN" /c/GNUstep/share 2>/dev/null
grep -r "$PATTERN" /GNUstep/System/Library/Headers 2>/dev/null
