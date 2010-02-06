.PHONY:all clean install update tcase
all:
	gcc pfxsend.c -Wall -g -I. -I../include -L. -L../lib -lpfxshare -lpthread -o pfxsend
	mv pfxsend ../../target/bin
clean:
	mv ../../target/bin/pfxsend
