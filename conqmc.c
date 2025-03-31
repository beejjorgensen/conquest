#include <stdio.h>
#include "defs.h"
#include "structs.h"
#include "vars.h"
#include "funcs.h"

void depart(int starnum)
{
    if ( (tf_stars[starnum][player]+col_stars[starnum][player])>0 )
        en_departures[starnum]=true;
}

void wander_bc(struct sttf *task, float slist[nstars+1])
{
    int ships,i,count,dest,new_tf;
    if ( (task->b>1) || (task->c > 1) ) {
        count = 0;
        for ( i = 1 ; i<=nstars; i++ ) {
            if ( slist[i] != 0 )
                count = count + 1;
        };
        if ( count > 0 ) {
            dest = rnd(count);
            count = 0;
            i = 0;
            do {
                i = i + 1;
                if ( slist[i]>0 ) count = count + 1;
            } 
            while (count != dest);
            get_tf(ENEMY,&new_tf,task->dest);
            ships = task->b / 2;
            tf[ENEMY][ new_tf].b = ships;
            task->b = task->b - ships;
            ships = task->c / 2;
            tf[ENEMY][new_tf].c = ships;
            task->c = task->c - ships;

            if (task->t > 3) {
                tf[ENEMY][new_tf].t = 2;
                task->t = task->t - 2;
                };

            tf[ENEMY][new_tf].dest = i;
            tf[ENEMY][new_tf].eta = (int)((slist[i]-0.01) / 
                                vel[ENEMY])+1;
            depart(task->dest);
        };
    };
}

int eval_bc_col(struct stplanet *planet)
{
int result;
    if ( ! (stars[planet->pstar].visit[ENEMY]) ) {
        result = 600;
    } 
    else {
        switch ( planet->esee_team ) {

        case none:
            result=100; 
            break;

        case ENEMY:
            if (planet->conquered)
                result = 1000;
            else {

            if ( ((6*planet->amb +planet->mb) <= (planet->iu / 15))
                && (!((! planet->conquered) 
                && (planet->iu < mb_cost)))
                ) {
                result = 300;
            } 
            else {
                result = 0;
            };
            };
            
            if (planet->amb >=4) result = result - 250;
            
            break;

        case player:
            if ( planet->conquered ) {
                result = 400;
            } 
            else {
                result = 200;
            };
            break;
        }; /*switch (*/
        if ( (planet->capacity < 40) && (planet->iu < 15) )
            result = result - 100;
    };
    result = result + rnd(20);
    return (result);
}

int eval_t_col(struct stplanet *planet, float range)
{
    int result;
    if ( (! stars[planet->pstar].visit[ENEMY]) )
        result=60;
    else {
        switch ( planet->esee_team ){
        case none: 
            result = 40 ;
            break;
        case ENEMY: 
            result= 30 ;
            break;
        case player: 
            result = 0;
        }; /*switch (*/
        if ( (planet->esee_team != player) 
            && (planet->capacity - planet->inhabitants > 40 - 
                                (turn / 2) ) )
            result = result + 40;
    };
    result -= (int)(2*range + .5);
    return(result);
}

void inputmach(void)
{
    int count,tfnum,starnum;
    float slist[nstars+1];
    for ( tfnum= 1 ; tfnum<=26; tfnum++ ) {
        if ( (tf[ENEMY][tfnum].eta==0) && (tf[ENEMY][tfnum].dest!=0) ) {
            starnum = tf[ENEMY][tfnum].dest;
            get_stars(starnum,slist,&count);
            send_scouts(slist,&tf[ENEMY][tfnum]);
            send4transports(slist,&tf[ENEMY][tfnum]);
            move_bc(&tf[ENEMY][tfnum],slist);
            zero_tf(ENEMY,tfnum);
        };
    };
}

void move_bc(struct sttf *task, float slist[nstars+1])
{
    int best_star,top_score,starnum,score,factors;
    struct stplanet *pplanet, *best_planet;

    if ( (task->b>0) || (task->c > 0) ) {
        for (starnum = 1; starnum <= nstars; starnum++) {
            if (slist[starnum] > 0 ) {
                best_star = starnum;
                starnum = nstars + 1;
            };
        };
        best_planet = nil;
        top_score = -1000;
        for ( starnum = 1 ; starnum<=nstars; starnum++ ) {
            if ( (slist[starnum] > 0) || (starnum==task->dest) ) {
                pplanet = stars[starnum].first_planet;
                while ( pplanet != nil ) {
                    score = eval_bc_col(pplanet) ;
                    if ( starnum==task->dest )
                        score += 250;

                    if (tf_stars[starnum][ENEMY] > 0)
                        score -= 150;
 
                    if ( score > top_score ) {
                        top_score = score;
                        best_planet = pplanet;
                        best_star = starnum;
                    };
                    pplanet = pplanet->next;
                };
            };
        };
        if ( best_star==task->dest ) { /*stay put*/
            if ( best_planet != nil ) {
                if ( (best_planet->team==ENEMY) 
                    && (best_planet->conquered)
                    && (best_planet->iu < 20)  ) {
                    factors = weapons[ENEMY] * ((task->c*c_guns)+(task->b*b_guns));
                    factors = MIN( factors, 4 * best_planet->inhabitants);
                    blast(best_planet,factors);
                    if ( (tf_stars[best_planet->pstar][player] > 0 ) ||
                        (col_stars[best_planet->pstar][player] > 0 ) )
                        best_planet->psee_capacity = best_planet->capacity;
                } 
                else if ( (best_planet->team==ENEMY) &&
                    (best_planet->conquered) ) { /*decide whether to split*/
                    if ((((task->b > 3) || (task->c > 3) ) && (rnd(4)==4) ) ||
                        (task->b > 8))
                        wander_bc(task,slist);
                };
            };
        } 
        else { /*move*/
            tf_stars[task->dest][ENEMY]--;
            depart(task->dest);
            task->dest = best_star;
            task->eta = (int)((slist[best_star]-0.01) /
                                vel[ENEMY])+1;
        };
    };
}
 
boolean underdefended(int starnum)
{
    struct stplanet *pplanet;
    boolean result;
    result = false;
    pplanet = stars[starnum].first_planet;
    while ( (pplanet != nil) && (! result) ) {
        if ( (pplanet->team==ENEMY) && (pplanet->iu > 10) &&
            ((6*pplanet->amb +pplanet->mb) < conq_round(pplanet->iu / 15)) )
            result = true;
        pplanet = pplanet->next;
    };
    return(result);
}


void send2t_tf(struct sttf *task, float slist[nstars+1], int dest_star)
{
    depart(task->dest);
    task->dest = dest_star;
    task->eta = (int)((slist[dest_star]-0.01)/vel[ENEMY])+1;
}


void send4transports(float slist[nstars+1], struct sttf *task)
{
    int new_tf,to_land,sec_star,sec_score,best_star,top_score,
        score,starnum;
    int xstar;
    struct stplanet *pplan, *best_plan;
    int trash1, trash2;

    if ( task->t >0 ) {
        best_star=0;
        sec_star = 0;
        sec_score = -11000;
        top_score = -10000;
        best_plan=nil;
        for ( starnum = 1 ; starnum<=nstars; starnum++ ) {
            if ( (slist[starnum] > 0) || (starnum==task->dest) ) {
                pplan = stars[starnum].first_planet;
                while ( pplan != nil ) {
                    score = eval_t_col(pplan,slist[starnum]);
                    xstar = starnum;
                    if ( score > top_score ) {
                        swap(&best_star,&xstar);
                        swap(&top_score,&score);
                        best_plan = pplan;
                    };
                    if ( score > sec_score ) {
                        sec_score = score;
                        sec_star = xstar;
                    };
                    pplan = pplan->next;
                };
            };
        };
        if ( best_star == task->dest ) { /*land*/
            if ( (tf_stars[best_star][player]==0) && (best_plan->team != player) 
                ) {
                trash1 = task->t;
                trash2 = (best_plan->capacity-best_plan->inhabitants)/3;
                to_land = MIN(trash1, trash2);
                if ( (to_land > 0) ) {
                    if ( best_plan->inhabitants==0 ) {
                        best_plan->team = ENEMY;
                        best_plan->esee_team = ENEMY;
                        col_stars[best_star][ENEMY]=col_stars[best_star][ENEMY]+1;
                    };
                    best_plan->inhabitants=best_plan->inhabitants+to_land;
                    best_plan->iu = best_plan->iu + to_land;
                    task->t = task->t - to_land;
                    send4transports(slist,task);
                };
            };
        } 
        else { /*move*/
            if ( (task->t >= 10) && (sec_star > 0) ) {
                get_tf(ENEMY,&new_tf,task->dest);
                tf[ENEMY][new_tf].t = task->t / 2;
                task->t = task->t - tf[ENEMY][new_tf].t;
                if ( (task->c >0) && (! underdefended(task->dest)) ) {
                    tf[ENEMY][new_tf].c = 1;
                    task->c = task->c -1;
                };
                send2t_tf(&tf[ENEMY][new_tf],slist,best_star);
                best_star = sec_star;
            };
            get_tf(ENEMY,&new_tf,task->dest);
            tf[ENEMY][new_tf].t = task->t;
            task->t = 0;
            if ( (task->c >0) && (! underdefended(task->dest)) ) {
                tf[ENEMY][new_tf].c = 1;
                task->c = task->c -1;
            };
            send2t_tf(&tf[ENEMY][new_tf],slist,best_star);
        };
    };
}


void send_scouts(float slist[nstars+1], struct sttf *task)
{
    int dest,new_tf,j,doind;
    int doable[nstars+1];
    if ( task->s > 0 ) {
        doind=0;
        for ( j = 1 ; j<=nstars; j++ ) {
            if ( (! stars[j].visit[ENEMY]) && (slist[j]>0) ) {
                doind=doind+1;
                doable[doind]=j;
            };
        };
        while ( (doind>0) && (task->s > 0) ) {
            get_tf(ENEMY,&new_tf,task->dest);
            tf[ENEMY][new_tf].s = 1;
            dest = rnd(doind);
            tf[ENEMY][new_tf].dest = doable[dest];
            tf[ENEMY][new_tf].eta = (int)((slist[doable[dest]]-0.01)/vel[ENEMY])
                +1;
            depart(task->dest);
            doable[dest] = doable[doind];
            doind = doind-1;
            task->s = task->s -1;
        };
        while ( task->s > 0 ) {
            do {
                dest= rnd(nstars);
            } 
            while (slist[dest] <= 0);
            get_tf(ENEMY,&new_tf,task->dest);
            tf[ENEMY][new_tf].s=1;
            tf[ENEMY][new_tf].dest = dest;
            tf[ENEMY][new_tf].eta = (int)((slist[dest]-0.01)/vel[ENEMY])+1;
            depart(task->dest);
            task->s = task->s -1;
        };
    };
}

