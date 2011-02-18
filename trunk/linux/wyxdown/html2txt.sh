#!/bin/sh
#if	[	! -e article	]  
#	then  
#		cc article.c ../datastructure/public.c -o article
#	fi

#./article $1 |tr '>' '\n' > $1.tmp 
./article $1 > $1.tmp 
sed '/^$/d' $1.tmp > $1.tmp.tmp
rm $1.tmp
mv $1.tmp.tmp $1.tmp
sed '/^<p$/d' $1.tmp > $1.tmp.tmp
rm $1.tmp
mv $1.tmp.tmp $1.tmp
sed 's/<.*//g' $1.tmp > $1.tmp.tmp
rm $1.tmp
mv $1.tmp.tmp $1.tmp
sed '/^$/d' $1.tmp > $1.tmp.tmp
rm $1.tmp
mv $1.tmp.tmp $1.txt
rm $1
