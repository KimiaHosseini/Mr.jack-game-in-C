#ifndef MAIN_C_ACTION_H
#define MAIN_C_ACTION_H

#include "map.h"

enum characters {
    JW = 0, WG, IL, JS, SH, MS, JB, SG
};
enum directions {
    North = 1, NorthEast, SouthEast, South, SouthWest, NorthWest
};

struct coordinate {
    int x;
    int y;
};
struct coordinate coordinate[8];

struct lamp_coordinate {
    int x;
    int y;
};
struct lamp_coordinate lamp_coordinate[8];

struct hole_coordinate {
    int x;
    int y;
};
struct hole_coordinate hole_coordinate[8];
struct card *suspected;

struct flashlight {
    int x;
    int y;
    int way;
};
struct flashlight flashlight;

//give the number of character and return its name as a string
char *name(int);

//give the name of character and return its num
int num_character(char *);

//check that going to a home is possible or not
int is_possible(int, int, int, int, int, int, int, int, int);

//chek if a character has # or not
int check(char s[4]);

//write name of characters for coordinates
int draw_move(int, int, int, int);

//check number of moves of characters
void num_of_moves(int *, int);

void move(int, int, int, int);

int move_hole(int *, int *, int, int, int);

//JW illuminate a way which its data will be saved in flashlight
struct flashlight action_JW();

void swap_coordinates(int *, int *);

//change place of 2 characters and show on the map
void action_WG();

//if try to block a blocked exit way return false else return true
int is_correct_to_block(int);

//if try to open an open exit way or open an exit way that was blocked now
//return false else return true
int is_correct_to_open(int, int);

//block an exit way and open one
void action_IL();

//turn a lamp off and turn one on
void action_JS();

//show one of the suspected cards and delete that from list
void action_SH(int, int, int *);

//open a hole and close one
void action_JB();

//find distance between destination of a character with SG and origin of the character with SG and compare
int is_correct_SG(int, int, int, int, int, int);

//like move func
void move_action_sg(int, int, int *, int *, int, int, int);

//choose some characters and move them
void action_SG();

//change background color when someone wins
void color();

#endif //MAIN_C_ACTION_H