# Conquest

This is an Amiga TUI game that I've modernized (to ANSI C!) and ported
to Linux and the Mac.

The gameplay is unchanged. You can git switch to the `original` tag to
get to the original sources.

I still don't know how to play, and there are probably bugs. But at
least it builds!

**Request for information!** If you have any other sources or any
additional information, I'd love to hear it either here on GitHub or at
`beej@beej.us`.

## Ports

* Markus Fueger's Ruby port: https://github.com/mfueger/conquest

## Original Source

* Source for the Amiga can be found in `archives/Conquest-source.lha`.
  This archive also contains the Amiga executables. I haven't tried them
  in an emulator, but I'll bet theyu works.

## Fish Disk info

This is apparently from [Fish
Disk](https://en.wikipedia.org/wiki/Fred_Fish#The_Amiga_Library_Disks)
[#10](https://www.amiga-stuff.com/pd/fish.html):

```
CONTENTS OF DISK 10
===================

conquest   You control an interstellar empire, decide which star
           systems to explore, which planets to colonize, etc. The
           computer will also be building its own empire and
           competing with you for resources. The one with the
           greatest population at the end wins. Executable only,
           no source. Downloaded from Denver BBS. Author: Unknown,
           ported to Amiga by Rob Shimbo
```

This is the `README` from the Amiga archive:

```
There are two executables provided here:

        conquest.orig   Executable received from Bob Shimbo with the
                        source disk.

        conquest        Executable built by me using my current
                        Lattice 3.03 compiler and the supplied sources.

I have not attempted to determine why they are different sizes, or
if the size difference is significant.  The both run to the extent
that you can start them up and then "QUIT" successfully.

-Fred    5-May-86
```

## Bugs in my Unix port

* Backspacing too far on a line can move the cursor effectively under
  the prompt
  * Sometimes this causes you units to jump to gigantic values so
    there's UB in there somewhere.
* Can't backspace properly over first entered character

