#include <stdio.h>
#include "defs.h"
#include "structs.h"
#include "vars.h"
#include "funcs.h"

void revolt(int starnum)
{
    tplanet *pplan;
    tteam loses, gets_back;
    pplan = stars[starnum].first_planet;
    if (col_stars[starnum][ENEMY] + col_stars[starnum][player] > 0) {
        while (pplan != nil) {
            if (pplan->conquered) {
                if ((pplan->team == player) && (!any_bc(player, starnum))) {
                    loses = player;
                    gets_back = ENEMY;
                } else if ((pplan->team == ENEMY) &&
                           (!any_bc(ENEMY, starnum))) {
                    loses = ENEMY;
                    gets_back = player;
                } else
                    loses = none;
                if (loses != none) {
                    col_stars[starnum][loses] =
                        col_stars[starnum][loses] - 1;
                    col_stars[starnum][gets_back]++;
                    pplan->team = gets_back;
                    pplan->conquered = false;
                    pplan->psee_capacity = pplan->capacity;
                    on_board(stars[starnum].x, stars[starnum].y);
                };
            };
            pplan = pplan->next;
        };
    };
}
