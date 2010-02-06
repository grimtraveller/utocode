CC=gcc
CFLAG=-Wall -ggdb
INCLUDE=-I./
LIB=-L./
AR=ar
ARFLAG=rcs
OBJ=hilib
all:hi

hi:hi.o libhi.a
	$(CC) $(CFLAG) hi.o -o hi $(LIB) -lhi
hi.o:hi.c hilib.h
	$(CC) -c $(CFLAG) hi.c
libhi.a:hilib.o
	$(AR) $(ARFLAG) libhi.a hilib.o
hilib.o:hilib.c
	$(CC) -c $(CFLAG) hilib.c

clean:
	-rm hi.o libhi.a hilib.o
	-rm hi
.PHONY:all clean
