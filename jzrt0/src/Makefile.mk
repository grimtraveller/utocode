#
# /*!@file	Makefile
#  *@brief	Make DataStruct project in win32
#  *
#  *usage	$>make all
#  			$>make clean
#  *@author	zuohaitao
#  *@date	2010-03-26
#  */
#
CC=cl
CFLAGS=/Zi /D "WIN32" /nologo
INCD=/I.
LDFLAGS= 
AR=lib
ARFLAG=
OBJS=
BINS=jzmem_t.exe jzerror_t.exe
.PHONY:all clean

all:$(BINS)
%.o:%.c %.h
	$(CC) $(CFLAGS) $(INCD) $(LDFLAGS) /c $(INCDIR)$< -o $(BINDIR)$@
libjzrt0.lib:jzmem.obj jztimer.obj jzerror.obj
	$(AR) $(ARFLAG) jzmem.obj jztimer.obj jzerror.obj /OUT:$@
jzmem_t.exe:jzmem_t.obj libjzrt0.lib
	$(CC) $(CFLAGS) $(LDFLAGS) jzmem_t.obj libjzrt0.lib  /Fe$@
jzerror_t.exe:jzerror_t.obj libjzrt0.lib
	$(CC) $(CFLAGS) $(LDFLAGS) jzmem_t.obj libjzrt0.lib  /Fe$@
clean:
	del *.obj *.pdb *.ilk
	del libjzrt0.lib
	del $(BINS)
