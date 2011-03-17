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
BINS=t_zjlog.exe t_zjhamming.exe
#BINS=t_zjlog.exe t_zjmem.exe t_zjerror.exe hamming.exe t_zjtype.exe
.PHONY:all clean

all:$(BINS)
	copy *.h ..\dist\zjrt0
	copy libzjrt0.lib ..\dist\zjrt0
%.o:%.c %.h
	$(CC) $(CFLAGS) $(INCD) $(LDFLAGS) /c $(INCDIR)$< -o $(BINDIR)$@
libzjrt0.lib:$(OBJS)
	$(AR) $(ARFLAG) $(OBJS) /OUT:$@
t_zjmem.exe:t_zjmem.obj libzjrt0.lib
	$(CC) $(CFLAGS) $(LDFLAGS) t_zjmem.obj libzjrt0.lib  /Fe$@
#t_zjerror.exe:t_zjerror.obj libzjrt0.lib
#	$(CC) $(CFLAGS) $(LDFLAGS) t_zjerror.obj libzjrt0.lib  /Fe$@
#haming.exe:haming.obj libzjrt0.lib
t_zjlog.exe:t_zjlog.obj libzjrt0.lib
	$(CC) $(CFLAGS) $(LDFLAGS) t_zjlog.obj libzjrt0.lib  /Fe$@
t_zjhamming.exe:t_zjhamming.obj libzjrt0.lib
	$(CC) $(CFLAGS) $(LDFLAGS) t_zjhamming.obj libzjrt0.lib  /Fe$@
clean:
	del *.obj *.pdb *.ilk
	del libzjrt0.lib
	del $(BINS)
