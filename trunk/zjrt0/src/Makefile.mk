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
OBJS=zjmem.obj	\
	 zjtimer.obj	\
	 zjerror.obj 	\
	 zjrand.obj		\
	 zjlog.obj		\
	 zjhamming.obj
BINS=t_zjmem.exe t_zjerror.exe t_zjhamming.exe t_zjtype.exe t_zjlog.exe
.PHONY:all clean

all:libzjrt0.lib $(BINS) 
	copy *.h ..\dist\zjrt0
	copy libzjrt0.lib ..\dist\zjrt0
	copy *.exe ..\dist\zjrt0
	copy *.bat ..\dist\zjrt0
#%.obj:%.c %.h
#	$(CC) $(CFLAGS) $(INCD) $(LDFLAGS) /c $(INCDIR)$< -o $(BINDIR)$@
libzjrt0.lib:$(OBJS)
	$(AR) $(ARFLAG) $(OBJS) /OUT:$@
t_zjmem.exe:libzjrt0.lib
	$(CC) $(CFLAGS) $(LDFLAGS) $?  /Fe$@
t_zjerror.exe:libzjrt0.lib
	$(CC) $(CFLAGS) $(LDFLAGS) $?  /Fe$@
t_zjhamming.exe:libzjrt0.lib
	$(CC) $(CFLAGS) $(LDFLAGS) $?  /Fe$@
t_zjlog.exe:libzjrt0.lib
	$(CC) $(CFLAGS) $(LDFLAGS) $?  /Fe$@
clean:
	del *.obj *.pdb *.ilk
	del libzjrt0.lib
	del $(BINS)
