#include <stdio.h>
#include <curses.h>
#include "defs.h"
#include "structs.h"
#include "vars.h"
#include "funcs.h"

void print_col(void)
{
    int i;
    boolean see;
    tplanet *pplanet;
    printw("olonies:");
    point(50, 1);
    for (i = 1; i <= nstars; i++) {
        pplanet = stars[i].first_planet;
        while (pplanet != nil) {
            if ((pplanet->team) == player) {
                addch(i + 'A' - 1);
                see = true;
                if (((y_cursor > 21) && (x_cursor >= 50)) ||
                    (y_cursor > 24)) {
                    pause();
                    clear_field();
                    point(50, 1);
                };
                printw("%d:%2d                         ", pplanet->number,
                       pplanet->psee_capacity);
                point(x_cursor + 5, y_cursor);
                x_cursor = x_cursor - 5;
                if (pplanet->psee_capacity == 0)
                    printw(" Decimated");
                else if ((pplanet->team == none) && see)
                    printw(" No colony");
                else if (pplanet->team == player) {
                    printw("(%2d,/%3d)", pplanet->inhabitants,
                           pplanet->iu);
                    if (pplanet->conquered)
                        printw("Con");
                    else
                        printw("   ");
                    if (pplanet->mb != 0)
                        printw("%2dmb", pplanet->mb);
                    else
                        printw("    ");
                    if (pplanet->amb != 0)
                        printw("%2damb", pplanet->amb);
                } else if ((pplanet->team == ENEMY) && see) {
                    printw("*EN*");
                    if (see && pplanet->conquered) {
                        printw("Conquered");
                    } else
                        printw("   ");
                    if (pplanet->under_attack) {
                        if (pplanet->mb != 0)
                            printw("%2dmb", pplanet->mb);
                        else
                            printw("    ");
                        if (pplanet->amb != 0)
                            printw("%2damb", pplanet->amb);
                    };
                };
                point(x_cursor, y_cursor + 1);

            };
            pplanet = pplanet->next;
        };
    };
    clear_field();
    cle3r_left();
}



void starsum(void)
{
    char iline[81];
    int ind, i, value;
    char strs;
    printw("tar summary:");
    cle3r_left();
    point(1, 19);
    addch(':');

    get_line(iline, &ind, true);
    get_token(iline, &ind, &value, &strs);
    point(50, 1);
    if (strs == ' ') {
        for (i = 1; i <= nstars; i++)
            pr5nt_star(i);
    } else {
        do {
            i = strs - 'A' + 1;
            pr5nt_star(i);
            get_token(iline, &ind, &value, &strs);
        }
        while (strs != ' ');
    };
    clear_field();
}

void tfsum(void)
{
    int i, value;
    char tfs;
    char iline[81];
    int ind;
    printw("f summary :");
    get_line(iline, &ind, true);
    get_token(iline, &ind, &value, &tfs);
    point(50, 1);
    if (tfs == ' ') {
        for (i = 1; i <= 26; i++)
            pr3nt_tf(i);
    } else {
        do {
            i = tfs - 'A' + 1;
            pr3nt_tf(i);
            get_token(iline, &ind, &value, &tfs);
        }
        while (tfs != ' ');
    };
    clear_field();
    cle3r_left();
}
