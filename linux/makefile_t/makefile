##Makefile#############################3
#this is a standard Makefile file
#thx sunhp
CC=gcc
CFLAGS=-Wall -ggdb -I. 
HEADFILE=def.h
OBJ=main.o\
	fun.o

.PHONY:all clean
all:main
main:main.o fun.o
	$(CC) $(OBJ) -o main

main.o:main.c $(HEADFILE)
	$(CC) -c $(CFLAGS) $(HEADFILE) main.c

fun.o:fun.c $(HEADFILE)
	$(CC) -c $(CFLAGS) fun.c

clean:
	-rm *.o
	-rm main
#################################

