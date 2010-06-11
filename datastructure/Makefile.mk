#
# /*!@file	Makefile
#  *@brief	Make DataStruct project
#  *
#  *usage	$>nmake all /f Makefile.mk
#  			$>make clean /f Makefile.mk /nologo
#  *@author	zuohaitao
#  *@date	2010-03-26
#  */
#
CC=cl
CFLAGS=/Zi /nologo
OBJS=public.obj public_t.obj
BINS=public_t.exe merge.exe sq_queue_t.exe stack_t.exe conversion.exe matchbrackets.exe lineedit.exe
.PHONY:all clean

all:$(BINS)
%.obj:%.c %.h
	$(CC) $@ $(CFLAGS) /c $<
public_t.exe:public_t.obj public.obj
	$(CC) $(CFLAGS) public_t.obj public.obj
merge.exe:merge.obj public.obj list.obj
	$(CC) $(CFLAGS) merge.obj public.obj list.obj
conversion.exe:conversion.obj public.obj stack.obj
	$(CC) $(CFLAGS) conversion.obj public.obj stack.obj
matchbrackets.exe:matchbrackets.obj public.obj stack.obj
	$(CC) $(CFLAGS) matchbrackets.obj public.obj stack.obj
sq_queue_t.exe:sq_queue_t.obj public.obj sq_queue.obj
	$(CC) $(CFLAGS) sq_queue_t.obj public.obj sq_queue.obj
stack_t.exe:stack_t.obj public.obj stack.obj
	$(CC) $(CFLAGS) stack_t.obj public.obj stack.obj
lineedit.exe:lineedit.obj public.obj stack.obj
	$(CC) $(CFLAGS) lineedit.obj public.obj stack.obj
clean:
	del *.obj
	del $(BINS)
	del *.ilk
	del *.pdb
