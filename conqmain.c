#define GLOBAL_DEFINITION_OR_EXTERN /**/

#include "defs.h"
#include "structs.h"
#include "vars.h"
#include <stdio.h>

main()

{
        raw_fd = Open("RAW:539/167/100/32/", 1006);
        printf("\n *** CONQUEST *** \n");
        initconst();
        initmach();
        do {
                inputplayer();
                if (!game_over) {
                        inputmach();
                        move_ships();
                        battle();
                        if (production_year == 4 && turn < 100) {
                                invest();
              }
                        up_year();
                }
                check_game_over();
        } 
        while (!game_over);
        Close(raw_fd);
}

point(col,row)
{
        switch ( terminal_type ) {
        case adm3: 
                printf("\33=%c%c", row+31, col+31);
                break;
        case vt52: 
                printf("\33[%d;%dH", row, col);
                break;
        case vis400:
                printf("\33[%d;%dH", row, col);
                break;
        case concept:
                printf("\33a%c%c", row+31, col+31);
                break;
        default: 
                putchar('\n');
        };
        x_cursor = col;
        y_cursor = row;
        if ( (x_cursor < 20) && (y_cursor != 18) )
                left_line[y_cursor] = true;
}

int

rnd(i)

int i;

{
        int k;
        k = lrand48() / 2;
        k = (k % i) + 1;
        return(k);
}

int
round(x)
float x;
{
        if ( x < 0.0)
                return ((int)(x-.5));
        else
                return ((int)(x+.5));
}

