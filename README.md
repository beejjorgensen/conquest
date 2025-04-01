# Conquest

This is an Amiga TUI game that I've modernized (bye, pre-ANSI function
signatures!) and gotten running on Linux and the Mac.

```
  15| .  .  .  .  .  .  .  .  . ?D  .  .  .  .  . |
    | .  .  .  .  .  .  . ?M  .  .  .  .  .  .  . |
    | . ?O  .  . ?J  .  .  .  .  .  .  .  .  .  . |
    | .  .  .  .  .  .  . ?K  .  .  .  .  .  . ?C |
    | .  .  .  .  .  . ?L  .  .  .  .  .  .  .  . |
  10| .  .  .  .  .  .  .  .  .  . ?F  .  .  .  . |
    | .  .  . ?I  .  . ?B  .  .  .  .  .  .  .  . |
    | .  .  .  .  .  .  .  .  .  .  .  .  .  .  . |
    | .  .  .  .  .  .  .  .  .  . ?S  .  .  .  . |
    | .  . ?P  .  .  .  .  .  .  .  .  .  .  .  . |
   5| .  .  .  .  .  .  .  .  . ?Q  .  .  . ?H ?N |
    | .  .  .  .  .  .  .  . ?T  .  .  .  .  .  . |
    | .  .  .  .  . ?U  .  . ?A ?G  .  .  .  .  . |
    | .  .  .  .  .  .  .  .  .  .  . ?R  .  .  . |
   1| .  .  .  .  .  .  .  .  . ?E  .  .  .  .  . |
     ---------------------------------------------
      1           5             10             15
   30?
  TFa:B( 7, 9)  35t               Production yr: 1
  choose your initial fleet.      *Initialization*
  you have 35 transports
   && 30 units to spend
  on ships or research.
```

The gameplay is unchanged. You can git switch to the `original` tag to
get to the original sources.

I still don't know how to play, and there are probably bugs. But at
least it builds!

## Instructions

All I have is what's in [`instructions.txt`](instructions.txt). I don't
actually know how to play. Yet.

## Ports

* Markus Fueger's Ruby port: https://github.com/mfueger/conquest

## History

**Request for information!** If you have any other sources or any
additional info, I'd love to hear it either here on GitHub or at
`beej@beej.us`.

### Thin Leads from the Instructions

The [`instructions.txt`](instructions.txt) file includes this at the
end:

```
6.0 Credits

   Amiga port by Bob Shimbo, orginal author unknown.

   This game started life on a UNIX system at Bell Labs.  It was ported
   to CP/M 80 by a Scott Kamin.  The manual was thrown together in an
   afternoon. (Typos and corrections welcome).

   You can reach me through Compuserve (UID 70260,231) or TBBS of
   colorado (303)-693-4735.
```

...*Bell Labs*?  I'm trying to track down more info, so if you have it,
please send it!

### Original Source

* Source for the Amiga can be found in
  [`archives/Conquest-source.lha`](archives/Conquest-source.lha]). This
  archive also contains the Amiga executables. I haven't tried them in
  an emulator, but I'll bet they work.

### Fish Disk info

This is apparently from [Fish
Disk](https://en.wikipedia.org/wiki/Fred_Fish#The_Amiga_Library_Disks)
[#10](https://www.amiga-stuff.com/pd/fish.html):

```
CONTENTS OF DISK 10
===================

conquest        You control an interstellar empire, decide which star
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
  * Sometimes this causes your units to jump to gigantic values so
    there's UB in there somewhere.
* Can't backspace properly over first entered character

