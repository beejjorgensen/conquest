CC =		cc
CFLAGS =
LDFLAGS =

# Due to deficiencies in the public domain make, with no continuation
# lines, macros must be only one line.  Also, since execute() can't
# handle lines longer than about 200 chars, must use join to put
# the objects into archives.  Sigh.  Unix wherefore art thou...

OBJ1 =		conqmain.o conqmp.o conqsubs.o conqs1.o conq2.o conq3.o
OBJ2 =		conq4.o conq1.o conqtf.o conqlib.o conqbat.o conqb2.o
OBJ3 =		conqi2.o conqd3.o conqs3.o conqpl.o conqmc.o conqve.o
OBJ4 =		conqmo.o conqin.o
OBJ =		obj1.o obj2.o obj3.o obj4.o

conquest :	$(OBJ)
		$(CC) -o conquest $(LDFLAGS) $(OBJ)

obj1.o :	$(OBJ1)
		join $(OBJ1) as obj1.o

obj2.o :	$(OBJ2)
		join $(OBJ2) as obj2.o

obj3.o :	$(OBJ3)
		join $(OBJ3) as obj3.o

obj4.o :	$(OBJ4)
		join $(OBJ4) as obj4.o

conqmain.o :	conqmain.c vars.h structs.h defs.h
		$(CC) -c $(CFLAGS) conqmain.c

conqmp.o :	conqmp.c vars.h structs.h defs.h
		$(CC) -c $(CFLAGS) conqmp.c

conqsubs.o :	conqsubs.c vars.h structs.h defs.h
		$(CC) -c $(CFLAGS) conqsubs.c

conqs1.o :	conqs1.c vars.h structs.h defs.h
		$(CC) -c $(CFLAGS) conqs1.c

conq2.o :	conq2.c vars.h structs.h defs.h
		$(CC) -c $(CFLAGS) conq2.c

conq3.o :	conq3.c vars.h structs.h defs.h
		$(CC) -c $(CFLAGS) conq3.c

conq4.o :	conq4.c vars.h structs.h defs.h
		$(CC) -c $(CFLAGS) conq4.c

conq1.o :	conq1.c vars.h structs.h defs.h
		$(CC) -c $(CFLAGS) conq1.c

conqtf.o :	conqtf.c vars.h structs.h defs.h
		$(CC) -c $(CFLAGS) conqtf.c

conqlib.o :	conqlib.c vars.h structs.h defs.h
		$(CC) -c $(CFLAGS) conqlib.c

conqbat.o :	conqbat.c vars.h structs.h defs.h
		$(CC) -c $(CFLAGS) conqbat.c

conqb2.o :	conqb2.c vars.h structs.h defs.h
		$(CC) -c $(CFLAGS) conqb2.c

conqi2.o :	conqi2.c vars.h structs.h defs.h
		$(CC) -c $(CFLAGS) conqi2.c

conqd3.o :	conqd3.c vars.h structs.h defs.h
		$(CC) -c $(CFLAGS) conqd3.c

conqs3.o :	conqs3.c vars.h structs.h defs.h
		$(CC) -c $(CFLAGS) conqs3.c

conqpl.o :	conqpl.c vars.h structs.h defs.h
		$(CC) -c $(CFLAGS) conqpl.c

conqmc.o :	conqmc.c vars.h structs.h defs.h
		$(CC) -c $(CFLAGS) conqmc.c

conqve.o :	conqve.c vars.h structs.h defs.h
		$(CC) -c $(CFLAGS) conqve.c

conqmo.o :	conqmo.c vars.h structs.h defs.h
		$(CC) -c $(CFLAGS) conqmo.c

conqin.o :	conqin.c vars.h structs.h defs.h
		$(CC) -c $(CFLAGS) conqin.c

