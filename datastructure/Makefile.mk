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
CC=cl
CFLAGS=/Zi /Wall
INCD=-I.
LDFLAGS=-L. 
AR=ar
ARFLAG=rcs
OBJS=public.obj public_t.obj
BINS=public_t.exe merge.exe sq_queue_t.exe stack_t.exe conversion.exe matchbrackets.exe lineedit.exe
.PHONY:all clean

all:$(BINS)
%.obj:%.c %.h
	$(CC) $(CFLAGS) $(INCD) $(LDFLAGS) -c $< -o $@
public_t.exe:public.obj public_t.obj
	$(CC) $(CFLAGS) public.obj public_t.obj -o $@
merge.exe:merge.obj public.obj list.obj
	$(CC) $(CFLAGS) merge.obj public.obj list.obj -o $@
conversion.exe:conversion.obj public.obj stack.obj
	$(CC) $(CFLAGS) conversion.obj public.obj stack.obj -o $@
matchbrackets.exe:matchbrackets.obj public.obj stack.obj
	$(CC) $(CFLAGS) matchbrackets.obj public.obj stack.obj -o $@
sq_queue_t.exe:sq_queue_t.obj public.obj sq_queue.obj
	$(CC) $(CFLAGS) sq_queue_t.obj public.obj sq_queue.obj -o $@
stack_t.exe:stack_t.obj public.obj stack.obj
	$(CC) $(CFLAGS) stack_t.obj public.obj stack.obj -o $@
lineedit.exe:lineedit.obj public.obj stack.obj
	$(CC) $(CFLAGS) lineedit.obj public.obj stack.obj -o $@
clean:
	del *.obj
	del $(BINS)
	del *.ilk
	del *.pdb
