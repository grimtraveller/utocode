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
	 zjrand.obj		
BINS=t_zjmem.exe t_zjerror.exe hamming.exe t_zjtype.exe
.PHONY:all clean

all:$(BINS)
%.o:%.c %.h
	$(CC) $(CFLAGS) $(INCD) $(LDFLAGS) /c $(INCDIR)$< -o $(BINDIR)$@
libzjrt0.lib:$(OBJS)
	$(AR) $(ARFLAG) $(OBJS) /OUT:$@
t_zjmem.exe:t_zjmem.obj libzjrt0.lib
	$(CC) $(CFLAGS) $(LDFLAGS) t_zjmem.obj libzjrt0.lib  /Fe$@
t_zjerror.exe:t_zjerror.obj libzjrt0.lib
	$(CC) $(CFLAGS) $(LDFLAGS) t_zjmem.obj libzjrt0.lib  /Fe$@
haming.exe:haming.obj libzjrt0.lib
clean:
	del *.obj *.pdb *.ilk
	del libzjrt0.lib
	del $(BINS)
