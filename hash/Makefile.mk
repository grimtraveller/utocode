CC=cl
CFLAGS=/Zi /D"WIN32" /nologo
INCD=/I .
LD=link
LDFLAGS=/NOLOGO /DLL /NOENTRY /MACHINE:X86
AR=lib
ARFLAG=
OBJS=CRC32.obj MD5.obj SHA1.obj sha256.obj
BINS=thash
.PHONY:all clean $(BINS)

all:$(BINS)

thash:$(OBJS)
	$(CC) $(CFLAGS) $? $@.cpp /Fe$@.exe
clean:
	del *.obj *.pdb *.ilk *.exp
	del *.lib
	del t*.exe
	del *.dll
