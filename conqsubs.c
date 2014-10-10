#include <stdio.h>
#include <curses.h>
#include "defs.h"
#include "structs.h"
#include "vars.h"

ressum()
{
        char key;
        int value, ind;
        char iline[81];
        printw("esearch field(s):");
        get_line(iline,&ind,true);
        get_token(iline,&ind,&value,&key);
        cle3r_left();
        if ( key==' ') {
                pr2nt_res('R');
                pr2nt_res('V');
                pr2nt_res('W');
        } 
        else {
                do {
                        pr2nt_res(key);
                        get_token(iline,&ind,&value,&key);
                } 
                while (key != ' ');
        };
}


pr2nt_res(field)
char field;
{
        switch ( field ) {
        case 'V':
                point(53,18);
                printw("V:%2d",vel[player]);
                if ( vel[player] < max_vel )
                        printw(" res: %3d need:%4d",vel_working[player],
                        vel_req[vel[player]+1]);
                else printw("                   ");
                break;
        case 'R':
                point( 53,19);
                printw("R:%2d",range[player]);
                if ( range[player] < bdsize )
                        printw(" res: %3d need:%4d",ran_working[player],
                        ran_req[range[player]+1]);
                else printw("                   ");
                break;
        case 'W':
                point(53,20);
                printw("W:%2d",weapons[player]);
                if ( weapons[player] < 10 )
                        printw(" res: %3d need:%4d",weap_working[player],
                        weap_req[weapons[player]+1]);
                else printw("                   ");
                break;
        }; /*switch (*/
}

research(team, field, amt)
int team;
char field;
int amt;
{
        switch ( field ) {
        case 'W':
                if ( weapons[team]<10 ) {
                        weap_working[team]=weap_working[team]+amt;
                        amt=0;
                        if ( weap_working[team]>= weap_req[weapons[team]+1] ) {
                                amt=weap_working[team]
                                                - weap_req[weapons[team]+1];
                                weapons[team]=weapons[team]+1;
                                if ( team==ENEMY ) {
                                        new_research();
                                        field = en_research;
                                };
                                weap_working[team]=0;
                                research(team,field,amt);
                        };
                };
                break;
        case 'R':
                if ( range[team] < bdsize ) {
                        ran_working[team]=ran_working[team]+amt;
                        amt=0;
                        if ( ran_working[team] >= ran_req[range[team]+1] )
                                {
                                amt= ran_working[team]
                                                - ran_req[range[team]+1];
                                range[team]=range[team]+1;
                                if ( team==ENEMY ) {
                                        new_research();
                                        field = en_research;
                                };
                                ran_working[team]=0;
                                research(team,field,amt);
                        };
                };
                break;
        case 'V':
                if ( vel[team] <max_vel ) {
                        vel_working[team]= vel_working[team]+amt;
                        amt=0;
                        if ( vel_working[team] >= vel_req[vel[team]+1] ) {
                                amt= vel_working[team]-vel_req[vel[team]+1];
                                vel[team]=vel[team]+1;
                                if ( team==ENEMY ) {
                                        new_research();
                                        field = en_research;
                                };
                                vel_working[team]=0;
                                research(team,field,amt);
                        };
                };
                break;
        default: 
                printw("error!!!! in research field %c\n", field);
        }; /*switch (*/
}

