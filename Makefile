TARGET=conquest

CC = gcc
CFLAGS = -Wall -Wextra
LDFLAGS = -lcurses -lm

SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)

.PHONY: all clean pristine help

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

BOLD := $(shell tput bold)
RESET := $(shell tput sgr0)

help:
	@printf "\n  $(BOLD)make all$(RESET)        build the application\n"
	@printf "  $(BOLD)make clean$(RESET)      remove temporary build products\n"
	@printf "  $(BOLD)make pristine$(RESET)   remove all build products\n"
	@printf "  $(BOLD)make help$(RESET)       this help\n\n"

