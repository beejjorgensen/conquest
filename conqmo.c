#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include "defs.h"
#include "structs.h"
#include "vars.h"
#include "funcs.h"

void move_ships(void)
{
    float ratio, prob;
    int there, dx, dy, i;
    tteam tm;
    struct stplanet *pplanet;
    boolean any, loss;
    /*clear the board */
    for (i = 1; i <= 26; i++) {
        if ((tf[player][i].dest != 0) && (tf[player][i].eta != 0)) {
            board[tf[player][i].x][tf[player][i].y].tf = ' ';
            update_board(tf[player][i].x, tf[player][i].y, right);
        };
    };
    /*move ships of both teams */
    tm = ENEMY;
    do {
        for (i = 1; i <= 26; i++) {
            if ((tf[tm][i].dest != 0) && (tf[tm][i].eta != 0)) {
                tf[tm][i].eta = tf[tm][i].eta - 1;
                if ((!stars[tf[tm][i].dest].visit[tm]) &&
                    (tf[tm][i].eta == 0) && (tm == player)) {

                    left_line[20] = true;
                    cle3r_left();
                    point(1, 19);
                    printw("Task force %c exploring %c.\n", '`' + i,
                           tf[tm][i].dest + '@');
                    prob = (t_e_prob + rnd(t_e_var) * tf[tm][i].t) / 100.0;
                    if (tf[tm][i].s != 0)
                        prob =
                            (s_e_prob +
                             rnd(s_e_var) * tf[tm][i].s) / 100.0;
                    if (tf[tm][i].c != 0)
                        prob =
                            (c_e_prob +
                             rnd(c_e_var) * tf[tm][i].c) / 100.0;
                    if (tf[tm][i].b != 0)
                        prob =
                            (b_e_prob +
                             rnd(b_e_var) * tf[tm][i].b) / 100.0;
                    if (prob > 100)
                        prob = 100;
                    loss = true;
                    lose(&tf[tm][i].t, &loss, 't', prob);
                    lose(&tf[tm][i].s, &loss, 's', prob);
                    lose(&tf[tm][i].c, &loss, 'c', prob);
                    lose(&tf[tm][i].b, &loss, 'b', prob);
                    if (loss)
                        printw("No ships");
                    printw(" destroyed.");
                    left_line[23] = true;
                    pause();
                    tf[tm][i].eta = 1;  /* fool zero tf */
                    zero_tf(tm, i);
                    tf[tm][i].eta = 0;  /* fool zero tf */
                }
                if (tf[tm][i].dest != 0) {
                    if (tm == player) {
                        dx = stars[tf[tm][i].dest].x;
                        dy = stars[tf[tm][i].dest].y;
                        ratio =
                            1.0 -
                            ((float) tf[tm][i].eta / tf[tm][i].origeta);
                        tf[tm][i].x =
                            tf[tm][i].xf +
                            conq_round(ratio * (dx - tf[tm][i].xf));
                        tf[tm][i].y =
                            tf[tm][i].yf +
                            conq_round(ratio * (dy - tf[tm][i].yf));
                        if (tf[tm][i].eta == 0) {
                            pplanet = stars[tf[tm][i].dest].first_planet;
                            while (pplanet != nil) {
                                pplanet->psee_capacity = pplanet->capacity;
                                pplanet = pplanet->next;
                            }
                            player_arrivals[tf[tm][i].dest] = true;
                            if (!stars[tf[tm][i].dest].visit[tm]) {
                                board[tf[tm][i].x][tf[tm][i].y].enemy =
                                    ' ';
                                update_board(tf[tm][i].x, tf[tm][i].y,
                                             left);
                                stars[tf[tm][i].dest].visit[tm] = true;
                            };
                        };
                    };
                    if ((tm == ENEMY) && (tf[tm][i].eta == 0)) {
                        pplanet = stars[tf[tm][i].dest].first_planet;
                        stars[tf[tm][i].dest].visit[ENEMY] = true;
                        while (pplanet != nil) {
                            pplanet->esee_team = pplanet->team;
                            pplanet = pplanet->next;
                        };
                        if (tf_stars[tf[tm][i].dest][ENEMY] > 0) {
                            there = 1;
                            while ((there == i)
                                   || (tf[ENEMY][there].dest !=
                                       tf[ENEMY][i]
                                       .dest)
                                   || (tf[ENEMY][there].eta != 0))
                                there = there + 1;
                            joinsilent(ENEMY, &tf[ENEMY][i],
                                       &tf[ENEMY][there]);
                        };
                        if ((tf_stars[tf[tm][i].dest][player] > 0) ||
                            (col_stars[tf[tm][i].dest][player] > 0))
                            enemy_arrivals[tf[tm][i].dest] = true;
                    };
                    if (tf[tm][i].eta == 0)
                        tf_stars[tf[tm][i].dest][tm]++;
                };
            };
        };
        tm++;
    }
    while (tm != none);
    /* put the good guys on the board */
    for (i = 1; i <= 26; i++) {
        if (tf[player][i].dest != 0) {
            tf[player][i].blasting = false;
            dx = tf[player][i].x;
            dy = tf[player][i].y;
            if (board[dx][dy].tf == ' ')
                board[dx][dy].tf = i + 'a' - 1;
            else if (board[dx][dy].tf != i + 'a' - 1)
                board[dx][dy].tf = '*';
            update_board(dx, dy, right);
        };
    };
    any = false;
    for (i = 1; i <= nstars; i++) {
        if (player_arrivals[i]) {
            if ((!any)) {
                point(33, 21);
                printw("Player arrivals :               ");
                point(50, 21);
                any = true;
            };
            addch(i + 'A' - 1);
            player_arrivals[i] = false;
        };
    };
    if ((!any)) {
        point(33, 21);
        printw(blank_line);
    };
    any = false;
    for (i = 1; i <= nstars; i++) {
        if (enemy_arrivals[i]) {
            if ((!any)) {
                point(33, 22);
                printw("Enemy arrivals  :               ");
                point(50, 22);
                any = true;
            };
            addch(i + 'A' - 1);
            enemy_arrivals[i] = false;
        };
    };
    if ((!any)) {
        point(33, 22);
        printw(blank_line);
    };
    any = false;
    for (i = 1; i <= nstars; i++) {
        if (en_departures[i]) {
            if (!any) {
                point(33, 23);
                printw("Enemy departures:               ");
                point(50, 23);
                any = true;
            };
            addch(i + 'A' - 1);
            en_departures[i] = false;
        };
    };
    if ((!any)) {
        point(33, 23);
        printw(blank_line);
    };
    for (i = 1; i <= nstars; i++)
        revolt(i);
}
