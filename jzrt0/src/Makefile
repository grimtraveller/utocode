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
CFLAGS=-ggdb -Wall -O -DDEBUG
INCD=-I.
LDFLAGS=-L. 
AR=ar
ARFLAG=rcs
OBJS=
BINS=jzmem_t
.PHONY:all clean

all:$(BINS)
%.o:%.c %.h
	$(CC) $(CFLAGS) $(INCD) $(LDFLAGS) -c $(INCDIR)$< -o $(BINDIR)$@
libjzrt0.a:jzmem.o
	$(AR) $(ARFLAG) $@ $^
jzmem_t:jzmem_t.o libjzrt0.a
	$(CC) $(CFLAGS) $(LDFLAGS) -ljzrt0 $^ -o $@
clean:
	rm *.o
	rm libjzrt0.a
	rm $(BINS)
