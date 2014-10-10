#include <stdio.h>
#include "defs.h"
#include "structs.h"
#include "vars.h"

tf_battle(starnum)
int starnum;
{
        int ennum,plnum; 
        float enodds,plodds; 
        boolean battle;
        int count,new_tf,i;  
        char ch;
        boolean pla_loss,ene_loss;
        int size; 
        tteam team;
        int dstar;
        float slist[nstars+1];
        boolean fin, first;
        board[stars[starnum].x][ stars[starnum].y].enemy = '!';
        update_board(stars[starnum].x, stars[starnum].y, left);
        ennum=1;
        while ((tf[ENEMY][ennum].dest!=starnum)||(tf[ENEMY][ennum].eta!=0)
            ) ennum = ennum+1;
        plnum=1;
        if ( tf_stars[starnum][player]>1 ) {
                get_tf(player,&new_tf,starnum);
                for ( i= 1 ; i<=26; i++ ) {
                        if ( (tf[player][i].dest == starnum) && (tf[player][i].eta==0)
                            && (i != new_tf) )
                                joinsilent(player, &tf[player][new_tf],&tf[player][i]);
                };
                tf_stars[starnum][player] = 1;
                plnum=new_tf;
        } 
        else {
                while ((tf[player][plnum].dest!=starnum)||(tf[player][plnum].eta!=0)
                    ) plnum=plnum+1;
        };
        display_forces(ennum,plnum,&enodds,&plodds,&battle);
        pause();
        first = true;
        while ( battle ) {
                if ( left_line[24] ) {
                        point(1,24);
                        printf(blank_line);
                        left_line[24] = false;
                };
                pla_loss = true;
                ene_loss = true;
                point(1,21);
                printf(" Enemy losses:                ");
                point(1,22);
                printf("Player losses:                ");
                do {
                        point(15,21);
                        lose(&tf[ENEMY][ennum].t,&ene_loss,'t',enodds);
                        lose(&tf[ENEMY][ennum].s,&ene_loss,'s',enodds);
                        lose(&tf[ENEMY][ennum].c,&ene_loss,'c',enodds);
                        lose(&tf[ENEMY][ennum].b,&ene_loss,'b',enodds);
                        point(15,22);
                        lose(&tf[player][plnum].t,&pla_loss,'t',plodds);
                        lose(&tf[player][plnum].s,&pla_loss,'s',plodds);
                        lose(&tf[player][plnum].c,&pla_loss,'c',plodds);
                        lose(&tf[player][plnum].b,&pla_loss,'b',plodds);
                } 
                while (!first && ene_loss && pla_loss);
                if ( ene_loss ) {
                        point(15,21);
                        printf("(none)");
                };
                if ( pla_loss ) {
                        point(15,22);
                        printf("(none)");
                };
                first = false;
                display_forces(ennum,plnum,&enodds,&plodds,&battle);
                if ( battle ) {
                        /*withdraw the bad guys*/
                        get_tf(ENEMY,&new_tf,starnum);
                        if ( (tf[player][plnum].c>0) || (tf[player][plnum].b>0)
                            ) {
                                tf[ENEMY][new_tf].t = tf[ENEMY][ennum].t;
                                tf[ENEMY][new_tf].s = tf[ENEMY][ennum].s;
                                best_plan(&starnum,&size,&team);
                                if ( ((enodds<0.7) && (size<30) )|| ((enodds<0.50)
                                    && (team==player)) || ((enodds <0.30) && (size < 60)) 
                                        || (enodds < 0.20)  ) {
                                        tf[ENEMY][new_tf].c = tf[ENEMY][ennum].c;
                                        tf[ENEMY][new_tf].b = tf[ENEMY][ennum].b;
                                };
                        };
                        if((tf[ENEMY][new_tf].t + tf[ENEMY][new_tf].s +
                                tf[ENEMY][new_tf].c + tf[ENEMY][new_tf].b) > 0) {
                                get_stars(starnum,slist,&count);
                                do {
                                        dstar = rnd(nstars);
                                } 
                                while (slist[dstar] <= 0);
                                tf[ENEMY][new_tf].dest = dstar;
                                tf[ENEMY][new_tf].eta = (int)((slist[dstar]-0.01)/vel[ENEMY])+1;
                                tf[ENEMY][new_tf].xf=stars[starnum].x;
                                tf[ENEMY][new_tf].yf=stars[starnum].y;
                        } 
                        else tf[ENEMY][new_tf].dest = 0;

                        fin = false;
                        do {
                                point(1,18);
                                printf("B?                            ");
                                point(3,18);
                                get_char(&ch);
                                switch ( ch ) {
                                case 'M':
                                        printmap();
                                        break;
                                case 'H': 
                                        help(2);
                                        break;
                                case 'S':
                                        starsum();
                                        break;
                                case 'T': 
                                        tfsum();
                                        break;
                                case 'C':
                                        print_col();
                                        break;
                                case '?': 
                                        break;
                                case 'R':
                                        ressum();
                                        break;
                                case 'O': 
                                        display_forces(ennum,plnum,&enodds, &plodds, &battle);
                                        break;
                                case 'W': 
                                        withdraw(starnum,plnum);
                                        display_forces(ennum,plnum,&enodds,&plodds,&battle);
                                        break;
                                case ' ': 
                                case 'G': 
                                        fin = true;
                                        break;
                                default: 
                                        printf("!illegal command");
                                }; /*switch (*/
                        } 
                        while (!fin && battle);
                        zero_tf(ENEMY,new_tf);
                        zero_tf(player,plnum);
                        if ( tf[ENEMY][new_tf].dest != 0 ) {
                                point(1,23);
                                printf("en withdraws");
                                point(14,23);
                                disp_tf(&tf[ENEMY][new_tf]);
                                tf[ENEMY][ennum].t = tf[ENEMY][ennum].t - tf[ENEMY][new_tf].t;
                                tf[ENEMY][ennum].s = tf[ENEMY][ennum].s - tf[ENEMY][new_tf].s;
                                tf[ENEMY][ennum].c = tf[ENEMY][ennum].c - tf[ENEMY][new_tf].c;
                                tf[ENEMY][ennum].b = tf[ENEMY][ennum].b - tf[ENEMY][new_tf].b;
                                zero_tf(ENEMY,ennum);
                                display_forces (ennum,plnum,&enodds,&plodds,&battle);
                        };
                };
        };
        zero_tf(ENEMY,ennum);
        zero_tf(player,plnum);
        revolt(starnum);
        on_board(stars[starnum].x,stars[starnum].y);
}

update_board(x, y, option)
toption option;
{
        int scren_x, screen_y;
        if ( terminal_type != hardcopy ) {
                scren_x = 3*x + 1;
                screen_y = 16 - y;
                switch ( option ) {
                case left:
                        point(scren_x,screen_y);
                        putchar(board[x][y].enemy);
                        break;

                case right:
                        point(scren_x+2,screen_y);
                        putchar(board[x][y].tf);
                        break;

                case both:
                        point(scren_x, screen_y);
                        printf("%c%c%c", board[x][y].enemy,board[x][y].star, board[x][y].tf);
                        break;
                }; /*switch (*/
        };
}


up_year()
{
        point(39,18);
        turn = turn + 1;
        if ( terminal_type == hardcopy )
                printf("Year ");
        printf("%3d", turn);
        point(48,19);
        production_year = production_year + 1;
        if ( terminal_type == hardcopy )
                printf("Production year ");
        printf("%d", production_year);
}

withdraw(starnum, plnum)
{
        int withnum;  
        boolean error;
        printf("ithdraw ");
        cle3r_left();
        point(1,19);
        split_tf(&plnum,&withnum);
        if ( tf[player][withnum].dest != 0 ) {
                point(1,20);
                set_des(withnum,&error);
                if ( error ) {
                        tf[player][plnum].dest=starnum;
                        joinsilent(player,&tf[player][plnum],
                                                        &tf[player][withnum]);
                        tf_stars[starnum][player] = 1;
                } 
                else
                        tf[player][withnum].withdrew = true;
        };
}

zero_tf(tm, tf_num)
tteam tm;
{
        int x,y, i;
        if ( tf[tm][tf_num].dest != 0 ) {
                x= tf[tm][tf_num].x;  
                y= tf[tm][tf_num].y;
                if ( (tf[tm][tf_num].s+tf[tm][tf_num].t + tf[tm][tf_num].c +
                    tf[tm][tf_num].b ) ==0 ) {
                        if ( tf[tm][tf_num].eta==0 ) {
                                tf_stars[tf[tm][tf_num].dest][tm]--;
                        };
                        tf[tm][tf_num].dest=0;
                        if(tm==player) {
                                board[x][y].tf=' ';
                                for(i=1; i<=26; i++) {
                                        if((tf[player][i].dest !=0) &&
                                                (tf[player][i].x == x) &&
                                                (tf[player][i].y == y)) {
                                                if(board[x][y].tf == ' ')
                                                        board[x][y].tf = i + 'a' - 1;
                                                else
                                                        board[x][y].tf = '*';
                                        };
                                }
                                update_board(x,y,right);
                        }

                };
        };
}

