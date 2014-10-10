#include <stdio.h>
#include "defs.h"
#include "structs.h"
#include "vars.h"


make_tf()
{
        char task; 
        int tf_num; 
        boolean error; 
        int new_tf;
        printf("ew tf- from tf:");
        get_char(&task);
        cle3r_left();
        tf_num = task-'A'+1;
        error= (tf_num<1) || (tf_num>26);
        if ( ! error ) error= ( tf[player][tf_num].eta!=0) ||
            (tf[player][tf_num].dest==0);
        if ( error  ) {
                error_message();
                printf("  !illegal tf");
        }
        else if ( tf[player][tf_num].blasting ) {
                error = true;
                error_message();
                printf(" !Tf is blasting a planet     ");
        }
        else {
                point(1,19);
                split_tf(&tf_num,&new_tf);
                point(1,20);
                pr3nt_tf(new_tf);
                point(1,21);
                pr3nt_tf(tf_num);
        };
}


split_tf(Tf2num, New2tf)
int *Tf2num, *New2tf;
{
        int tf_num, new_tf;
        char ships; 
        int x,y,n_ships;
        int ind; 
        char iline[81];
        new_tf = *New2tf;  
        tf_num = *Tf2num;
        get_tf(player,&new_tf,tf[player][tf_num].dest);
        tf_stars[tf[player][tf_num].dest][player]++;
        printf(" ships:");
        point(8,y_cursor);
        get_line(iline,&ind,false);
        get_token(iline,&ind,&n_ships,&ships);
        if ( ships==' ' ) { /*entire fleet*/
                tf[player][new_tf].s=tf[player][tf_num].s;
                tf[player][new_tf].t=tf[player][tf_num].t;
                tf[player][new_tf].c=tf[player][tf_num].c;
                tf[player][new_tf].b=tf[player][tf_num].b;
                tf[player][tf_num].s=0;
                tf[player][tf_num].t=0;
                tf[player][tf_num].c=0;
                tf[player][tf_num].b=0;
        } 
        else {
                do {
                        switch ( ships ) {
                        case 'T': 
                                if ( tf[player][tf_num].t < n_ships )
                                        n_ships= tf[player][tf_num].t;
                                tf[player][tf_num].t=tf[player][tf_num].t-n_ships;
                                tf[player][new_tf].t=tf[player][new_tf].t+n_ships;
                                break;
                        case 'S': 
                                if ( tf[player][tf_num].s <n_ships )
                                        n_ships=tf[player][tf_num].s;
                                tf[player][tf_num].s=tf[player][tf_num].s-n_ships;
                                tf[player][new_tf].s=tf[player][new_tf].s+n_ships;
                                break;
                        case 'C': 
                                if ( tf[player][tf_num].c < n_ships )
                                        n_ships=tf[player][tf_num].c;
                                tf[player][tf_num].c=tf[player][tf_num].c-n_ships;
                                tf[player][new_tf].c=tf[player][new_tf].c+n_ships;
                                break;
                        case 'B': 
                                if ( tf[player][tf_num].b < n_ships )
                                        n_ships=tf[player][tf_num].b;
                                tf[player][tf_num].b=tf[player][tf_num].b-n_ships;
                                tf[player][new_tf].b=tf[player][new_tf].b+n_ships;
                                break;
                        default:
                                error_message();
                                printf("  ! Illegal field %c",ships);
                        }; /*switch (*/
                        get_token(iline,&ind,&n_ships,&ships);
                } 
                while (ships != ' ');
        };
        x= tf[player][tf_num].x; 
        y=tf[player][tf_num].y;
        zero_tf(player,tf_num);
        zero_tf(player,new_tf);
        on_board(x,y);
        *Tf2num = tf_num; 
        *New2tf = new_tf;
}

join_tf()
{
        char tf1,tf2; 
        int tf1n,tf2n,ind,value;
        char iline[81];
        printf("oin tfs:");
        ind = 1;
        get_line(iline,&ind,true);
        cle3r_left();
        get_token( iline,& ind,& value,& tf1);
        tf1n=tf1-'A'+1;
        if ( (tf1n<1) || (tf1n>26) ) {
                error_message();
                printf("  ! illegal tf %c",tf1);
        }
        else if ( (tf[player][tf1n].eta)>0 ) {
                error_message();
                printf("  !tf%c is not in normal space ", tf1);
        }
        else if ( tf[player][tf1n].dest==0 ) {
                error_message();
                printf("  !nonexistent tf");
        }
        else if ( tf[player][tf1n].blasting ) {
                error_message();
                printf("  !Tf is blasting a planet    ");
        }
        else {
                get_token(iline,&ind,&value,&tf2);
                while ( tf2!=' ' ) {
                        tf2n = tf2-'A'+1;
                        if ( (tf2n<1) || (tf2n>26) ) {
                                error_message();
                                printf("  !illegal tf %c",tf2);
                        }
                        else if ( tf2n == tf1n) {
                                error_message();
                                printf("!Duplicate tf %c",tf2);
                        }
                        else if ( (tf[player][tf2n].dest == 0 ) ) {
                                error_message();
                                printf("!Nonexistant TF%c",tf2);
                        }
                        else if ( (tf[player][tf2n].x!=tf[player][tf1n].x) ||
                            (tf[player][tf2n].y!= tf[player][tf2n].y) ) {
                                error_message();
                                printf("  !tf%c bad location", tf2);
                        }
                        else if ( tf[player][tf2n].eta !=0 ) {
                                error_message();
                                printf("  !tf%c is not in normal space ", tf2);
                        }
                        else if ( tf[player][tf2n].blasting ) {
                                error_message();
                                printf(" !Tf%c is blasting a planet    ", tf2);
                        }
                        else {
                                joinsilent(player,&tf[player][tf1n],&tf[player][tf2n]);
                        };
                        get_token(iline,&ind,&value,&tf2);
                };
                on_board(tf[player][tf1n].x,tf[player][tf1n].y);
                point(1,19);
                pr3nt_tf(tf1n);
        };
}

