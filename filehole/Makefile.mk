CC=cl
CFLAGS= /nologo /Zi /D "WIN32"
BINS=mkhole.exe cphole.exe
.PHONY:all clean
%.c:%.obj
	$(CC) $@ $(CFLAGS) /c $<
all:$(BINS)
mkhole.exe:mkhole.obj
	$(CC) $(CFLAGS) mkhole.obj /Fe$@
cphole.exe:cphole.obj
	$(CC) $(CFLAGS) cphole.obj /Fe$@
clean:
	del *.obj *.pdb *.ilk
	del $(BINS)
