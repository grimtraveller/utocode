#nmake all /f Makefile.mk
all:
	cl concurrency_server.c /Zi  /DWIN32
	cl concurrency_client.c /Zi /DWIN32
clean:
	del *.exe *.pdb *.obj *.ilk
