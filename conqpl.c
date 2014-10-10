#include <stdio.h>
#include <curses.h>
#include "defs.h"
#include "structs.h"
#include "vars.h"

bl2st_planet()
{
        char tf_char, pl_char; 
        int tf_num, planet_num;
        tplanet *pplanet; 
        int factors,starnum; 
        boolean see, done;
        char iline[81]; 
        int ind,amount; 
        char dum;
        printw("last");
        cle3r_left();
        point(1,19);
        printw("Firing TF:");
        get_char(&tf_char);
        tf_num = tf_char-'A'+1;
        if ( (tf_num < 1) || (tf_num > 26) ) {
                error_message();
                printw(" !Illegal tf");
        }
        else if ( tf[player][tf_num].dest==0 ) {
                error_message();
                printw(" !Nonexistent tf");
        }
        else if ( tf[player][tf_num].eta!=0 ) {
                error_message();
                printw(" !Tf is not in normal space   ");
        }
        else if ( tf[player][tf_num].blasting ) {
                error_message();
                printw(" !Tf is already blasting     ");
        }
        else if ( (tf[player][tf_num].b == 0) && (tf[player][tf_num].c == 0) ) {
                error_message();
                printw(" !Tf has no warships         ");
        }
        else {
                starnum = tf[player][tf_num].dest;
                pplanet = stars[starnum].first_planet;
                if ( pplanet==nil ) {
                        error_message();
                        printw(" !No planets at star %c       ", 
                                                        starnum + 'A' - 1);
                }
                else {
                        point(1,20);
                        printw("Target colony ");
                        if ( pplanet->next== nil ) {
                                printw("%2d",pplanet->number);
                        }
                        else {
                                printw(":");
                                get_char(&pl_char);
                                planet_num = pl_char -'0';
                                done = false;
                                while ( ! done ) {
                                        if ( (pplanet->number==planet_num)  )
                                                done = true;
                                        else {
                                                if ( pplanet->next==nil )
                                                        done = true;
                                                else
                                                        pplanet = pplanet->next;
                                        };
                                };
                                if ( pplanet->number != planet_num ) {
                                     error_message();
                                     printw(" !No such planet at this star ");
                                     pplanet = nil;
                                };
                        };
                        if ( pplanet != nil ) {
                                if ( pplanet->team==ENEMY ) {
                                        error_message();
                                        printw(" !Conquer it first!");
                                }
                                else if ( (pplanet->team==player) 
                                                && (! pplanet->conquered)
                                    ) {
                                        error_message();
                                        printw(" !That is a human colony!!    ");
                                }
                                else {
                                   factors = weapons[player] 
                                        * ((tf[player][tf_num].c * c_guns) +
                                            (tf[player][tf_num].b * b_guns));
                                        point(1,21);
                                        printw("Units (max %3d) :", factors/4);
                                        point(18,21);
                                        get_line(iline,&ind,false);
                                        get_token(iline,&ind,&amount,&dum);
                                        if ( amount < 0 )
                                           factors = 0;
                                        else if ( amount > 0 )
                                           factors = min (factors, amount * 4);
                                        tf[player][tf_num].blasting = true;
                                        point(1,22);
                                        printw("Blasting %3d units", factors/4);
                                        blast(pplanet,factors);
                                        point(1,23);
                                        left_line[23] = true;
                                        addch(pplanet->pstar+'A'-1);
                                        pplanet->psee_capacity = pplanet->capacity;
                                        see = true;
        if ( ((y_cursor > 21) && (x_cursor >= 50)) ||
            (y_cursor > 24) ) {
                pause();
                clear_field();
                point(50,1);
        };
        printw("%d:%2d                         ", pplanet->number, pplanet->psee_capacity);
        point(x_cursor + 5, y_cursor);
        x_cursor = x_cursor - 5;
        if ( pplanet->psee_capacity==0 )
                printw(" Decimated");
        else if ( (pplanet->team==none) && see )
                printw(" No colony");
        else if ( pplanet->team==player ) {
                printw("(%2d,/%3d)", pplanet->inhabitants, pplanet->iu);
                if ( pplanet->conquered )
                        printw("Con");
                else
                        printw("   ");
                if ( pplanet->mb!=0 )
                        printw("%2dmb", pplanet->mb);
                else
                        printw("    ");
                if ( pplanet->amb!=0 )
                        printw("%2damb", pplanet->amb);
        } 
        else if ( (pplanet->team==ENEMY) && see ) {
                printw("*EN*");
                if ( see && pplanet->conquered ) {
                        printw("Conquered");
                } 
                else
                        printw("   ");
                if ( pplanet->under_attack ) {
                        if ( pplanet->mb != 0 )
                                printw("%2dmb", pplanet->mb);
                        else
                                printw("    ");
                        if ( pplanet->amb != 0 )
                                printw("%2damb", pplanet->amb);
                };
        };
        point(x_cursor,y_cursor + 1);

                                };
                        };
                };
        };
}

inputplayer()
{
        char key; 
        boolean fin;
        point(33,20);
        printw("* Movement *    ");
        fin=false;
        do {
                point(1,18);
                printw("?                             ");
                point(2,18);
                get_char(&key);
                switch ( key ) {
                case 'M': 
                        printmap();
                        break;
                case 'B': 
                        bl2st_planet();
                        break;
                case 'G': 
                case ' ': 
                        fin=true;
                        break;
                case 'H': 
                        help(1);
                        break;
                case 'L': 
                        land();
                        break;
                case 'D': 
                        send_tf();
                        break;
                case 'S': 
                        starsum();
                        break;
                case 'N': 
                        make_tf();
                        break;
                case 'J': 
                        join_tf();
                        break;
                case 'C': 
                        print_col();
                        break;
                case 'R': 
                        ressum();
                        break;
                case 'Q':
                        fin = true;
                        quit();
                        break;
                case '?':
                        break;
                case 'T': 
                        tfsum();
                        break;
                default:
                        error_message();
                        printw("  !illegal command");
                };  /*switch (*/
        } 
        while (!fin);
}



land()
{
        char tfc,trc,planc; 
        boolean see;
        int x,y,room_left,tfnum,transports,planet_num;
        int starnum,ind; 
        char iline[81]; 
        boolean found;
        struct stplanet  *pplanet;
        printw("and tf:");
        get_char(&tfc);
        cle3r_left();
        tfnum = tfc-'A'+1;
        if ( (tfnum<1) || (tfnum>26) ) {
                error_message();
                printw("  !illegal tf");
        }
        else if ( (tf[player][tfnum].dest==0)) {
                error_message();
                printw("  !nonexistent tf");
        }
        else if ( (tf[player][tfnum].eta!=0) ) {
                error_message();
                printw("  !tf is not in normal space  ");
        }
        else {
                starnum= tf[player][tfnum].dest;
                pplanet= stars[starnum].first_planet;
                if ( pplanet==nil ) {
                        error_message();
                        printw("  !no planets at this star    ");
                }
                else if ( tf_stars[starnum][ENEMY]>0 ) {
                        error_message();
                        printw("  !enemy ships present");
                }
                else {
                        point(11,18);
                        printw(" planet ");
                        if ( pplanet->next==nil ) {
                                planet_num = pplanet->number;
                                printw("%d", planet_num);
                        } 
                        else {
                                printw(":");
                                get_char(&planc);
                                planet_num = planc-'0';
                                found=false;
                                while ( (pplanet!= nil) && ! found){
                                        if ( pplanet->number == planet_num )
                                                found=true;
                                        else pplanet = pplanet->next;
                                };
                                if ( ! found ) {
                                        planet_num = 0;
                                        error_message();
                                        printw(" !Not a habitable planet ");
                                }
                        };
                        if ( planet_num != 0 ) {
                                if ( (pplanet->team == ENEMY) || ((pplanet->team == player)
                                    && (pplanet->conquered))  ) {
                                        error_message();
                                        printw("  !Enemy infested planet  !!  ");
                                }
                                else {    /*get the number of transports*/
                                        room_left = pplanet->capacity - pplanet->inhabitants;
                                        point(1,19);
                                        printw(" transports:");
                                        get_line(iline,&ind,false);
                                        get_token(iline,&ind,&transports,&trc);
                                        if ( transports==0 ) transports = tf[player][tfnum].t;
                                        if ( transports < 1 ) {
                                                error_message();
                                                printw("  !illegal transports");
                                        }
                                        else if ( transports > tf[player][tfnum].t ) {
                                                error_message();
                                                printw("  !only %2d transports in tf", tf[player][tfnum].t);
                                        }
                                        else if ( transports > room_left ) {
                                                error_message();
                                                printw("  !only room for %2d transports", room_left);
                                        }
                                        else {
                                                pplanet->team = player;
                                                if ( pplanet->inhabitants==0 ) {
                                                        col_stars[starnum][player]++;
                                                };
                                                pplanet->inhabitants=pplanet->inhabitants+transports;
                                                pplanet->iu=pplanet->iu + transports;
                                                tf[player][tfnum].t=tf[player][tfnum].t-transports;
                                                x=tf[player][tfnum].x; 
                                                y=tf[player][tfnum].y;
                                                if ( board[x][y].enemy == ' ' ) {
                                                        board[x][y].enemy = '@';
                                                        update_board(x,y,left);
                                                };
                                                point(1,20);
                                                addch(starnum+'A'-1);
                                                see = true;
        if ( ((y_cursor > 21) && (x_cursor >= 50)) ||
            (y_cursor > 24) ) {
                pause();
                clear_field();
                point(50,1);
        };
        printw("%d:%2d                         ", pplanet->number, pplanet->psee_capacity);
        point(x_cursor + 5, y_cursor);
        x_cursor = x_cursor - 5;
        if ( pplanet->psee_capacity==0 )
                printw(" Decimated");
        else if ( (pplanet->team==none) && see )
                printw(" No colony");
        else if ( pplanet->team==player ) {
                printw("(%2d,/%3d)", pplanet->inhabitants, pplanet->iu);
                if ( pplanet->conquered )
                        printw("Con");
                else
                        printw("   ");
                if ( pplanet->mb!=0 )
                        printw("%2dmb", pplanet->mb);
                else
                        printw("    ");
                if ( pplanet->amb!=0 )
                        printw("%2damb", pplanet->amb);
        } 
        else if ( (pplanet->team==ENEMY) && see ) {
                printw("*EN*");
                if ( see && pplanet->conquered ) {
                        printw("Conquered");
                } 
                else
                        printw("   ");
                if ( pplanet->under_attack ) {
                        if ( pplanet->mb != 0 )
                                printw("%2dmb", pplanet->mb);
                        else
                                printw("    ");
                        if ( pplanet->amb != 0 )
                                printw("%2damb", pplanet->amb);
                };
        };
        point(x_cursor,y_cursor + 1);

                                                zero_tf(player,tfnum);
                                                pr3nt_tf(tfnum);
                                        };
                                };
                        };
                };
        };
}

quit()
{
        char answer;
        cle2r_screen();
        printw("Quit game....[verify]\n");
        get_char(&answer);
        if ( answer != 'Y' )
                printmap();
        else {
                game_over = true;
        };
}

send_tf()
{
        char tf_move;
        int tf_num;
        boolean error;
        printw("estination tf:");
        get_char(&tf_move);
        cle3r_left();
        point(1,19);
        tf_num = tf_move-'A'+1;
        if ( (tf_num<1) || (tf_num>26) ) { 
                error_message();
                printw(" !illegal tf");
        } 
        else if ( tf[player][tf_num].dest==0 ) { 
                error_message();
                printw(" !nonexistent tf");
        } 
        else if ( (tf[player][tf_num].eta != 0) &&
            ( (tf[player][tf_num].eta != tf[player][tf_num].origeta) ||
            (tf[player][tf_num].withdrew) )
            ) { 
                error_message();
                printw(" !Tf is not in normal space  ");
        } 
        else if ( tf[player][tf_num].blasting ) { 
                error_message();
                printw(" !Tf is blasting a planet");
        } 
        else {
                tf[player][tf_num].withdrew = false;
                set_des(tf_num,&error);
        };
}

