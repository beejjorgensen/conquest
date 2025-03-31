#include <stdio.h>
#include <stdlib.h>
#include <curses.h>

#include "defs.h"
#include "structs.h"
#include "vars.h"
#include "funcs.h"

int main(void)
{
    initscr();

    printw("\n *** CONQUEST *** \n");
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

    endwin();
}

void point(int col, int row)
{
    move(row - 1, col - 1);     // curses is 0-based
    x_cursor = col;
    y_cursor = row;
    if ((x_cursor < 20) && (y_cursor != 18))
        left_line[y_cursor] = true;
}

int rnd(int i)
{
    int k;
    k = lrand48() / 2;
    k = (k % i) + 1;
    return (k);
}

int conq_round(float x)
{
    if (x < 0.0)
        return ((int) (x - .5));
    else
        return ((int) (x + .5));
}

int min(int a, int b)
{
    return a < b ? a : b;
}
