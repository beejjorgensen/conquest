TARGET=conquest

CC = gcc
CFLAGS = -Wall -Wextra
LDFLAGS = -lcurses -lm

SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)

.PHONY: all clean pristine

all: $(TARGET)

$(TARGET): $(OBJS)
		$(CC) $^ -o $@ $(LDFLAGS)

%.o: %.c vars.h structs.h defs.h funcs.h
	$(CC) -c $(CFLAGS) $<

conq2.o: conq2.c vars.h structs.h defs.h
	$(CC) -c $(CFLAGS) $<

conqgl.o: conqgl.c structs.h
	$(CC) -c $(CFLAGS) $<

conqlib.o: conqlib.c
	$(CC) -c $(CFLAGS) $<

clean:
	rm -f $(OBJS)

pristine: clean
	rm -f $(TARGET)
