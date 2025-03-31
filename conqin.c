#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <time.h>
#include <math.h>
#include "defs.h"
#include "structs.h"
#include "vars.h"
#include "funcs.h"

#define new(x) x=(tplanet *)malloc(sizeof(tplanet))

int conv_bcd(int nibble, char byte)
{
 if (nibble == 1) return( byte & 0x0f);
 return( ( byte >> 4) & 0x0f);
}

void assign_planets(tstar *Ustar0, int starnum)
{
        int i1, nplanets;
        tplanet *pplanet;
        tstar ustar;

        ustar = *Ustar0;
        nplanets=rnd(4)-2;
        if (nplanets < 0) nplanets = 1;
        if ( nplanets==0 )
                ustar.first_planet = nil;
        else {
                new(pplanet);
                ustar.first_planet=pplanet;
                for ( i1=1 ; i1<=nplanets; i1++ ) {
                        pplanet->number= rnd(2) + (2*i1) - 2;
                        if ( rnd(4)>2 ) pplanet->capacity = 10 * (rnd(4) + 2);
                             else pplanet->capacity = 5 * rnd(3);
                        pplanet->psee_capacity = pplanet->capacity;
                        pplanet->team=none;
                        pplanet->inhabitants=0;
                        pplanet->iu=0;
                        pplanet->mb=0;
                        pplanet->amb=0;
                        pplanet->conquered = false;
                        pplanet->under_attack = false;
                        pplanet->esee_team = none;
                        pplanet->esee_def = 1;
                        pplanet->pstar=starnum;
                        if ( i1 == nplanets )
                                pplanet->next=nil;
                        else {
                                new(pplanet->next);
                                pplanet=pplanet->next;
                        }
                }
        };
        *Ustar0 = ustar;
}


void init_player(void)
{
        char str, key; 
        int star_number;
        int balance,cost,amt,ind; 
        char iline[81];
        do {
                point(1,18);
                printw("start at star?\n     ");
                get_char(&str);
                point(1,19);
                star_number= str-'A'+1;
        } 
        while (star_number < 1 || star_number > nstars);
        tf[player][1].x=stars[star_number].x;
        tf[player][1].y=stars[star_number].y;
        tf_stars[star_number][player]=1;
        tf[player][1].dest = star_number;
        point(1,20);
        printw("choose your initial fleet.");
        point(1,21);
        printw("you have %d transports", initunit);
        point(1,22);
        printw(" && %d units to spend", initmoney);
        point(1,23);
        printw("on ships or research.");
        balance= initmoney;
        do {
                point(1,19);
                pr3nt_tf(1);
                point(1,18);
                printw("%3d?                          ", balance);
                point(6,18);
                get_line(iline,&ind,false);
                do {
                        get_token(iline,&ind,&amt,&key);
                        switch ( key ) {
                        case 'C':
                                cost= amt*c_cost;
                                if ( cost <= balance ) {
                                        tf[player][1].c=tf[player][1].c+amt;
                                }
                                break;
                        case 'S':
                                cost= amt*s_cost;
                                if ( cost <= balance ) {
                                        tf[player][1].s=tf[player][1].s+amt;
                                };
                                break;
                        case 'B':
                                cost= amt*b_cost;
                                if ( cost <= balance ) {
                                        tf[player][1].b=tf[player][1].b+amt;
                                };
                                break;
                        case 'H': 
                                help(0); 
                                cost = 0;
                                break;
                        case 'W': 
                        case 'V': 
                        case 'R':
                                cost= amt;
                                if ( cost <= balance )
                                        research(player,key,amt);
                                break;
                        case ' ': 
                                cost=0; 
                                break;
                        case '>':
                                point(1,18);
                                printw(">?      ");
                                point(3,18);
                                cost=0;
                                get_char(&key);
                                switch ( key ) {
                                case 'M': 
                                        printmap(); 
                                        break;
                                case 'R': 
                                        ressum(); 
                                        break;
                                default:
                                        error_message();
                                        printw(" !Only M,R during initialize");
                                }; /*!= switch (*/
                                break;
                        default:
                                error_message();
                                printw( " !Illegal field %c",key);
                        }; /*switch (*/
                        if ( cost <= balance )
                                balance = balance - cost;
                        else {
                                error_message();
                                printw("  !can't afford %c",key);
                        };
                } 
                while (key != ' ');
        } 
        while (balance >0);
        stars[star_number].visit[player]=true;
        board[stars[star_number].x][stars[star_number].y].tf = 'a';
        board[stars[star_number].x][stars[star_number].y].enemy=' ';
        on_board(stars[star_number].x,stars[star_number].y);
        point(33,20);
}

void initconst(void)
{
        int i3, i1,i2,x,y,temp;
        tteam team; 
        char tt;
        time_t date;

		cbreak();
		noecho();

        printw("\n* Welcome to CONQUEST! *\n\n");
        printw("Amiga version 1.0\n");
        printw("Hit return to continue\n");
        get_char(&tt);

        printw("\33<");

		date = time(NULL);
        srand48(date);

        saved_game = false;

        /*init the board*/
        for ( i1=1 ; i1<=bdsize; i1++ ) {
                for ( i2=1 ; i2<=bdsize; i2++ ) {
                        board[i1][i2].enemy=' ';
                        board[i1][i2].tf= ' ';
                        board[i1][i2].star='.';
                }
        };
        /*init stars*/
        for ( i1= 1 ; i1<=nstars; i1++ ) {
                enemy_arrivals[i1]=false;
                en_departures[i1]=false;
                for ( i2=1; i2<=i1;i2++) i3=rnd(64);
                player_arrivals[i1]=false;
                do {
                        x=rnd(bdsize);  
                        y=rnd(bdsize);
                } 
                while (board[x][y].star != '.');
                stars[i1].x=x;     
                stars[i1].y=y;
                for(i2=1; i2<=i1; i2++) {
                        if(i1==i2) r2nge[i1][i2] = 0;
                        else {
                                temp =(((x-stars[i2].x)*(x-stars[i2].x))+
                                        ((y-stars[i2].y)*(y-stars[i2].y)));
                                r2nge[i1][i2] = 23;
                                for(i3=1; i3<22; i3++)
                                        if(temp < i3*i3) {
                                                r2nge[i1][i2] = i3;
                                                break;
                                                }
                                r2nge[i2][i1]=r2nge[i1][i2];
                        }
                }
                for ( team = ENEMY ; team<=player; team++ ) {
                        tf_stars[i1][team]=0;
                        col_stars[i1][team] = 0;
                };
                board[x][y].star = 'A'+i1-1;
                board[x][y].enemy = '?';
                stars[i1].visit[player]=false; 
                stars[i1].visit[ENEMY]=false;
                assign_planets(&stars[i1],i1);
        };
        /*initialize research costs*/
        ran_req[5]=0; 
        ran_req[6]=20; 
        ran_req[7]=40; 
        ran_req[8]=70;
        ran_req[9]=100; 
        ran_req[10]=150;
        ran_req[11]= 200;  
        ran_req[12]=300; 
        ran_req[13]=400;
        ran_req[14]=600; 
        ran_req[15] = 900;
        vel_req[2]=40; 
        vel_req[3]=60; 
        vel_req[4]=80; 
        vel_req[5]= 120;
        vel_req[6]=150; 
        vel_req[7]=200; 
        vel_req[8]=250; 
        vel_req[9]=300;
        vel_req[10]=400; 
        vel_req[11]=500; 
        vel_req[12]=600;
        weap_req[3]=0; 
        weap_req[4]=50; 
        weap_req[5]=70;
        weap_req[6]=90; 
        weap_req[7]=120; 
        weap_req[8]=150;
        weap_req[9]=250; 
        weap_req[10]=350;
        /*initialize task forces*/
        tf[ENEMY][1].x = 1; 
        tf[ENEMY][1].y = 1;
        for ( team = ENEMY ; team<=player; team++ ) {
                for ( i1= 1 ; i1<=26; i1++ ) {
                        tf[team][i1].dest = 0;
                        tf[team][i1].blasting = false;
                        tf[team][i1].withdrew = false;
                        tf[team][i1].s = 0;
                        tf[team][i1].t = 0;
                        tf[team][i1].c = 0;
                        tf[team][i1].b = 0;
                        tf[team][i1].dest = 0;
                        tf[team][i1].eta = 0;
                };
                tf[team][1].t = initunit;
                vel[team] = initvel;
                range[team] = initrange;
                weapons[team] = initweap;
                weap_working[team] = 0;
                vel_working[team] = 0;
                ran_working[team] = 0;
        };
        range[ENEMY] = initrange + 2;
        growth_rate[player]=0.3;
        growth_rate[ENEMY]= 0.5;

        switch (rnd(3)) {
                case 1: weapons[ENEMY] = rnd(4) + 2;
                        break;

                case 2: vel[ENEMY] = rnd(3);
                        break;

                case 3: growth_rate[ENEMY] = (float)(rnd(4) + 3) / 10.0;
                        break;
        };

        game_over=false;
        turn=1;
        production_year = 1;
        printmap();
        point(33,20);
        printw("*Initialization*");
        init_player();
}

void initmach(void)
{
        int res_amt, maxx, start_star, starnum, count;
        float slist[nstars+1];

        en_research = 'V';
        tf[ENEMY][1].c = 1;
        tf[ENEMY][1].s = 2;
        res_amt = 2;
        research(ENEMY, en_research, res_amt);
        maxx = 0;
        start_star = 0;
        for (starnum = 1; starnum <= nstars; starnum++) {
                get_stars(starnum, slist, &count);
                count += rnd(5);
                if (count > maxx) {
                        maxx = count;
                        start_star = starnum;
                }
        }
        tf[ENEMY][1].dest = start_star;
        tf[ENEMY][1].x = stars[start_star].x;
        tf[ENEMY][1].y = stars[start_star].y;
        stars[start_star].visit[ENEMY] = true;
        tf_stars[start_star][ENEMY] = 1;
        point(50,1);
        pr5nt_star(tf[player][1].dest);
        clear_field();
        if (start_star == tf[player][1].dest) {
                cle3r_left();
                battle();
        }
}

