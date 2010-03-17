CC=gcc
CFLAGS=-ggdb -Wall -O
INCD=-I.
LDFLAGS=-L. 
AR=ar
ARFLAG=rcs
OBJS=public.o public_t.o
BINS=public_t merge sq_queue_t stack_t conversion matchbrackets lineedit
.PHONY:all clean

all:$(BINS)
%.o:%.c %.h
	$(CC) $(CFLAGS) $(INCD) $(LDFLAGS) -c $< -o $@
public_t:public.o public_t.o
	$(CC) $(CFLAGS) $^ -o $@
merge:merge.o public.o list.o
	$(CC) $(CFLAGS) $^ -o $@
conversion:conversion.o public.o stack.o
	$(CC) $(CFLAGS) $^ -o $@
matchbrackets:matchbrackets.o public.o stack.o
	$(CC) $(CFLAGS) $^ -o $@
sq_queue_t:sq_queue_t.o public.o sq_queue.o
	$(CC) $(CFLAGS) $^ -o $@
stack_t:stack_t.o public.o stack.o
	$(CC) $(CFLAGS) $^ -o $@
lineedit:lineedit.o public.o stack.o
	$(CC) $(CFLAGS) $^ -o $@
clean:
	rm *.o
	rm $(BINS)
