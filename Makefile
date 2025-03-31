CC = cc
CFLAGS = -Wall -Wextra
LDFLAGS = -lcurses -lm

OBJ = conqmain.o conqmp.o conqsubs.o conqs1.o conq2.o conq3.o \
	conq4.o conq1.o conqtf.o conqlib.o conqbat.o conqb2.o \
	conqi2.o conqd3.o conqs3.o conqpl.o conqmc.o conqve.o \
	conqmo.o conqin.o conqglobals.o

conquest: $(OBJ)
		$(CC) -o $@ $(LDFLAGS) $(OBJ)

conqmain.o: conqmain.c vars.h structs.h defs.h funcs.h
		$(CC) -c $(CFLAGS) $<

conqmp.o: conqmp.c vars.h structs.h defs.h
		$(CC) -c $(CFLAGS) $<

conqsubs.o: conqsubs.c vars.h structs.h defs.h
		$(CC) -c $(CFLAGS) $<

conqs1.o: conqs1.c vars.h structs.h defs.h
		$(CC) -c $(CFLAGS) $<

conq2.o: conq2.c vars.h structs.h defs.h
		$(CC) -c $(CFLAGS) $<

conq3.o: conq3.c vars.h structs.h defs.h
		$(CC) -c $(CFLAGS) $<

conq4.o: conq4.c vars.h structs.h defs.h
		$(CC) -c $(CFLAGS) $<

conq1.o: conq1.c vars.h structs.h defs.h
		$(CC) -c $(CFLAGS) $<

conqtf.o: conqtf.c vars.h structs.h defs.h
		$(CC) -c $(CFLAGS) $<

conqlib.o: conqlib.c vars.h structs.h defs.h
		$(CC) -c $(CFLAGS) $<

conqbat.o: conqbat.c vars.h structs.h defs.h
		$(CC) -c $(CFLAGS) $<

conqb2.o: conqb2.c vars.h structs.h defs.h
		$(CC) -c $(CFLAGS) $<

conqi2.o: conqi2.c vars.h structs.h defs.h
		$(CC) -c $(CFLAGS) $<

conqd3.o: conqd3.c vars.h structs.h defs.h
		$(CC) -c $(CFLAGS) $<

conqs3.o: conqs3.c vars.h structs.h defs.h
		$(CC) -c $(CFLAGS) $<

conqpl.o: conqpl.c vars.h structs.h defs.h
		$(CC) -c $(CFLAGS) $<

conqmc.o: conqmc.c vars.h structs.h defs.h
		$(CC) -c $(CFLAGS) $<

conqve.o: conqve.c vars.h structs.h defs.h
		$(CC) -c $(CFLAGS) $<

conqmo.o: conqmo.c vars.h structs.h defs.h
		$(CC) -c $(CFLAGS) $<

conqin.o: conqin.c vars.h structs.h defs.h
		$(CC) -c $(CFLAGS) $<

conqglobals.o: conqglobals.c
		$(CC) -c $(CFLAGS) $<

.PHONY: clean

clean:
	rm -f *.o
