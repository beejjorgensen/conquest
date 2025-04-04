#include <stdio.h>
#include <curses.h>
#include "defs.h"
#include "structs.h"
#include "vars.h"
#include "funcs.h"

#define new(x) x=(tplanet *)alloc(sizeof(tplanet))

void battle(void)
{
    boolean first;
    int starnum;
    first = true;
    for (starnum = 1; starnum <= nstars; starnum++) {
        if (tf_stars[starnum][ENEMY] > 0
            && tf_stars[starnum][player] > 0
            && (any_bc(ENEMY, starnum) || any_bc(player, starnum))
            ) {
            if (first) {
                point(33, 20);
                printw("* Tf battle *   ");
                first = false;
            };
            tf_battle(starnum);
        };
        if ((any_bc(ENEMY, starnum)) && (col_stars[starnum][player]
                                         > 0))
            EN2MY_attack(starnum);
        else if ((tf_stars[starnum][player] > 0)
                 && (col_stars[starnum][ENEMY] > 0))
            pl2yerattack(starnum);
    };
}
