#ifndef MAIN_C_MAP_H
#define MAIN_C_MAP_H

enum types_map {
    simple = 0, house, lightOn, lightOff, openHole, blockedHole, extra, openExit, blockedExit
};

struct map {
    int type;
    char character[4];
    int is_on;
};
struct map map[13][9];

//we give a number to each lamp
static int num_lights;

//we give a number to each hole
static int num_holes;

//set background of name of characters to blue
void color_character(int, int);

//set default color
void color_normal();

void draw_map();

//read types of each home from file
void type_map(FILE *);

void set_character(int, int, char s[3]);

char *print_types(int n);

#endif //MAIN_C_MAP_H
