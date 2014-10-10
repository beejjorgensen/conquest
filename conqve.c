#include <stdio.h>
#include "defs.h"
#include "structs.h"
#include "vars.h"

inv_enemy(x,y, planet)
struct stplanet *planet;
{
        int num,inv_amount,balance,min_mb,transports,new_tf;
        int trash1, trash2;

        balance = planet->iu;
        if ( tf_stars[planet->pstar][ENEMY]==0 ) {
                get_tf(ENEMY,&new_tf,planet->pstar);
                tf_stars[planet->pstar][ENEMY]=1;
        } 
        else { /*use present tf*/
                new_tf = 1;
                while ( (tf[ENEMY][new_tf].dest != planet->pstar) || (tf[
                    ENEMY][new_tf].eta != 0) ) new_tf = new_tf + 1;
        };
        min_mb = planet->capacity / 20;
        while ( ((planet->amb==0) && (! planet->conquered) &&
            (planet->mb < min_mb) && (balance >= mb_cost))) {
                balance = balance - mb_cost;
                planet->mb = planet->mb+1;
        };
        if ( ( balance >= b_cost) && (rnd(5) != 1) && (rnd(07) <= planet->amb+3)
            &&
            (planet->amb > 1) ) {
                balance = balance - b_cost;
                tf[ENEMY][new_tf].b ++;
        };
        if ( (balance >= amb_cost) && ((planet->amb< 4) || (rnd(2)==2))
            && (! planet->conquered) ) {
                balance = balance - amb_cost;
                planet->amb++;
        };
        while ( balance >= 9 ) {
                switch ( rnd(12) ){
                case 1: 
                case 2: 
                        research(ENEMY,en_research,8);
                        balance = balance-8;
                        break;

                default:
                        inv_amount = min(3,planet->inhabitants*iu_ratio - planet->iu);
                        balance = balance - i_cost * inv_amount;
                        planet->iu = planet->iu + inv_amount;
                        break;

                case 3: 
                case 4:
                case 10:
                        if ( balance >= c_cost ) {
                                balance = balance - c_cost;
                                tf[ENEMY][new_tf].c++;
                        } 
                        else if ( (! planet->conquered) && (balance >= mb_cost)
                            ) {
                                balance = balance - mb_cost;
                                planet->mb = planet->mb + 1;
                        } 
                        else {
                                balance = balance - 9;
                                research(ENEMY,en_research,9);
                        };
                        break;

                case 11: 
                case 12:
                        if ( ((float)planet->inhabitants/planet->capacity < 0.6) ||
                            ((planet->capacity >= b_cost / iu_ratio) && (planet->iu <
                            b_cost+10)) ) { /*no t"s*/
                                inv_amount = min(3,planet->inhabitants * iu_ratio - planet->iu);
                                balance = balance - inv_amount * i_cost;
                                planet->iu = planet->iu + inv_amount;
                        } 
                        else /*build transports*/
                                if(!(planet->conquered)) {
                                        transports = min(rnd(2)+6,planet->inhabitants-1);
                                        if ( planet->iu > b_cost )
                                                transports = min(transports,planet->iu - b_cost);
                                        balance = balance - transports;
                                        planet->inhabitants = planet->inhabitants - transports;
                                        trash1 = planet->iu - transports;
                                        trash2 = planet->inhabitants * iu_ratio;
                                        planet->iu = min(trash1, trash2);
                                        tf[ENEMY][new_tf].t = tf[ENEMY][new_tf].t + transports;
                                };
                        break;
                        /*
                             balance = balance - s_cost;
                             tf[ENEMY][new_tf].s = tf[ENEMY][new_tf].s + 1;
                             if ( debug ) write(tty, "s ");
                         */
                }; /*switch (*/
        }; /*while (*/
        zero_tf(ENEMY,new_tf);
        research(ENEMY,en_research,balance);
}

inv_player(x,y,planet)
struct stplanet *planet;
{
        boolean printtf; 
        char iline[81]; 
        char key;
        int cost,amount,ind,new_tf,balance;
        int trash1, trash2;

        get_tf(player,&new_tf,planet->pstar);
        tf_stars[planet->pstar][player]++;
        printtf = false;
        balance=planet->iu;
        cle3r_left();
        point(1,19);
        putchar(planet->pstar+'A'-1);

        printf("%d:%2d                         ", planet->number, planet->psee_capacity);
        point(x_cursor + 5, y_cursor);
        x_cursor = x_cursor - 5;
        printf("(%2d,/%3d)", planet->inhabitants, planet->iu);
        if ( planet->conquered )
                printf("Con");
        else
                printf("   ");
        if ( planet->mb!=0 )
                printf("%2dmb", planet->mb);
        else
                printf("    ");
        if ( planet->amb!=0 )
                printf("%2damb", planet->amb);
        point(x_cursor,y_cursor + 1);

        do {
                point(1,18);
                printf("%3d?                          ", balance);
                point(5,18);
                get_line(iline,&ind,false);
                do {
                        get_token(iline,&ind,&amount,&key);
                        switch ( key ){
                        case 'A': 
                                cost=amount*amb_cost;
                                if ( planet->inhabitants ==0 ) {
                                        cost = 0;
                                        error_message();
                                        printf("  !abandoned planet");
                                } 
                                else if (planet->conquered) {
                                        cost =0;
                                        error_message();
                                        printf(" !No amb  on conquered colony ");
                                } 
                                else
                                        if ( cost<=balance ) {
                                                planet->amb=planet->amb+amount;
                                        };
                                break;
                        case 'B': 
                                cost=amount * b_cost;
                                if ( cost<=balance ) {
                                      tf[player][new_tf].b=tf[player][new_tf].b
                                                                + amount;
                                        printtf = true;
                                };
                                break;
                        case 'C': 
                                cost=amount * c_cost;
                                if ( cost <= balance ) {
                                     tf[player][new_tf].c=tf[player][new_tf].c
                                                                + amount;
                                        printtf = true;
                                };
                                break;
                        case 'H': 
                                help(4);
                                cost = 0;
                                break;
                        case 'M': 
                                cost=amount * mb_cost;
                                if ( planet->inhabitants ==0 ) {
                                        cost =0;
                                        error_message();
                                        printf("  !abandoned planet");
                                } 
                                else if (planet->conquered) {
                                        cost =0;
                                        error_message();
                                        printf(" !No Mb  on conquered colony  ");
                                } 
                                else
                                        if ( cost <= balance ) {
                                                planet->mb=planet->mb+amount;
                                        };
                                break;
                        case 'S': 
                                cost= amount * s_cost;
                                if ( cost <= balance ) {
                                     tf[player][new_tf].s=tf[player][new_tf].s
                                                                + amount;
                                        printtf = true;
                                };
                                break;
                        case 'T': 
                                cost= amount;
                                if ( cost <= balance ) {
                                        if ( cost > planet->inhabitants ) {
                                                error_message();
                                                printf(" ! Not enough people for ( trans");
                                                cost=0;
                                        } 
                                        else if ( planet->conquered ) {
                                                cost = 0;
                                                error_message();
                                                printf( "!No transports on conqered col");
                                        } 
                                        else {
                                                tf[player][new_tf].t=tf[player][new_tf].t+amount;
                                                planet->inhabitants=planet->inhabitants-amount;
                                                trash1 = planet->iu - amount;
                                                trash2 = planet->inhabitants * iu_ratio;
                                                planet->iu = min(trash1, trash2);
                                                printtf = true;
                                                if ( planet->inhabitants==0 ) {
                                                        col_stars[planet->pstar][player]--;
                                                        if ( col_stars[planet->pstar][player]==0 ) {
                                                                board[x][y].enemy = ' ';
                                                                update_board(x,y,left);
                                                        };
                                                        planet->team=none;
                                                        planet->amb=0; 
                                                        planet->mb=0; 
                                                        planet->iu=0;
                                                };
                                        };
                                };
                                break;
                        case 'I': 
                                cost= i_cost * amount;
                                if ( (amount + planet->iu) > (planet->inhabitants*iu_ratio)
                                    ) {
                                        cost = 0;
                                        error_message();
                                        printf(" !Can't support that many iu's");
                                } 
                                else if ( cost <= balance ) {
                                        planet->iu=planet->iu+amount;
                                };
                                break;
                        case 'R': 
                        case 'V': 
                        case 'W':
                                cost=amount;
                                if ( cost <= balance ) {
                                        point(1,21);
                                        research(player,key,amount);
                                };
                                pr2nt_res(key);
                                break;
                        case ' ': 
                                cost=0; 
                                break;
                        case '>': 
                                cost=0;
                                point(1,18);
                                printf(">?     ");
                                point(3,18);
                                get_char(&key);
                                switch ( key ){
                                case 'M':
                                        printmap(); 
                                        break;
                                case 'S':
                                        starsum(); 
                                        break;
                                case 'C':
                                        print_col(); 
                                        break;
                                case 'R': 
                                        ressum(); 
                                        break;
                                default:
                                        error_message();
                                        printf(" !Only M,S,C,R allowed      ");
                                }; /*switch ( > */
                                break;
                        default:
                                error_message();
                                printf(" !Illegal field %c",key);
                        }; /*switch (*/
                        if ( cost > balance ) {
                                error_message();
                                printf(" !can't affort %3d%c", amount, key);
                        }
                        else
                                balance = balance - cost;
                } 
                while (key != ' ');
                cle3r_left();
                point(1,19);
                putchar(planet->pstar+'A'-1);

        printf("%d:%2d                         ", planet->number, planet->psee_capacity);
        point(x_cursor + 5, y_cursor);
        x_cursor = x_cursor - 5;
        printf("(%2d,/%3d)", planet->inhabitants, planet->iu);
        if ( planet->conquered )
                printf("Con");
        else
                printf("   ");
        if ( planet->mb!=0 )
                printf("%2dmb", planet->mb);
        else
                printf("    ");
        if ( planet->amb!=0 )
                printf("%2damb", planet->amb);
        point(x_cursor,y_cursor + 1);

                if ( printtf ) {
                        point(1,20);
                        pr3nt_tf(new_tf);
                };
        } 
        while (balance > 0);
        zero_tf(player,new_tf);
        on_board(x,y);
}

invest()
{
        int newborn,starnum; 
        struct stplanet  *pplan;
        production_year = 0;
        point(33,20);
        printf("* investment *  ");
        for ( starnum= 1 ; starnum<=nstars; starnum++ ) {
                pplan= stars[starnum].first_planet;
                while ( pplan!=nil ) {
                        if ( ((pplan->esee_team) == player) 
                            && ((pplan->capacity) > 10)
                            && ((pplan->esee_def) < 12) )
                                pplan->esee_def = (pplan->esee_def) + 1;
                        if ( (pplan->team) != none ) {
                                newborn= round((pplan->inhabitants)
                                    * growth_rate[pplan->team] *
                                    (1-((pplan->inhabitants)/(pplan->capacity))));
                                if ( pplan->conquered )
                                        newborn = newborn / 2;
                                pplan->inhabitants= (pplan->inhabitants) 
                                                                + newborn;
                                pplan->iu = (  pplan->iu) + newborn;
                                if ( pplan->team==ENEMY )
                                        inv_enemy(stars[starnum].x,
                                                stars[starnum].y,pplan);
                                else
                                        inv_player(stars[starnum].x,
                                                stars[starnum].y,pplan);
                        };
                        pplan=pplan->next;
                };
        };
        battle();
}

