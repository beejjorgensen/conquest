.Key file,opt1,opt2,opt3
;           Compile a C program                       Version 3.00
;           Works with Lattice version 3.02 and above
if not exists <file$t1>.c
   echo "File <file$t1>.c does not exist.  Try again."
   skip END
   endif
echo "-- compiling...<file$t1>.c"

:c/copy <file$t1>.c to ram:
:c/lc1 <opt1> <opt2> <opt3> -i:include/ -i:include/lattice/ ram:<file$t1>
if not exists "ram:<file$t1>.q"
   echo "Compile failed."
   quit 20
   endif
:c/lc2 ram:<file$t1>
:c/delete ram:<file$t1>.c
:c/copy ram:<file$t1>.o to <file$t1>.o
:c/delete ram:<file$t1>.o

LAB END
date >df0:now

