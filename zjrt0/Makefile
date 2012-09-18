#
# /*!@file	Makefile
#  *@brief	Make DataStruct project
#  *
#  *usage	$>make all
#  			$>make clean
#  *@author	zuohaitao
#  *@date	2010-03-26
#  */
#
CC=gcc
CFLAGS+=-ggdb -Wall -O -DDEBUG
INCD=-I.
LDFLAGS=-L. 
AR=ar
ARFLAG=rcs
OBJS=
BINS=tzjmem
.PHONY:all clean

all:$(BINS)
%.o:%.c %.h
	$(CC) $(CFLAGS) $(INCD) $(LDFLAGS) -c $(INCDIR)$< -o $(BINDIR)$@
libzjrt0.a:zjmem.o
	$(AR) $(ARFLAG) $@ $^
tzjmem:t_zjmem.o libzjrt0.a
	$(CC) $(CFLAGS) $(LDFLAGS) -lzjrt0 $^ -o $@
clean:
	rm *.o
	rm libzjrt0.a
	rm $(BINS)
