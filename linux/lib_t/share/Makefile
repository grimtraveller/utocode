CC=gcc
CFLAG=-Wall -ggdb
INCLUDE=-I./
LIB=-L./
OBJ=hilib.o
#all:hi dl_hi dl_hi_abs
all:dlhi dlhi_abs hi
#dl absolute and LD_LIBRARY_PATH
dlexec_abs:
	./dlhi_abs
dlexec:
	export LD_LIBRARY_PATH=.&&./dlhi
dlhi_abs:dlhi_abs.o dlhi.so
	$(CC) $(CFLAG) dlhi_abs.o -o dlhi_abs $(LIB) -ldl
dlhi:dlhi.o dlhi.so
	$(CC) $(CFLAG) dlhi.o -o dlhi $(LIB) -ldl

dlhi_abs.o:dlhi.c
	$(CC) $(CFLAG) $(CLDFLAG) $(CLFLAG) -DABSOLUTE_PATH -c $< -o $@
dlhi.o:dlhi.c
	$(CC) $(CFLAG) $(CLDFLAG) $(CLFLAG) -c $< -o $@
dlhi.so:libhi.o
	$(CC) $(CFLAG) -shared -nostartfiles $< -o $@
#share
hi:hi.o libhi.so
	$(CC) $(CFLAG) hi.o -o hi $(LIB) -lhi
hi.o:hi.c libhi.h
	$(CC) -c $(CFLAG) $<
libhi.so:libhi.so.0
	ln -sf libhi.so.0 libhi.so
libhi.so.0:libhi.so.0.0
	ln -sf libhi.so.0.0 libhi.so.0
libhi.so.0.0:libhi.o
	$(CC) $< $(CFLAG) -shared -Wl,-soname,libhi.so.0 -o $@
libhi.so.0.1:libhi1.o
	$(CC) $< $(CFLAG) -shared -Wl,-soname,libhi.so.0 -o $@
libhi.o:libhi.c
	$(CC) -fPIC $(CFLAG) -c $< -o $@
libhi1.o:libhi1.c
	$(CC) -fPIC $(CFLAG) -c $< -o $@
clean:
	-rm *.o
	-rm dlhi.so
	-rm dlhi
	-rm dlhi_abs
	-rm libhi.so*
	-rm hi

exec:
	LD_LIBRARY_PATH=. ./hi
exec_sys:
	./hi
exec2:
	LD_PRELOAD=./libhi.so ./hi
install:libhi.so
	cp libhi.so libhi.so.0 libhi.so.0.0 /usr/lib/
update:libhi.so.0.1
	ln -sf libhi.so.0.1 libhi.so.0
	cp libhi.so.0 libhi.so.0.1 /usr/lib/
revert:libhi.so.0.0
	-ln -sf libhi.so.0.0 libhi.so.0
	-cp libhi.so.0 /usr/lib
	-rm /usr/lib/libhi.so.0.1

install2:
	#su root
	echo . >> /etc/ld.so.conf
	ldconfig
uninstall2:
	#su root
	sed -i '/^\.$$/d' /etc/ld.so.conf
	ldconfig
uninstall:
	-rm /usr/lib/libhi.so
	-rm /usr/lib/libhi.so.0
	-rm /usr/lib/libhi.so.0.0

.PHONY:all clean dl_exec_abs dl_exec exec install uninstall update revert exe2 install2 uninstall2
