#include <stdio.h>
#include <string.h>
#include <windows.h>
#include "action.h"

//set background of name of characters to blue
void color_character(int i, int j) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (strncmp(map[i][j].character, "  ", 2) != 0)
        SetConsoleTextAttribute(hConsole, BACKGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
    else
        SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
}

//set default color
void color_normal() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
}

//read types of each home from file
void type_map(FILE *type) {
    for (int i = 0; i < 13; ++i) {
        for (int j = 0; j < 9; ++j) {
            fscanf(type, "%d", &map[i][j].type);
        }
    }
}

void set_character(int x, int y, char s[4]) {
    strcpy(map[x][y].character, s);
    map[x][y].character[3] = '\0';
}

char *print_types(int n) {
    char *s = (char *) malloc(8 * sizeof(char));
    if (n == house) {
        strcpy(s, "  HOUSE");
    } else if (n == lightOn) {
        num_lights++;
        // '+' means that this light is on
        strcpy(s, "+LIGHT");
        //allocate memory for string which contain number of this light
        char *temp = malloc(sizeof(char));
        //cast this number to string
        sprintf(temp, "%d", num_lights);
        //concatenate this string to "+light"
        strncat(s, temp, 1);
    } else if (n == lightOff) {
        num_lights++;
        // '-' means that this light is on
        strcpy(s, "-LIGHT");
        //allocate memory for string which contain number of this light
        char *temp = malloc(sizeof(char));
        //cast this number to string
        sprintf(temp, "%d", num_lights);
        //concatenate this string to "-light"
        strncat(s, temp, 1);
    } else if (n == openHole) {
        num_holes++;
        // '+' means that this hole is open
        strcpy(s, " +HOLE");
        //allocate memory for string which contain number of this hole
        char *temp = malloc(sizeof(char));
        //cast this number to string
        sprintf(temp, "%d", num_holes);
        //concatenate this string to "+hole"
        strncat(s, temp, 1);
    } else if (n == blockedHole) {
        num_holes++;
        // '-' means that this hole is blocked
        strcpy(s, " -HOLE");
        //allocate memory for string which contain number of this hole
        char *temp = malloc(sizeof(char));
        //cast this number to string
        sprintf(temp, "%d", num_holes);
        //concatenate this string to "-hole"
        strncat(s, temp, 1);
    } else if (n == extra || n == blockedExit) {
        strcpy(s, "*******");
    } else if (n == simple)
        strcpy(s, "       ");
    else if (n == openExit)
        strcpy(s, "  EXIT ");

    return s;
}

//by finding formulas for index of map array based on i in the loop
void draw_map() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    printf("\n");
    printf("     ");
    //first line of map
    for (int i = 1; i <= 7; ++i) {
        color_normal();
        if (i == 7)
            printf("          ");
        else
            printf("          ______");
    }
    printf("\n");
    for (int i = 1; i <= 38; ++i) {
        printf("     ");
        if (i % 4 == 1) {
            for (int j = 0; j < 7; ++j) {
                if (i == 1 && j == 0) {
                    color_normal();
                    printf("         /  ");
                    color_character(1, 8);
                    printf("%s", map[1][8].character);
                    color_normal();
                    printf(" ");
                } else if (i == 1 && j == 6) {
                    color_normal();
                    printf("\\        ");
                } else if (i == 1) {
                    color_normal();
                    printf("\\        /  ");
                    color_character(2 * j + 1, 8);
                    printf("%s", map[2 * j + 1][8].character);
                    color_normal();
                    printf(" ");
                } else {
                    if (j == 6 || i == 37) {
                        color_normal();
                        printf("\\ %s/      ", print_types(map[2 * j][(37 - i) / 4].type));
                    } else {
                        color_normal();
                        printf("\\ %s/  ", print_types(map[2 * j][(37 - i) / 4].type));
                        color_character(2 * j + 1, (37 - i) / 4 - 1);
                        printf("%s", map[2 * j + 1][(37 - i) / 4 - 1].character);
                        color_normal();
                        printf(" ");
                    }
                }
            }
            printf("\n");
        } else if (i % 4 == 2) {
            color_normal();
            for (int j = 0; j < 7; ++j) {
                if (i == 2 && j == 0)
                    printf("  ______/       ");
                else if (i == 2 && j == 6)
                    printf(" \\______ ");
                else
                    printf(" \\______/       ");
            }
            printf("\n");
        } else if (i % 4 == 3) {
            for (int j = 0; j < 7; ++j) {
                if (j == 6) {
                    color_normal();
                    printf(" /  ");
                    color_character(2 * j, (35 - i) / 4);
                    printf("%s", map[2 * j][(35 - i) / 4].character);
                    color_normal();
                    printf(" \\       ");
                } else {
                    color_normal();
                    printf(" /  ");
                    color_character(2 * j, (35 - i) / 4);
                    printf("%s", map[2 * j][(35 - i) / 4].character);
                    color_normal();
                    printf(" \\%s", print_types(map[2 * j + 1][(35 - i) / 4].type));
                }
            }
            printf("\n");
        } else if (i % 4 == 0) {
            color_normal();
            for (int j = 0; j < 7; ++j) {
                if (j == 6)
                    printf("/        \\");
                else
                    printf("/        \\______");
            }
            printf("\n");
        }
    }
    num_lights = 0;
    num_holes = 0;
}