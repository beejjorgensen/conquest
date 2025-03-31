#include <stdio.h>
#include <curses.h>
#include "defs.h"
#include "structs.h"
#include "vars.h"

void get_char(char *C)
{
char result;
result = getch();
if(result == 0x0d) result = '\n';
*C = toupper(result);
printw("%c", *C);
}

void get_line(char *iline, int *Ind0, int onech)
{
        char ch;
        int ind;

        ind=1;
        do {
                get_char(&ch);
                if ( ch == '\b' ) { /*backspace*/
                        if ( ind!=1 ) {
                                ind = ind - 1;
                                if ( (ind != 1) && onech ) {
                                        addch('\b');
                                        ind = ind - 1;
                                };
                                if ((ind != 1) && !onech) {
                                        addch(' ');
                                        addch('\b');
                                }
                        };
                } 
                else if ( ch != '\n' ) {
                        iline[ind] = ch;
                        ind = ind + 1;
                        if ( onech ) {
                                addch(' ');
                                iline[ind] = ' ';
                                ind = ind + 1;
                        };
                };
        } 
        while (ind < 25 && ch != '\n');
        while (ind != 80) {
                iline[ind] = ' ';
                ind = ind + 1;
        };
        *Ind0 = 1;
}

void get_stars(int s_star, float slist[nstars+1], int *C0unt)
{
        int starnum, count;
        float this_range, range2;

        count=0;
        for ( starnum = 1 ; starnum <= nstars; starnum++ ) {
                if ( range[0] >= r2nge[s_star][starnum] ) {
                        count=count+1;
                        slist[starnum] = r2nge[s_star][starnum];
                } 
                else
                        slist[starnum] = 0;
        };
        *C0unt = count;
}

void clear_field(void)
{
        int new_bottom, y;
        new_bottom = y_cursor - 1;
        if ( new_bottom < bottom_field ) {
                        for ( y = new_bottom + 1  ; y<=bottom_field; y++ ) {
                                point(50,y);
								printw(blank_line);
                        };
        };
        bottom_field = new_bottom;
}

void cle3r_left(void)
{
        int i;
		for ( i = 19 ; i<=24; i++ ) {
				if ( left_line[i] ) {
						point(1,i);
						printw(blank_line);
						left_line[i] = false;
				}
		}
}



void cle2r_screen(void)
{
		clear();
        point(1,1);
}



void error_message(void)
{
        point(1,24);
}

void get_token(char *line, int *Index0, int *Value0, char *Token0)
{
        int index, value; 
        char token;
        index = *Index0;
        value = 0;
        token = ' ';
        while (line[index]==' ' && (index < 80) ) index = index+1;
        if ( index<80 ) {
                if ( (line[index] < '0') || (line[index] > '9') )
                        value = 1;
                else {
                        while (line[index] >='0' && (line[index] <= '9') ) {
                                value = 10*value + line[index] - '0';
                                index = index+1;
                        };
                };
                token = line[index];
                index = index+1;
        }
        while (line[index]!=' ' && (index<80) ) index= index+1;
        while (line[index] == ' ' && (index < 80) ) index = index+1;
        *Index0 = index; 
        *Value0 = value; 
        *Token0 = token;
}


struct helpst {
        char *cmd, *does;
};

struct helpst help0[] = {
        {       "B",    "Bld Battlestar(s)    75"},
        {       "C",    "Bld Cruiser(s)       16"},
        {       "H",    "Help"                   },
        {       "R",    "Range Research"         },
        {       "S",    "Bld Scout(s)          6"},
        {       "V",    "Velocity Research"      },
        {       "W",    "Weapons Research"       },
        {       ">M",   "Redraw Map"             },
        {       ">R",   "Research summary"       },
        {       0,      0                        }
};

struct helpst help1[] = {
        {       "B",    "Blast Planet"           },
        {       "C",    "Colony summary"         },
        {       "D",    "TaskForce Destination"  },
        {       "G",    "Go on (done)"           },
        {       "H",    "Help"                   },
        {       "J",    "Join TaskForces"        },
        {       "L",    "Land transports"        },
        {       "M",    "Redraw Map"             },
        {       "N",    "New TaskForce"          },
        {       "Q",    "Quit"                   },
        {       "R",    "Research summary"       },
        {       "S",    "Star summary"           },
        {       "T",    "TaskForce summary"      },
        {       0,      0                        }
};

struct helpst help2[] = {
        {       "C",    "Colonies"               },
        {       "G",    "Go on (done)"           },
        {       "H",    "Help"                   },
        {       "M",    "Map"                    },
        {       "O",    "Odds"                   },
        {       "R",    "Research summary"       },
        {       "S",    "Star summary"           },
        {       "T",    "TaskForce summary"      },
        {       "W",    "Withdraw"               },
        {       0,      0,                       }
};

struct helpst help3[] = {
        {       "B",    "Break off Attack"       },
        {       "C",    "Colony summary"         },
        {       "G",    "Go on (done)"           },
        {       "H",    "Help"                   },
        {       "J",    "Join TFs"               },
        {       "M",    "Redraw Map"             },
        {       "N",    "New TF"                 },
        {       "R",    "Research summary"       },
        {       "S",    "Star summary"           },
        {       "T",    "TaskForce summary"      },
        {       0,      0                        }
};

struct helpst help4[] = {
        {       "A",    "Bld Adv. Missle Base 35"},
        {       "B",    "Bld Battlestar(s)    70"},
        {       "C",    "Bld Cruiser(s)       16"},
        {       "H",    "Help"                   },
        {       "I",    "Invest                3"},
        {       "M",    "Bld Missle Base(s)    8"},
        {       "R",    "Range Research"         },
        {       "S",    "Bld Scout(s)          6"},
        {       "T",    "Bld Transports"         },
        {       "V",    "Vel Research"           },
        {       "W",    "Weapons Research"       },
        {       ">C",   "Colony summary"         },
        {       ">M",   "Redraw Map"             },
        {       ">R",   "Research summary"       },
        {       ">S",   "Star summary"           },
        {       0,      0                        }
};

void help(int which)
{
        struct helpst *h;
        int j;

        j = 1;
        point(50, j++);
        if (which == 0) h = help0;
        if (which == 1) h = help1;
        if (which == 2) h = help2;
        if (which == 3) h = help3;
        if (which == 4) h = help4;
        while (h->cmd != 0) {
                printw("%2s - %-25s", h->cmd, h->does);
                point(50, j++);
                h++;
        }
        clear_field();
}


void on_board(int x, int y)
{
        int i;
        int starnum;
        board[x][y].tf=' ';
        i=1;
        do {
                if ( (tf[player][i].dest!=0) && (tf[player][i].x==x) 
                                                        && (tf[player][i].y==y
                    ) ) {
                        if ( board[x][y].tf==' ' )
                                board[x][y].tf= i+'a'-1;
                        else {
                                board[x][y].tf='*';
                                i=27;
                        };
                };
                i=i+1;
        } 
        while (i <= 26);
        if ( board[x][y].star != '.' ) {
                board[x][y].enemy = ' ';
                starnum= board[x][y].star - 'A' +1;
                if ( col_stars[starnum][player] != 0 )
                        board[x][y].enemy = '@';
        };
        update_board(x,y,both);
}

void pause(void)
{
        char dummy;
        point(1,18);
        printw("Press any key to continue  ");
        get_char(&dummy);
}


void pr3nt_tf(int i)
{
        int x, y;
        if ( (i!=0) && (i<27) ) {
                if ( tf[player][i].dest != 0 ) {
                        printw("TF%c:", i+'a'-1);
                        x=tf[player][i].x; 
                        y=tf[player][i].y;
                        if ( tf[player][i].eta==0 )
                                addch(tf[player][i].dest+'A'-1);
                        else
                                addch(' ');
                        printw("(%2d,%2d)               ",x,y);
                        point(x_cursor + 14, y_cursor);
                        x_cursor = x_cursor - 14;
                        disp_tf(&tf[player][i]);
                        if ( tf[player][i].eta != 0 ) {
                                printw("%c2m", 0x9b);
                                printw("%c%d",
                                tf[player][i].dest+'A'-1, tf[player][i].eta);
                                printw("%c0m", 0x9b);
                        };
                        point(x_cursor, y_cursor+1);
                };
        };
}


void pr5nt_star(int stnum)
{
        boolean see;
        int i, x, y;
        tplanet *p;
        if ( (stnum!=0) && (stnum<=nstars) ) {
                if ( (y_cursor + 3 + tf_stars[stnum][player] + tf_stars[stnum][
                    ENEMY] ) > 19 ) {
                        clear_field();
                        pause();
                        point(50,1);
                };
                if ( stars[stnum].visit[player]==true ) {
                        see=false;
                        printw("----- star %c -----            ", stnum+'A'-1)
                                ;
                        point(50, y_cursor + 1);
                        x=stars[stnum].x;
                        y=stars[stnum].y;
                        if ( tf_stars[stnum][player] !=0 ) {
                                see = true;
                                for ( i=1 ; i<=26; i++ ) {
                                        if (tf[player][i].dest==stnum && tf[player][i].eta == 0)
                                        {
                                                printw("TF%c                           ", i+'a'-1);
                                                point(55,y_cursor);
                                                disp_tf(&tf[player][i]);
                                                point(50, y_cursor + 1);
                                        };
                                };
                        };
                        if ( ! see )
                                see = col_stars[stnum][player] != 0;
                        if ( see && (tf_stars[stnum][ENEMY] != 0) ) {
                                i=1;
                                while(tf[ENEMY][i].eta!=0 ||
                                   (tf[ENEMY][i].dest!=stnum) )
                                        i=i+1;
                                printw(" EN:                          ");
                                point(55,y_cursor);
                                disp_tf(&tf[ENEMY][i]);
                                point( 50, y_cursor + 1);
                        };
                        p=stars[stnum].first_planet;
                        if ( p==nil ) {
                                printw("  no useable planets          ");
                                point(50,y_cursor + 1);
                        }
                        else {
                                while ( p!=nil ) {
                                        addch(' ');
        if ( ((y_cursor > 21) && (x_cursor >= 50)) ||
            (y_cursor > 24) ) {
                pause();
                clear_field();
                point(50,1);
        };
        printw("%d:%2d                         ", p->number, p->psee_capacity);
        point(x_cursor + 5, y_cursor);
        x_cursor = x_cursor - 5;
        if ( p->psee_capacity==0 )
                printw(" Decimated");
        else if ( (p->team==none) && see )
                printw(" No colony");
        else if ( p->team==player ) {
                printw("(%2d,/%3d)", p->inhabitants, p->iu);
                if ( p->conquered )
                        printw("Con");
                else
                        printw("   ");
                if ( p->mb!=0 )
                        printw("%2dmb", p->mb);
                else
                        printw("    ");
                if ( p->amb!=0 )
                        printw("%2damb", p->amb);
        } 
        else if ( (p->team==ENEMY) && see ) {
                printw("*EN*");
                if ( see && p->conquered ) {
                        printw("Conquered");
                } 
                else
                        printw("   ");
                if ( p->under_attack ) {
                        if ( p->mb != 0 )
                                printw("%2dmb", p->mb);
                        else
                                printw("    ");
                        if ( p->amb != 0 )
                                printw("%2damb", p->amb);
                };
        };
        point(x_cursor,y_cursor + 1);

                                        p=p->next;
                                };
                        }
                }
        }
}


void swap(int *a, int *b)
{
        int t;

        t = *a;
        *a = *b;
        *b = t;
}

