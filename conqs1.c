#include <stdio.h>
#include <curses.h>
#include <math.h>
#include "defs.h"
#include "structs.h"
#include "vars.h"
#include "funcs.h"

void set_des(int tf_num, boolean *Err0r)
{
        boolean error;
        int st_num,min_eta; 
        char istar; 
        float r;
        int dst;
        int from_star;
        if ( (tf[player][tf_num].eta != 0) ) { /*cancel*/
                tf[player][tf_num].eta = 0;
                from_star = 
                  board[tf[player][tf_num].x][tf[player][tf_num].y].star-'A'+1;
                tf[player][tf_num].dest = from_star;
                tf_stars[from_star][player]++;
                printw("(Cancelling previous orders)");
                point(1,y_cursor + 1);
        };
        error = true;
        printw(" to star:");
        point(10,y_cursor);
        get_char(&istar);
        st_num = istar-'A'+1;
        if ( (st_num<0) || (st_num>nstars) ) {
                error_message();
                printw("  !illegal star");
        }
        else {
                r=sqrt((float) (((stars[st_num].x-tf[1][tf_num].x)*
                        (stars[st_num].x-tf[1][tf_num].x)) +
                        ((stars[st_num].y-tf[1][tf_num].y) * 
                        (stars[st_num].y-tf[1][tf_num].y))));
                point(1,y_cursor + 1);
                printw("   distance:%5.1f", r);
                dst = r - 0.049 + 1;
                if ( ( dst  > range[player] ) &&
                        ( (tf[1][tf_num].b != 0) ||
                                (tf[1][tf_num].c != 0) ||
                                (tf[1][tf_num].t != 0) ) ) {
                        error_message();
                        printw("  !maximum range is %2d",
                        range[player]);
                }
                else if ( r < 0.5 ) {
                        point(1,y_cursor + 1);
                        printw("Tf remains at star");
                }
                else {
                        min_eta= ( (dst-1) / vel[player]) + 1;
                        point(1,y_cursor + 1);
                        printw("eta in %2d turns", min_eta);
                        tf_stars[tf[player][tf_num].dest][player]--;
                        tf[player][tf_num].dest=st_num;
                        tf[player][tf_num].eta = min_eta;
                        tf[player][tf_num].origeta= tf[player][tf_num].eta;
                        tf[player][tf_num].xf= tf[player][tf_num].x;
                        tf[player][tf_num].yf=tf[player][tf_num].y;
                        error = false;
                }
        }
        *Err0r = error;
}

