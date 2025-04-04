#include <stdio.h>
#include <curses.h>
#include <math.h>
#include "defs.h"
#include "structs.h"
#include "vars.h"
#include "funcs.h"

#define new(x) x=(tplanet *)alloc(sizeof(tplanet))

void blast(tplanet *planet, int factors)
{
    int killed;

    killed = MIN(planet->capacity, factors / 4);
    planet->inhabitants =
        MIN(planet->inhabitants, planet->capacity) - killed;
    planet->iu = MIN(planet->iu - killed, planet->inhabitants * iu_ratio);
    planet->capacity = planet->capacity - killed;
    if (planet->inhabitants <= 0) {
        planet->inhabitants = 0;
        planet->iu = 0;
        planet->mb = 0;
        planet->amb = 0;
        if (planet->team != none) {
            col_stars[planet->pstar][planet->team]--;
            planet->team = none;
            planet->esee_team = none;
            planet->conquered = false;
            on_board(stars[planet->pstar].x, stars[planet->pstar].y);
        };
    };
}



void fire_salvo(tteam att_team, struct sttf *task, int tfnum,
                struct stplanet *planet, boolean first_time)
{
    int bases, att_forces, def_forces;
    boolean a_lose_none, p_lose_none;
    float att_odds, def_odds, attack_save, defend_save;
    tteam def_team;
    if (left_line[24]) {
        point(1, 24);
        printw(blank_line);
        left_line[24] = false;
    };
    if (att_team == ENEMY)
        def_team = player;
    else
        def_team = ENEMY;
    att_forces = weapons[att_team] * (task->c * c_guns + task->b * b_guns);
    def_forces =
        weapons[def_team] * (planet->mb * c_guns + planet->amb * b_guns);
    if (def_forces > 0) {
        att_odds = fmin((((float) def_forces) / att_forces), 14.0);
        attack_save = exp(log(0.8) * (att_odds));
        def_odds = fmin((float) att_forces / def_forces, 14.0);
        defend_save = exp(log(0.8) * (def_odds));
        point(1, 20);
        if (att_team == player)
            printw("TF%c", tfnum + 'a' - 1);
        else
            printw(" EN");
        printw(": %4d(weap %2d)sur: %4.0f", att_forces,
               weapons[att_team], attack_save * 100);
        point(1, 21);
        printw(" %c%d:%4d (weap %2d)sur: %4.0f", planet->pstar + 'A' - 1,
               planet->number,
               def_forces, weapons[def_team], defend_save * 100);
        point(1, 22);
        printw("Attacker losses:              ");
        point(1, 23);
        left_line[23] = true;
        printw(" Planet losses :              ");
        a_lose_none = true;
        p_lose_none = true;
        do {
            point(17, 22);
            lose(&task->c, &a_lose_none, 'c', attack_save);
            lose(&task->b, &a_lose_none, 'b', attack_save);
            point(17, 23);
            bases = planet->mb;
            lose(&planet->mb, &p_lose_none, 'm', defend_save);
            if (planet->mb != bases)
                printw("b");
            bases = planet->amb;
            lose(&planet->amb, &p_lose_none, 'a', defend_save);
            if (planet->amb != bases)
                printw("mb");
        }
        while (!first_time && p_lose_none && a_lose_none);
        if (a_lose_none) {
            point(17, 22);
            printw("(none)");
        };
        if (p_lose_none) {
            point(17, 23);
            printw("(none)");
        };
    };
    if ((planet->mb + planet->amb == 0)
        && (any_bc(att_team, planet->pstar))) {
        point(1, 24);
        printw("Planet %d falls!               ", planet->number);
        planet->team = att_team;
        planet->esee_team = att_team;
        planet->conquered = true;
        col_stars[task->dest][def_team]--;
        col_stars[task->dest][att_team]++;
        point(50, 1);
        pr5nt_star(planet->pstar);
        clear_field();
        on_board(stars[task->dest].x, stars[task->dest].y);
    };
}

void play_salvo(int starnum, boolean *Battl9)
{
    boolean battle;
    char tf_char, planch;
    int planet_num = 0, tf_num;
    boolean found;
    struct stplanet *pplanet;
    boolean first_time;
    battle = *Battl9;
    printw("Attack planet ");
    pplanet = stars[starnum].first_planet;
    if ((col_stars[starnum][ENEMY] > 1)) {
        printw(":");
        get_char(&planch);
        cle3r_left();
        planet_num = planch - '0';
        found = false;
        while (!found) {
            if (pplanet->number == planet_num)
                found = true;
            else {
                if (pplanet->next == nil)
                    found = true;
                else
                    pplanet = pplanet->next;
            }
        }
        if (pplanet->number != planet_num) {
            planet_num = 0;
            error_message();
            printw("! That is not a useable planet");
        } else if (pplanet->team != ENEMY) {
            error_message();
            printw(" !Not an enemy colony");
            planet_num = 0;
        };
    } else {
        while (pplanet->team != ENEMY)
            pplanet = pplanet->next;
        printw("%d", pplanet->number);
        cle3r_left();
    };
    if (planet_num != 0) {
        point(1, 19);
        printw(" attacking tf ");
        if (tf_stars[starnum][player] > 1) {
            printw(":");
            get_char(&tf_char);
            tf_num = tf_char - 'A' + 1;
        } else {
            tf_num = 1;
            while (tf[player][tf_num].dest != starnum ||
                   tf[player][tf_num].eta != 0)
                tf_num++;
            addch(tf_num + 'a' - 1);
        }
        if (tf_num < 1 || tf_num > 26) {
            error_message();
            printw(" !Illegal tf");
        } else if (tf[player][tf_num].dest == 0) {
            error_message();
            printw(" !Nonexistent tf");
        } else if ((tf[player][tf_num].dest != starnum) ||
                   (tf[player][tf_num].eta != 0)) {
            error_message();
            printw(" !Tf is not at this star      ");
        } else if ((tf[player][tf_num].b + tf[player][tf_num].c) == 0) {
            error_message();
            printw(" !Tf has no warships");
        } else {
            first_time = !pplanet->under_attack;
            if (!(pplanet->under_attack)) {
                pplanet->under_attack = true;
                point(50, 1);
                pr5nt_star(starnum);
                clear_field();
            };
            fire_salvo(player, &tf[player][tf_num], tf_num,
                       pplanet, first_time);
            zero_tf(player, tf_num);
            battle = (col_stars[starnum][ENEMY] > 0) &&
                (any_bc(player, starnum));
        };
    };
    *Battl9 = battle;
}
