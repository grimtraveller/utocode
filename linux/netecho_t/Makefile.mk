CC=cl
CFLAG=/Zi /nologo /DWIN32
all:serv.exe client.exe
serv.exe:echoserv.exe
	ren echoserv.exe serv.exe
client.exe:echoclient.exe
	ren echoclient.exe client.exe
echoserv.exe:echoserv.c
	$(CC) $(CFLAG) echoserv.c
echoclient.exe:echoclient.c
	$(CC) $(CFLAG) echoclient.c
clean:
	del *.obj
	del *.pdb
	del *.ilk
	del client.exe serv.exe
