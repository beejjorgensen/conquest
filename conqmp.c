#include "defs.h"
#include "structs.h"
#include "vars.h"
#include <stdio.h>
#include <curses.h>


printmap()
{
        int i1, i2;
        cle2r_screen();
        for ( i1= bdsize ; i1>=1; i1-- ) {
                if ( ((i1==1) || (((i1 / 5)*5)==i1)) )
                        printw("%2d|", i1);
                else
                        printw("  |");
                for ( i2=1 ; i2<=bdsize; i2++ )
                        printw("%c%c%c",board[i2][i1].enemy,board[i2][i1].star,
                        board[i2][i1].tf);
                printw("|\n");
        };
        printw("   ");
        for ( i1 = 1 ; i1<=bdsize; i1++ ) printw("---");
        addch('\n');
        printw("   ");
        for ( i1 = 1 ; i1<=bdsize; i1++ ) {
                if ( ((i1==1) || (((i1 / 5) * 5) == i1)) )
                        printw("%2d ", i1);
                else
                        printw("   ");
        };
        addch('\n');
        point(33,18);
        printw("Turn: %3d",turn);
        point(33,19);
        printw("Production yr: %d",production_year);
        bottom_field = 0;
        for ( i1 = 19 ; i1<=24; i1++ )
                left_line[i1] = false;
}

