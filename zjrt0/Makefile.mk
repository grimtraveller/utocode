
CC=cl
CFLAGS=/Zi /D"WIN32" /nologo
INCD=/I .
LD=link
LDFLAGS=/NOLOGO /DLL /NOENTRY /MACHINE:X86
AR=lib
ARFLAG=
DEFFILE=zjrt0.def
LIBFILE=libzjrt0.lib
DLLFILE=zjrt0.dll
DLLOBJ=zjdllmain.obj
OBJS=zjtuxlist.obj zjtype.obj zjsleep.obj zjlock.obj
BINS=tzjtype tzjlock tzjsleep tzjlock
.PHONY:all clean $(BINS)

all:$(DLLFILE) $(LIBFILE) $(BINS)
#%.obj:%.c %.h
#	$(CC) $(CFLAGS) $(INCD) $(LDFLAGS) /c $(INCDIR)$< -o $(BINDIR)$@
$(DLLFILE):$(DLLOBJ) $(LIBFILE) $(DEFFILE)
	$(LD) $(LDFLAGS) $(DLLOBJ) $(LIBFILE) /OUT:$@ /DEF:$(DEFFILE)
$(LIBFILE):$(OBJS)
	$(AR) $(ARFLAG) $(OBJS) /OUT:$@
tzjtype:$(LIBFILE)
	$(CC) $(CFLAGS) $? $@.c /Fe$@.exe
tzjlock:$(LIBFILE)
	$(CC) $(CFLAGS) $? $@.c /Fe$@.exe
tzjsleep:$(LIBFILE)
	$(CC) $(CFLAGS) $? $@.c /Fe$@.exe

clean:
	del *.obj *.pdb *.ilk *.exp
	del *.lib
	del t*.exe
	del *.dll

