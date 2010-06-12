CC=cl
CFLAG=/nologo /O2 /Zi /DWIN32
RM=del
all:
	$(CC) $(CFLAG) udp_getip.c /Feudp_noconn_getip.exe
	$(CC) $(CFLAG) udp_noconn_send_t.c /Feudp_send.exe
	$(CC) $(CFLAG) udp_noconn_recv_t.c /Feudp_recv.exe
clean:
	del udp_send.exe udp_recv.exe udp_noconn_getip.exe
	del *.pdb *.ilk *.obj
