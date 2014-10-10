#include <stdio.h>
#include "defs.h"
#include "structs.h"
#include "vars.h"

boolean
any_bc(team, starnum)
tteam team;
int starnum;
{
        boolean any;  
        int tf_number;
        any = false;
        if ( tf_stars[starnum][team]>0 ) {
                tf_number = 1;
                while ( (! any) && (tf_number < 27) ) {
                        any = (tf[team][tf_number].dest==starnum) &&
                            (tf[team][tf_number].eta==0) &&
                            ((tf[team][tf_number].c>0) || 
                                                (tf[team][tf_number].b>0));
                        tf_number = tf_number +1;
                };
        };
        return (any);
}


best_plan(St9rnum, S9ze, T9am)
int *St9rnum, *S9ze;
tteam *T9am;
{
        tplanet *pplanet;
        int starnum, size;
        tteam team;
        team = none;
        starnum = *St9rnum;
        size = 0;
        pplanet = stars[starnum].first_planet;
        while ( pplanet != nil ) {
                if ( pplanet->capacity > size ) {
                        size = pplanet->capacity;
                        team = pplanet->team;
                };
                pplanet=pplanet->next;
        };
        *T9am = team;
        *St9rnum = starnum;
        *S9ze = size;
}

check_game_over()
{
        boolean dead[2];
        boolean quit_game;
        int total[2], transports[2], inhabs[2];
        tteam team; 
        int tfnum, starnum; 
        struct stplanet *pplan;
        quit_game = game_over;
        for (team=ENEMY; team <= player; team++) {
                transports[team] = 0;
                inhabs[team] = 0;
                for ( tfnum = 1 ; tfnum<=26; tfnum++ ) {
                        if ( tf[team][tfnum].dest != 0 )
                                transports[team] = transports[team] 
                                                        + tf[team][tfnum].t;
                };
        };
        for ( starnum = 1 ; starnum<=nstars; starnum++ ) {
                pplan = stars[starnum].first_planet;
                while ( pplan != nil ) {
                        switch ( pplan->team ){
                        case player: 
                                inhabs[player] = inhabs[player] + pplan->iu;
                                break;
                        case ENEMY: 
                                inhabs[ENEMY] = inhabs[ENEMY] + pplan->iu;
                                break;
                        }; /*switch (*/
                        pplan = pplan->next;
                };
        };
        for ( team = ENEMY ; team<=player; team++ ) {
                total[team] = inhabs[team] + transports[team];
                dead[team] = total[team]==0;
        };
        if (  (! dead[player]) && (! dead[ENEMY]) && (turn >= 40) ) {
                dead[ENEMY] = total[player] / total[ENEMY] >= 8;
                dead[player] = total[ENEMY] / total[player] >=8;
        };
        game_over = dead[player] || dead[ENEMY] || (turn>100) ||
            quit_game;
        if ( game_over ) {
                cle2r_screen();
                printf("*** Game over ***\n");
                printf("Player: Population in transports:%3d", 
                                                        transports[player]);
                printf("  IU's on colonies: %3d  TOTAL: %3d\n", inhabs[player],
                                                        total[player]);
                putchar('\n');
                printf("Enemy:  Population in transports:%3d",
                                                        transports[ENEMY]);
                printf("  IU's on colonies: %3d  TOTAL: %3d\n",
                                                inhabs[ENEMY], total[ENEMY]);
                if ( (total[ENEMY] > total[player]) || quit_game  )
                       printf("**** THE ENEMY HAS CONQUERED THE GALAXY ***\n");
                else if ( (total[player] > total[ENEMY]) )
                   printf("*** PLAYER WINS- YOU HAVE SAVED THE GALAXY! ***\n");
                else
                        printf("*** DRAWN ***\n");
        };
}


double fmin();
double exp(), log();

display_forces(ennum, plnum, En0dds, Pl0dds, Battl9)
float *En0dds, *Pl0dds;
boolean *Battl9;
{
        float enodds, plodds;
        boolean battle;
        int en_forces, pl_forces;
        zero_tf(ENEMY,ennum);
        zero_tf(player,plnum);
        battle = true;
        if ( tf[ENEMY][ennum].dest != 0 )
                en_forces = weapons[ENEMY] * ((tf[ENEMY][ennum].c*c_guns)
                                + (tf[ENEMY][ennum].b*b_guns));
        else
                battle = false;
        if ( tf[player][plnum].dest !=0 )
                pl_forces= weapons[player]*((tf[player][plnum].c*c_guns)
                                + (tf[player][plnum].b*b_guns));
        else
                battle = false;
        point(50,1);
        if ( tf[ENEMY][ennum].dest != 0 )
                pr5nt_star(tf[ENEMY][ennum].dest);
        else if ( tf[player][plnum].dest != 0 )
                pr5nt_star(tf[player][plnum].dest);
        clear_field();
        if ( (en_forces==0) && (pl_forces==0)  ) battle=false;
        if ( battle ) {
                enodds = ((float)pl_forces) / (en_forces
                                        + tf[ENEMY][ennum].t*t_def 
                                        + tf[ENEMY][ennum].s*s_def);
                enodds = fmin( 14.0, enodds);
                enodds = exp((log(0.8)) * enodds);
                plodds = ((float)en_forces) / (pl_forces
                                        + tf[player][plnum].t*t_def 
                                        + tf[player][plnum].s*s_def);
                plodds = fmin( 14.0, plodds);
                plodds = exp( (log(0.8)) * plodds);
                point(1,19);
                printf("enemy %5d", en_forces);
                if ( en_forces > 0 )
                        printf("(weap %2d)", weapons[ENEMY]);
                else
                        printf("         ");
                printf("sur: %4.0f", enodds*100.0);
                point(1,20);
                printf("player %5d", pl_forces);
                if ( pl_forces > 0 )
                        printf("(weap %2d)", weapons[player]);
                else
                        printf("         ");
                printf("sur: %4.0f", plodds*100.0);
        };
        *En0dds = enodds; 
        *Pl0dds = plodds; 
        *Battl9 = battle;
}


disp_tf(taskf)
struct sttf *taskf;
{
        if ( taskf->t !=0 ) printf("%2dt", taskf->t);
        else printf("   ");
        if ( taskf->s !=0 ) printf("%2ds", taskf->s);
        else printf("   ");
        if ( taskf->c !=0 ) printf("%2dc", taskf->c);
        else printf("   ");
        if ( taskf->b !=0 ) printf("%2db", taskf->b);
        else printf("   ");
}

EN2MY_attack(starnum)
int starnum;
{
        int attack_factors,def_factors;  
        float odds,best_score;
        struct stplanet *pplanet,*best_planet; 
        int en_tf,i;
        boolean first[8];
        for ( i= 1 ; i<=7; i++ )
                first[i] = true;
        en_tf = 1;
        while ((tf[ENEMY][en_tf].dest!=starnum) || 
                (tf[ENEMY][en_tf].eta != 0 ) ) en_tf = en_tf + 1;
        do {
                attack_factors = tf[ENEMY][en_tf].c + 6
                                                * tf[ENEMY][en_tf].b;
                best_planet = nil;
                best_score = 1000;
                pplanet = stars[starnum].first_planet;
                while (pplanet != nil ) {
                        if ( (pplanet->team == player) ) {
                                def_factors =  pplanet->esee_def;
                                odds = (float)def_factors / attack_factors;
                                if ( pplanet->capacity > 30 )
                                        odds = (odds - 2) * pplanet->capacity;
                                else
                                        odds = (odds - 1.5)* pplanet->capacity;
                                if ( odds < best_score ) {
                                        best_score = odds;
                                        best_planet = pplanet;
                                };
                        };
                        pplanet = pplanet->next;
                };
                if ( best_score < 0 ) {
                        cle3r_left();
                        point(1,19);
                        printf("Enemy attacks: %c%d", starnum+'A'-1, 
                                                        best_planet->number);
                        point(50,1);
                        pr5nt_star(starnum);
                        clear_field();
                        pause();
                        fire_salvo(ENEMY,&tf[ENEMY][en_tf],0,best_planet,
                                        first[best_planet->number]);
                        first[best_planet->number] = false;
                        zero_tf(ENEMY,en_tf);
                        best_planet->esee_def = best_planet->mb + 
                                                        6 * best_planet->amb;
                        pause();
                };
        } 
        while (best_score < 0 && any_bc(ENEMY, starnum));
        revolt(starnum);
}

