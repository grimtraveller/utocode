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
	 zjhamming.obj	\
	 zjvalarray.obj
BINS=t_zjmem t_zjerror t_zjhamming t_zjtype t_zjlog t_zjvalarray
.PHONY:all clean $(BINS)

all:libzjrt0.lib $(BINS) 
	copy *.h ..\dist\zjrt0
	copy libzjrt0.lib ..\dist\zjrt0
	copy *.exe ..\dist\zjrt0
	copy *.bat ..\dist\zjrt0
#%.obj:%.c %.h
#	$(CC) $(CFLAGS) $(INCD) $(LDFLAGS) /c $(INCDIR)$< -o $(BINDIR)$@
libzjrt0.lib:$(OBJS)
	$(AR) $(ARFLAG) $(OBJS) /OUT:$@
t_zjtype:libzjrt0.lib
	$(CC) $(CFLAGS) $(LDFLAGS) $?  $@.c /Fe$@
t_zjmem:libzjrt0.lib
	$(CC) $(CFLAGS) $(LDFLAGS) $?  $@.c /Fe$@
t_zjerror:libzjrt0.lib
	$(CC) $(CFLAGS) $(LDFLAGS) $?  $@.c /Fe$@
t_zjhamming:libzjrt0.lib
	$(CC) $(CFLAGS) $(LDFLAGS) $?  $@.c /Fe$@
t_zjlog:libzjrt0.lib
	$(CC) $(CFLAGS) $(LDFLAGS) $?  $@.c /Fe$@
t_zjvalarray:libzjrt0.lib
	$(CC) $(CFLAGS) $(LDFLAGS) $?  $@.c /Fe$@.exe
clean:
	del *.obj *.pdb *.ilk
	del libzjrt0.lib
	del t_*.exe
	del ..\prj\vs2008\*.user
