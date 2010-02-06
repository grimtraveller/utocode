CC=gcc
CFLAG=-Wall -ggdb
INCLUDE=-I./
LIB=-L./
OBJ=hilib.o
all:hi

hi:hi.o libhi.so
	$(CC) $(CFLAG) hi.o -o hi $(LIB) -lhi
hi.o:hi.c hilib.h
	$(CC) -c $(CFLAG) hi.c
libhi.so:libhi.so.0
	ln -sf libhi.so.0 libhi.so
libhi.so.0:libhi.so.0.0
	ln -sf libhi.so.0.0 libhi.so.0
libhi.so.0.0:hilib.o
	$(CC) hilib.o $(CFLAG) -shared -Wl,-soname,libhi.so.0 -o libhi.so.0.0 
hilib.o:hilib.c
	$(CC) -fPIC -c $(CFLAG) hilib.c

clean:
	-rm hi.o libhi.so* hilib.o
	-rm hi

exec:
	LD_LIBRARY_PATH=./ ./hi
install:libhi.so
	cp libhi.so libhi.so.0 libhi.so.0.0 /usr/lib/
#install2:
	#$>add "/home/zuohaitao/src/share" in /etc/ld.so.conf
	#$>sudo ldconfig
uninstall:
	-rm /usr/lib/libhi.so
	-rm /usr/lib/libhi.so.0
	-rm /usr/lib/libhi.so.0.0

.PHONY:all clean exec install uninstall
