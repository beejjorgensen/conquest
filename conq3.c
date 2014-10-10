#include "defs.h"
#include "structs.h"
#include "vars.h"
#include <stdio.h>


new_research()
{
        if ( weapons[player] - weapons[ENEMY] > 1 )
                en_research = 'W';
        else {
                switch ( rnd(10) ){

                case 1: 
                case 2: 
                case 3: 
                        en_research = 'V';
                        break;
                default:
                        en_research = 'W';
                        break;
                case 10: 
                        en_research = 'R';

                }; /*switch (*/
        };
}


pl2yerattack(starnum)
int starnum;
{
        boolean battle;
        char command;
        struct stplanet *pplanet;
        battle = any_bc(player,starnum);
        if ( (battle) ) {
                point(33,20);
                printf("Attack at star %c", starnum+'A'-1);
                while (battle ) {
                        point(50,1);
                        pr5nt_star(starnum);
                        clear_field();
                        point(1,18);
                        printf("P?                            ");
                        point(3,18);
                        get_char(&command);
                        switch ( command ) {
                        case 'S':
                                starsum();
                                break;
                        case 'M' : 
                                printmap();
                                break;
                        case 'H': 
                                help(3);
                                pause();
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
                        case 'T': 
                                tfsum();
                                break;
                        case 'G': 
                        case ' ': 
                                play_salvo(starnum,&battle);
                                break;
                        case 'B': 
                                printf("reak off attack");
                                battle = false;
                                break;
                        default:
                                cle3r_left();
                                error_message();
                                printf(" !Illegal command");
                                break;
                        }; /*switch (*/
                };
                pplanet = stars[starnum].first_planet;
                while (pplanet != nil ) {
                        pplanet->under_attack = false;
                        pplanet = pplanet->next;
                };
                point(1,24);
                printf("Planet attack concluded       ");
                revolt(starnum);
        };

}

