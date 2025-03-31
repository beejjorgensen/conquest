#include "structs.h"

int r2nge[nstars+1][nstars+1],
    tf_stars[nstars+1][2],
    col_stars[nstars+1][2];

char en_research;

sector board[bdsize+1][bdsize+1];

ttf tf[2][27];

tstar stars[nstars+1];

float growth_rate[2];

int vel[2],range[2],weapons[2],
    weap_working[2],ran_working[2],vel_working[2],
    weap_req[11], ran_req[bdsize+1],
    vel_req[max_vel+1], turn,production_year,
    enemy_arrivals[nstars+1], en_departures[nstars+1],
    player_arrivals[nstars+1], game_over,
    bottom_field;

int x_cursor, y_cursor,
    saved_game,
    left_line[25], debug;

