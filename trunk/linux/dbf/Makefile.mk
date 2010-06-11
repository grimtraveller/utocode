CC=cl
CFLAG= /Zi /nologo /DWIN32
all:dbf_t.exe
dbf_t.exe:dbf_t.obj dbf.obj
	$(CC) $(CFLAG) dbf_t.obj dbf.obj
dbf_t.obj:dbf_t.c
	$(CC) $(CFLAG) /c dbf_t.c
dbf.obj:dbf.c dbf.h
	$(CC) $(CFLAG) /c dbf.c
clean:
	del *.exe
	del *.pdb
	del *.obj
	del *.ilk
