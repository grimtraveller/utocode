TARGET = ../../target
SRC = ..
CC = gcc
CFLAGS = -Wall -g -I.
LDFLAGS = -L. -L$(SRC)/lib
AR=ar
ARFLAGS=rcs
LIBS=libpfxshare.a
HEAD=pfxdatetime.h pfxerror.h pfxlock.h pfxdirapi.h pfxthreadapi.h
SRCS=pfxdatetime.c pfxerror.c pfxlock.c pfxdirapi.c pfxthreadapi.c
OBJS=pfxdatetime.o pfxerror.o pfxlock.o pfxdirapi.o pfxthreadapi.o
APPS=pfxdatetime_t pfxerror_t pfxlock_t pfxdirapi_t pfxthreadapi_t
.PHONY:all clean install update tcase
all:$(LIBS) $(APPS) install
$(LIBS):$(OBJS)
	$(AR) $(ARFLAGS) $(LIBS) $(OBJS)
	rm $(OBJS)
	mv $(LIBS) $(SRC)/lib
	cp $(HEAD) $(SRC)/include
pfxdatetime_t: $(LIBS) pfxdatetime_t.c 
	$(CC) pfxdatetime_t.c $(LDFLAGS) -lpfxshare -o $@
	mv $@ $(TARGET)/bin
pfxerror_t: $(LIBS) pfxerror_t.c 
	$(CC) pfxerror_t.c $(LDFLAGS) -lpfxshare -o $@
	mv $@ $(TARGET)/bin
pfxlock_t:$(LIBS) pfxlock_t.c
	$(CC) pfxlock_t.c $(LDFLAGS) -lpfxshare -o $@
	mv $@ $(TARGET)/bin
pfxdirapi_t:$(LIBS) pfxdirapi_t.c
	$(CC) pfxdirapi_t.c $(LDFLAGS) -lpfxshare -o $@
	mv $@ $(TARGET)/bin
pfxthreadapi_t:$(LIBS) pfxthreadapi_t.c
	$(CC) pfxthreadapi_t.c $(LDFLAGS) -lpfxshare -lpthread -o $@
	mv $@ $(TARGET)/bin
clean:
	rm -f ? *.o
	rm -f ? $(SRC)/lib/$(LIBS)
	rm -f $(TARGET)/bin/pfxdatetime_t
	rm -f $(TARGET)/bin/pfxerror_t
	rm -f $(TARGET)/bin/pfxlock_t
	rm -f $(TARGET)/bin/pfxdirapi_t
	rm -f $(TARGET)/bin/pfxthreadapi_t
install:
	
update:
