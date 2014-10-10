#ifndef GLOBAL_DEFINITION_OR_EXTERN
#define GLOBAL_DEFINITION_OR_EXTERN extern
#endif

GLOBAL_DEFINITION_OR_EXTERN int r2nge[nstars+1][nstars+1],
           tf_stars[nstars+1][2],
           col_stars[nstars+1][2];

GLOBAL_DEFINITION_OR_EXTERN char en_research;

GLOBAL_DEFINITION_OR_EXTERN struct {
        int enemy, star, tf;
        } board[bdsize+1][bdsize+1];

GLOBAL_DEFINITION_OR_EXTERN struct {
        int x, y;
        struct stplanet *first_planet;
        int visit[2];
        } stars[nstars+1];

GLOBAL_DEFINITION_OR_EXTERN struct {
        int x, y, xf, yf, s, t, c, b, dest, eta, origeta;
        int blasting, withdrew;
        } tf[2][27];

GLOBAL_DEFINITION_OR_EXTERN float growth_rate[2];

GLOBAL_DEFINITION_OR_EXTERN int vel[2],range[2],weapons[2],
           weap_working[2],ran_working[2],vel_working[2],
           weap_req[11], ran_req[bdsize+1],
           vel_req[max_vel+1], turn,production_year,
           enemy_arrivals[nstars+1], en_departures[nstars+1],
           player_arrivals[nstars+1], game_over,
           bottom_field;

GLOBAL_DEFINITION_OR_EXTERN termtype        terminal_type;

GLOBAL_DEFINITION_OR_EXTERN int x_cursor, y_cursor,
           saved_game,
           left_line[25], debug;

GLOBAL_DEFINITION_OR_EXTERN int raw_fd;

