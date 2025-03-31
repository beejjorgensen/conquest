#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include "defs.h"
#include "structs.h"
#include "vars.h"

void get_tf(tteam tm, int *I0, int starnum)
{
        int i;
        i=1;
        while ( (tf[tm][i].dest!=0) && (i<27) ) i=i+1;
        if ( i==27 )
                i=0;
        else {
                tf[tm][i].s=0;
                tf[tm][i].t=0;
                tf[tm][i].c=0;
                tf[tm][i].b=0;
                tf[tm][i].eta=0;
                tf[tm][i].x = stars[starnum].x;
                tf[tm][i].y = stars[starnum].y;
                tf[tm][i].xf = tf[tm][i].x;
                tf[tm][i].yf = tf[tm][i].y;
                tf[tm][i].dest = starnum;
                tf[tm][i].origeta=0;
                tf[tm][i].blasting = false;
        };
        *I0 = i;
}

void joinsilent(tteam team, struct sttf *parent, struct sttf *child)
{
        parent->t = parent->t + child->t;
        parent->s = parent->s + child->s;
        parent->c = parent->c + child->c;
        parent->b = parent->b + child->b;
        if ( (parent->dest!=0) && (child->dest!=0) )
                tf_stars[parent->dest][team] --;
        child->dest = 0;
}

void lose(int *Ship9s, boolean *L0se_none, char typ, float percent)
{
        int i,sleft;
        int ships;
        boolean lose_none;
        ships = *Ship9s;
        lose_none = *L0se_none;
        if ( ships > 0 ) {
                sleft = ships;
                for ( i = 1 ; i<=ships; i++ ) {
                        if ( ((float)(rand()&0x7fff)/(float)(0x7fff)) > percent ) {
                                lose_none = false;
                                sleft = sleft - 1;
                        };
                };
                if ( sleft < ships ) {
                        printw(" %2d%c", ships-sleft, typ);
                        ships = sleft;
                };
        };
        *Ship9s = ships;
        *L0se_none = lose_none;
}

