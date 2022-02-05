#include <stdio.h>
#include <string.h>
#include <windows.h>
#include "map.h"
#include "cards.h"
#include <unistd.h>
#include "round.h"
#include <math.h>

//#include <dos.h>
enum characters {
    JW = 0, WG, IL, JS, SH, MS, JB, SG
};
enum directions {
    North = 1, NorthEast, SouthEast, South, SouthWest, NorthWest
};
//coordinate of our 8 characters
struct coordinate {
    int x;
    int y;
};
struct coordinate coordinate[8];

//coordinate of our 8 lamps
struct lamp_coordinate {
    int x;
    int y;
};
struct lamp_coordinate lamp_coordinate[8];

//coordinate of our 8 holes
struct hole_coordinate {
    int x;
    int y;
};
struct hole_coordinate hole_coordinate[8];

//change the background color when someone wins
void color() {
    system("color 10");
    sleep(1);
    system("color 20");
    sleep(1);
    system("color 30");
    sleep(1);
    system("color 40");
    sleep(1);
    system("color B0");
    sleep(1);
    system("color C0");
    sleep(1);
    system("color D0");
    sleep(1);
    system("color E0");
    sleep(1);
    system("color F0");
    sleep(1);
    system("color 50");
    sleep(1);
    system("color 60");
    sleep(1);
    system("color 80");
    sleep(1);
    system("color B0");
    sleep(7);
};

//give the number of character and return its name as a string
char *name(int character) {
    char *s = (char *) malloc(4 * sizeof(char));
    if (character == JW)
        s = "JW";
    else if (character == WG)
        s = "WG";
    else if (character == IL)
        s = "IL";
    else if (character == JS)
        s = "JS";
    else if (character == SH)
        s = "SH";
    else if (character == MS)
        s = "MS";
    else if (character == JB)
        s = "JB";
    else if (character == SG)
        s = "SG";
    return s;
}

//give the name of character and return its num
int num_character(char *character) {
    if (strncmp(character, "JW", 2) == 0)
        return JW;
    else if (strncmp(character, "WG", 2) == 0)
        return WG;
    else if (strncmp(character, "IL", 2) == 0)
        return IL;
    else if (strncmp(character, "JS", 2) == 0)
        return JS;
    else if (strncmp(character, "SH", 2) == 0)
        return SH;
    else if (strncmp(character, "MS", 2) == 0)
        return MS;
    else if (strncmp(character, "JB", 2) == 0)
        return JB;
    else if (strncmp(character, "SG", 2) == 0)
        return SG;
}


//check that going to a home is possible or not
int is_possible(int x_first, int y_first, int x, int y, int character, int num, int i, int jack, int round) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    //if we were in the last move and we want to go to the first home that show an error
    if (i == (num - 1) && x == x_first && y == y_first) {
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
        printf("U CAN'T COME BACK TO YOUR FIRST PLACE!\n");
        printf("\a");
        return 0;
    }
    //if player is playing with MS and is not in the last move , can pass
    //houses with types:simple,holes,house,lights
    if (character == MS && i != (num - 1)) {
        if ((map[x][y].type == simple || map[x][y].type == openHole ||
             map[x][y].type == blockedHole || map[x][y].type == house ||
             map[x][y].type == lightOn || map[x][y].type == lightOff)
            && strcmp(map[x][y].character, "   ") == 0) {
            return 1;
        } else {
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
            printf("U CAN'T GO TO THIS HOME!\n");
            printf("\a");
            return 0;
        }
        //if player is playing with character of jack
        // and jack was invisible in the last round
        //and we are not in first round , player can pass places with types:
        //simple , holes , exit ways
    } else if (character == jack && visible_jack[round - 1] == 0 && round != 1) {
        if ((map[x][y].type == simple || map[x][y].type == openHole ||
             map[x][y].type == blockedHole || map[x][y].type == openExit)) {
            return 1;
        } else {
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
            printf("U CAN'T GO TO THIS HOME!\n");
            printf("\a");
            return 0;
        }
    }
        //in normal state can pass houses with types:
        //simple,holes
    else {
        if ((map[x][y].type == simple || map[x][y].type == openHole || map[x][y].type == blockedHole) &&
            strcmp(map[x][y].character, "   ") == 0) {
            return 1;
        } else {
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
            printf("U CAN'T GO TO THIS HOME!\n");
            printf("\a");
            return 0;
        }
    }
}

//chek if a character has # or not
int check(char s[4]) {
    if (s[2] == '#')
        return 1;
    else
        return 0;
}

//write name of characters for coordinates
int draw_move(int x, int y, int character, int temp) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    coordinate[character].x = x;
    coordinate[character].y = y;
    char s[4];
    strcpy(s, name(character));
    //if it has #
    if (temp) {
        s[2] = '#';
        set_character(x, y, s);
    }
        //if it doesn't have #
    else {
        s[2] = ' ';
        set_character(x, y, s);
    }
    system("cls");
    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
    Beep(75, 300);
    draw_map();
    //if the type of this coordinate was exit way:
    if (map[x][y].type == openExit) {
        for (int i = 0; i < 40; ++i) {
            for (int j = 0; j < 25; ++j) {
                printf("MR.JACK WON!");
            }
            printf("\n");
        }
        color();
        exit(0);
    }
    return 1;
}

//check number of moves of characters
void num_of_moves(int *num, int character) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    scanf("%d", num);
    //all characters can pass max 3 moves except MS that can pass max 4 moves
    if ((character != MS && (*num > 3 || *num < 1))) {
        SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_RED);
        printf("INVALID INPUT! TRY AGAIN");
        printf("\a");
        num_of_moves(num, character);
    } else if (character == MS && (*num > 4 || *num < 1)) {
        printf("INVALID INPUT! TRY AGAIN");
        printf("\a");
        num_of_moves(num, character);
    }
}

int move_hole(int *x, int *y, int i, int num, int character) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_RED);
    int temp;
    //ask player that wants to go to the hole or continue moving in the path
    printf("IF YOU WANT TO GO TO THE HOLE ENTER 1 ELSE ENTER 2");
    scanf("%d", &temp);
    if (temp == 1) {
        int hole;
        SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
        printf("ENTER NUMBER OF HOLE YOU WANT TO GO IN: ");
        scanf("%d", &hole);
        //while player choose a hole that is blocked or a character is on it show error and give another hole number
        while (map[hole_coordinate[hole - 1].x][hole_coordinate[hole - 1].y].type == blockedHole
               || strcmp(map[hole_coordinate[hole - 1].x][hole_coordinate[hole - 1].y].character, "   ") != 0) {
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
            printf("THIS HOLE IS BLOCKED OR A CHARACTER IS ON THIS HOLE!TRY AGAIN ");
            printf("\a");
            scanf("%d", &hole);
        }
        char s[4];
        strcpy(s, name(character));
        //set # or ' '
        if (map[*x][*y].character[2] == '#')
            strcat(s, "#");
        else
            strcat(s, " ");
        //set character of origin hole to "   "
        set_character(*x, *y, "   ");
        //set character of destination hole to name of the character that is moving
        set_character(hole_coordinate[hole - 1].x, hole_coordinate[hole - 1].y, s);
        //set coordinate of this character to coordinate of destination hole
        *x = hole_coordinate[hole - 1].x;
        *y = hole_coordinate[hole - 1].y;
        coordinate[character].x = *x;
        coordinate[character].y = *y;
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        system("cls");
        draw_map();
        Beep(75, 300);
        return 1;
    } else
        //if player doesn't want to go to the hole
        return 0;
}

void move(int character, int jack, int round, int arrest) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_RED);
    printf("HOW MANY HOMES DO U WANT TO MOVE %s?", name(character));
    int num;
    //check that the number of moves is valid or not
    num_of_moves(&num, character);
    int x = coordinate[character].x, y = coordinate[character].y;
    //keep x_first & y_first as coordinate of origin of character
    int x_first = x, y_first = y;
    //go_hole is used for check that this character had gone to the hole or no
    int go_hole = 0;
    for (int i = 0; i < num; ++i) {
        //if the character is now on an open hole and is not in the last move
        // and have not gone to the hole call move_hole func
        if (i != num && map[x][y].type == openHole && go_hole == 0) {
            go_hole = move_hole(&x, &y, i, num, character);
            if (go_hole)
                continue;
        }
        int direction;
        SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
        printf("IN WHICH DIRECTION DO U WANT TO MOVE?\n1 NORTH, 2 NORTH-EAST, 3 SOUTH-EAST, 4 SOUTH, 5 SOUTH-WEST, 6 NORTH-WEST\n");
        scanf("%d", &direction);
        int temp = check(map[x][y].character);
        while (direction != 1 && direction != 2 && direction != 3 && direction != 4 && direction != 5 &&
               direction != 6) {
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
            printf("WRONG INPUT! TRY AGAIN ");
            printf("\a");
            scanf("%d", &direction);
        }
        if (direction == North) {
            //check possibility for x,y+1
            if (is_possible(x_first, y_first, x, y + 1, character, num, i, jack, round)) {
                set_character(x, y, "   ");
                y++;
                draw_move(x, y, character, temp);
            } else
                //if this direction is not possible subtract from loop counter(i) one
                i--;
        } else if (direction == NorthEast) {
            if (x % 2 == 0) {
                if (is_possible(x_first, y_first, x + 1, y, character, num, i, jack, round)) {
                    set_character(x, y, "   ");
                    x++;
                    draw_move(x, y, character, temp);
                } else
                    i--;
            } else {
                if (is_possible(x_first, y_first, x + 1, y + 1, character, num, i, jack, round)) {
                    set_character(x, y, "   ");
                    x++;
                    y++;
                    draw_move(x, y, character, temp);
                } else
                    i--;
            }
        } else if (direction == SouthEast) {
            if (x % 2 == 0) {
                if (is_possible(x_first, y_first, x + 1, y - 1, character, num, i, jack, round)) {
                    set_character(x, y, "   ");
                    x++;
                    y--;
                    draw_move(x, y, character, temp);
                } else
                    i--;
            } else {
                if (is_possible(x_first, y_first, x + 1, y, character, num, i, jack, round)) {
                    set_character(x, y, "   ");
                    x++;
                    draw_move(x, y, character, temp);
                } else
                    i--;
            }
        } else if (direction == South) {
            if (is_possible(x_first, y_first, x, y - 1, character, num, i, jack, round)) {
                set_character(x, y, "   ");
                y--;
                draw_move(x, y, character, temp);
            } else
                i--;
        } else if (direction == SouthWest) {
            if (x % 2 == 0) {
                if (is_possible(x_first, y_first, x - 1, y - 1, character, num, i, jack, round)) {
                    set_character(x, y, "   ");
                    x--;
                    y--;
                    draw_move(x, y, character, temp);
                } else
                    i--;
            } else {
                if (is_possible(x_first, y_first, x - 1, y, character, num, i, jack, round)) {
                    set_character(x, y, "   ");
                    x--;
                    draw_move(x, y, character, temp);
                } else
                    i--;
            }
        } else if (direction == NorthWest) {
            if (x % 2 == 0) {
                if (is_possible(x_first, y_first, x - 1, y, character, num, i, jack, round)) {
                    set_character(x, y, "   ");
                    x--;
                    draw_move(x, y, character, temp);
                } else
                    i--;
            } else {
                if (is_possible(x_first, y_first, x - 1, y + 1, character, num, i, jack, round)) {
                    set_character(x, y, "   ");
                    x--;
                    y++;
                    draw_move(x, y, character, temp);
                } else
                    i--;
            }
        }
    }
    //if detective says that wants to arrest jack
    if (arrest == 1) {
        printf("WHO DO U WANT TO ARREST?");
        char s[3];
        scanf("%s", s);
        //check all places near detective
        //if the character that detective wants to arrest is not near the detective show an error
        // else if this character is mr,jack print that detective wins else mr.jack wins
        if (x % 2 == 0) {
            //if x%2==0 & x==0 we should not check northwest and southwest
            if (x == 0) {
                if (strncmp(map[x][y + 1].character, s, 2) != 0 && strncmp(map[x + 1][y].character, s, 2) != 0 &&
                    strncmp(map[x + 1][y - 1].character, s, 2) != 0 && strncmp(map[x][y - 1].character, s, 2) != 0) {
                    SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
                    printf("U SHOULD FIRST GO NEAR A CHARACTER AND THEN ARREST!\n\a");
                } else {
                    if (strcmp(name(jack), s) == 0) {
                        sleep(2);
                        system("cls");
                        SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
                        for (int i = 0; i < 22; ++i) {
                            for (int j = 0; j < 10; ++j) {
                                printf("DETECTIVE WON!!!          ");
                            }
                            printf("\n");
                        }
                        color();
                        exit(0);
                    } else {
                        sleep(2);
                        system("cls");
                        printf("U ARREST A CHARACTER WHO IS NOT MR.JACK :( \n\n");
                        for (int i = 0; i < 22; ++i) {
                            for (int j = 0; j < 10; ++j) {
                                printf("MR.JACK WON!!          ");
                            }
                            printf("\n");
                        }
                        color();
                        exit(0);
                    }
                }
            }
                //if x%2==0 & x==12 we should not check northeast and southeast
            else if (x == 12) {
                if (strncmp(map[x][y + 1].character, s, 2) != 0 && strncmp(map[x][y - 1].character, s, 2) != 0
                    && strncmp(map[x - 1][y - 1].character, s, 2) != 0 && strncmp(map[x - 1][y].character, s, 2) != 0) {
                    SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
                    printf("U SHOULD FIRST GO NEAR A CHARACTER AND THEN ARREST!\n");
                } else {
                    if (strcmp(name(jack), s) == 0) {
                        sleep(2);
                        system("cls");
                        for (int i = 0; i < 22; ++i) {
                            for (int j = 0; j < 10; ++j) {
                                printf("DETECTIVE WON!!          ");
                            }
                            printf("\n");
                        }
                        color();
                        exit(0);
                    } else {
                        sleep(2);
                        system("cls");
                        printf("U ARREST A CHARACTER WHO IS NOT MR.JACK :( \n\n");
                        for (int i = 0; i < 22; ++i) {
                            for (int j = 0; j < 10; ++j) {
                                printf("MR.JACK WON!!          ");
                            }
                            printf("\n");
                        }
                        color();
                        exit(0);
                    }
                }
            }
                //if x%2==0 & y==8 we should not check north
            else if (y == 8) {
                if (strncmp(map[x + 1][y].character, s, 2) != 0 && strncmp(map[x + 1][y - 1].character, s, 2) != 0
                    && strncmp(map[x][y - 1].character, s, 2) != 0 && strncmp(map[x - 1][y - 1].character, s, 2) != 0
                    && strncmp(map[x - 1][y].character, s, 2) != 0) {
                    SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
                    printf("U SHOULD FIRST GO NEAR A CHARACTER AND THEN ARREST!\n\a");
                } else {
                    if (strcmp(name(jack), s) == 0) {
                        sleep(2);
                        system("cls");
                        for (int i = 0; i < 22; ++i) {
                            for (int j = 0; j < 10; ++j) {
                                printf("DETECTIVE WON!!          ");
                            }
                            printf("\n");
                        }
                        color();
                        exit(0);
                    } else {
                        sleep(2);
                        system("cls");
                        printf("U ARREST A CHARACTER WHO IS NOT MR.JACK :( \n\n");
                        for (int i = 0; i < 22; ++i) {
                            for (int j = 0; j < 10; ++j) {
                                printf("MR.JACK WON!!          ");
                            }
                            printf("\n");
                        }
                        color();
                        exit(0);
                    }
                }
            } else {
                if (strncmp(map[x][y + 1].character, s, 2) != 0 && strncmp(map[x + 1][y].character, s, 2) != 0 &&
                    strncmp(map[x + 1][y - 1].character, s, 2) != 0 && strncmp(map[x][y - 1].character, s, 2) != 0
                    && strncmp(map[x - 1][y - 1].character, s, 2) != 0 && strncmp(map[x - 1][y].character, s, 2) != 0) {
                    SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
                    printf("U SHOULD FIRST GO NEAR A CHARACTER AND THEN ARREST!\n\a");
                } else {
                    if (strcmp(name(jack), s) == 0) {
                        sleep(2);
                        system("cls");
                        for (int i = 0; i < 22; ++i) {
                            for (int j = 0; j < 10; ++j) {
                                printf("DETECTIVE WON!!          ");
                            }
                            printf("\n");
                        }
                        color();
                        exit(0);
                    } else {
                        sleep(2);
                        system("cls");
                        printf("U ARREST A CHARACTER WHO IS NOT MR.JACK :( \n\n");
                        for (int i = 0; i < 22; ++i) {
                            for (int j = 0; j < 10; ++j) {
                                printf("MR.JACK WON!!          ");
                            }
                            printf("\n");
                        }
                        color();
                        exit(0);
                    }
                }
            }
        } else {
            //if x%2==1 & y==0 we should not check south
            if (y == 0) {
                if (strncmp(map[x][y + 1].character, s, 2) != 0 && strncmp(map[x + 1][y + 1].character, s, 2) != 0
                    && strncmp(map[x + 1][y].character, s, 2) != 0 && strncmp(map[x - 1][y].character, s, 2) != 0
                    && strncmp(map[x - 1][y + 1].character, s, 2) != 0) {
                    SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
                    printf("U SHOULD FIRST GO NEAR A CHARACTER AND THEN ARREST!\n\a");
                } else {
                    if (strcmp(name(jack), s) == 0) {
                        sleep(2);
                        system("cls");
                        for (int i = 0; i < 22; ++i) {
                            for (int j = 0; j < 10; ++j) {
                                printf("DETECTIVE WON!!          ");
                            }
                            printf("\n");
                        }
                        color();
                        exit(0);
                    } else {
                        sleep(2);
                        system("cls");
                        printf("U ARREST A CHARACTER WHO IS NOT MR.JACK :( \n\n");
                        for (int i = 0; i < 22; ++i) {
                            for (int j = 0; j < 10; ++j) {
                                printf("MR.JACK WON!!          ");
                            }
                            printf("\n");
                        }
                        color();
                        exit(0);
                    }
                }
            } else {
                if (strncmp(map[x][y + 1].character, s, 2) != 0 && strncmp(map[x + 1][y + 1].character, s, 2) != 0
                    && strncmp(map[x + 1][y].character, s, 2) != 0 && strncmp(map[x][y - 1].character, s, 2) != 0
                    && strncmp(map[x - 1][y].character, s, 2) != 0 && strncmp(map[x - 1][y + 1].character, s, 2) != 0) {
                    SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
                    printf("U SHOULD FIRST GO NEAR A CHARACTER AND THEN ARREST!\n\a");
                } else {
                    if (strcmp(name(jack), s) == 0) {
                        sleep(2);
                        system("cls");
                        for (int i = 0; i < 22; ++i) {
                            for (int j = 0; j < 10; ++j) {
                                printf("DETECTIVE WON!!          ");
                            }
                            printf("\n");
                        }
                        color();
                        exit(0);
                    } else {
                        sleep(2);
                        system("cls");
                        printf("U ARREST A CHARACTER WHO IS NOT MR.JACK :( \n\n");
                        for (int i = 0; i < 22; ++i) {
                            for (int j = 0; j < 10; ++j) {
                                printf("MR.JACK WON!          ");
                            }
                            printf("\n");
                        }
                        color();
                        exit(0);
                    }
                }
            }
        }
    }
}

struct flashlight {
    int x;
    int y;
    int way;
};
struct flashlight flashlight = {0};

//JW illuminate a way which its data will be saved
// in flashlight
struct flashlight action_JW() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_RED);
    printf("WHICH DIRECTION DO U WANT TO BE ILLUMINATED?\n1 NORTH, 2 NORTH-EAST, 3 SOUTH-EAST, 4 SOUTH, 5 SOUTH-WEST, 6 NORTH-WEST\n");
    int way;
    scanf("%d", &way);
    while (way != 1 && way != 2 && way != 3 && way != 4 && way != 5 && way != 6) {
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
        printf("INVALID INPUT! TRY AGAIN \a");
        scanf("%d", &way);
    }
    flashlight.way = way;
    int y = coordinate[JW].y, x = coordinate[JW].x;
    flashlight.x = x;
    flashlight.y = y;
    if (way == North) {
        //actually no place will be illuminated
        if (y == 8)
            return flashlight;
        //go to the north until there were no home or reach to a house or light
        while (y < 9 && map[x][y].type != house && map[x][y].type != lightOff && map[x][y].type != lightOn &&
               map[x][y].type != extra)
            map[x][++y].is_on = 1;
        return flashlight;
    } else if (way == NorthEast) {
        //actually no place will be illuminated
        if (y == 8 || x == 12)
            return flashlight;
        while (1) {
            //go to the northeast and set member 'is_on' of flashlight to 1
            // until there were no home or reach to a house or light
            if (x % 2 == 0) {
                x++;
                if (x >= 13 || map[x][y].type == house || map[x][y].type == lightOff || map[x][y].type == lightOn ||
                    map[x][y].type == extra)
                    break;
                map[x][y].is_on = 1;
                y++;
                if (y >= 9)
                    break;
            } else {
                x++;
                if (x >= 13 || map[x][y].type == house || map[x][y].type == lightOff || map[x][y].type == lightOn ||
                    map[x][y].type == extra)
                    break;
                map[x][y].is_on = 1;
            }
        }
        return flashlight;
    } else if (way == SouthEast) {
        //actually no place will be illuminated
        if (y == 0 || x == 12)
            return flashlight;
        while (1) {
            //go to the southeast and set member 'is_on' of flashlight to 1
            // until there were no home or reach to a house or light
            if (x % 2 == 1) {
                x++;
                if (x >= 13 || map[x][y].type == house || map[x][y].type == lightOff || map[x][y].type == lightOn ||
                    map[x][y].type == extra)
                    break;
                map[x][y].is_on = 1;
            } else {
                x++;
                y--;
                if (x >= 13 || y < 0 || map[x][y].type == house || map[x][y].type == lightOff ||
                    map[x][y].type == lightOn || map[x][y].type == extra)
                    break;
                map[x][y].is_on = 1;
            }
        }
        return flashlight;
    } else if (way == South) {
        if (y == 0)
            //actually no place will be illuminated
            return flashlight;
        while (y > -1 && map[x][y].type != house && map[x][y].type != lightOff && map[x][y].type != lightOn &&
               map[x][y].type != extra)
            //go to the south and set member 'is_on' of flashlight to 1
            // until there were no home or reach to a house or light
            map[x][--y].is_on = 1;
        return flashlight;
    } else if (way == SouthWest) {
        if (y == 0 || x == 0)
            //actually no place will be illuminated
            return flashlight;
        while (1) {
            if (x % 2 == 1) {
                x--;
                if (x < 0 || map[x][y].type == house || map[x][y].type == lightOff || map[x][y].type == lightOn ||
                    map[x][y].type == extra)
                    break;
                map[x][y].is_on = 1;
            } else {
                x--;
                y--;
                if (x < 0 || y < 0 || map[x][y].type == house || map[x][y].type == lightOff ||
                    map[x][y].type == lightOn || map[x][y].type == extra)
                    break;
                map[x][y].is_on = 1;
            }
        }
        return flashlight;
    } else if (way == NorthWest) {
        if (y == 8 || x == 0)
            //actually no place will be illuminated
            return flashlight;
        while (1) {
            if (x % 2 == 0) {
                x--;
                if (x < 0 || map[x][y].type == house || map[x][y].type == lightOff || map[x][y].type == lightOn ||
                    map[x][y].type == extra)
                    break;
                map[x][y].is_on = 1;
            } else {
                x--;
                y++;
                if (x < 0 || y > 12 || map[x][y].type == house || map[x][y].type == lightOff ||
                    map[x][y].type == lightOn || map[x][y].type == extra)
                    break;
                map[x][y].is_on = 1;
            }
        }
        return flashlight;
    }
}


void swap_coordinates(int *n, int *m) {
    int temp = *m;
    *m = *n;
    *n = temp;
}

//change place of 2 characters and show on the map
void action_WG() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
    printf("WHO DO U WANT TO CHANGE YOUR PLACE WITH?\n");
    char s[4];
    scanf("%s", s);
    int temp = num_character(s);
    int x_s = coordinate[temp].x, y_s = coordinate[temp].y;
    if (map[x_s][y_s].character[2] == '#')
        strcat(s, "#");
    else
        strcat(s, " ");
    strcpy(map[x_s][y_s].character, map[coordinate[WG].x][coordinate[WG].y].character);
    strcpy(map[coordinate[WG].x][coordinate[WG].y].character, s);
    swap_coordinates(&coordinate[temp].x, &coordinate[WG].x);
    swap_coordinates(&coordinate[temp].y, &coordinate[WG].y);
    system("cls");
    SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN);
    draw_map();
}

//if try to block a blocked exit way return false else return true
int is_correct_to_block(int num_exit_way) {
    if (num_exit_way == 1 && map[0][8].type == blockedExit)
        return 0;
    if (num_exit_way == 2 && map[11][8].type == blockedExit)
        return 0;
    if (num_exit_way == 3 && map[0][0].type == blockedExit)
        return 0;
    if (num_exit_way == 4 && map[11][0].type == blockedExit)
        return 0;
    return 1;
}

//if try to open an open exit way or open an exit way that was blocked now
//return false else return true
int is_correct_to_open(int num_exit_way, int temp1) {
    if (num_exit_way == 1 && map[0][8].type == openExit || num_exit_way == temp1)
        return 0;
    if (num_exit_way == 2 && map[11][8].type == openExit)
        return 0;
    if (num_exit_way == 3 && map[0][0].type == openExit)
        return 0;
    if (num_exit_way == 4 && map[11][0].type == openExit)
        return 0;
    return 1;
}

//block an exit way and open one
void action_IL() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_RED);
    printf("(TOP LEFT:1 ;TOP RIGHT:2 ;BOTTOM LEFT:3 ;BOTTOM RIGHT:4)\n");
    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
    printf("ENTER THE NUMBER OF EXIT-WAY U WANT TO BLOCK:");
    int temp1, temp2;
    scanf("%d", &temp1);
    while (!is_correct_to_block(temp1)) {
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
        printf("WRONG INPUT! TRY AGAIN \a");
        scanf("%d", &temp1);
    }
    if (temp1 == 1) {
        map[0][8].type = blockedExit;
        map[1][8].type = blockedExit;
    } else if (temp1 == 2) {
        map[11][8].type = blockedExit;
        map[12][8].type = blockedExit;
    } else if (temp1 == 3) {
        map[0][0].type = blockedExit;
        map[0][1].type = blockedExit;
        map[1][0].type = blockedExit;
    } else if (temp1 == 4) {
        map[11][0].type = blockedExit;
        map[12][0].type = blockedExit;
        map[12][1].type = blockedExit;
    }
    system("cls");
    SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN);
    draw_map();
    SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_RED);
    printf("(TOP LEFT:1 ;TOP RIGHT:2 ;BOTTOM LEFT:3 ;BOTTOM RIGHT:4)\n");
    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
    printf("ENTER THE NUMBER OF EXIT-WAY U WANT TO OPEN:\n");
    scanf("%d", &temp2);
    while (!is_correct_to_open(temp2, temp1)) {
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
        printf("WRONG INPUT! TRY AGAIN \a");
        scanf("%d", &temp2);
    }
    if (temp2 == 1) {
        map[0][8].type = openExit;
        map[1][8].type = openExit;
    } else if (temp2 == 2) {
        map[11][8].type = openExit;
        map[12][8].type = openExit;
    } else if (temp2 == 3) {
        map[0][0].type = openExit;
        map[0][1].type = openExit;
        map[1][0].type = openExit;
    } else if (temp2 == 4) {
        map[11][0].type = openExit;
        map[12][0].type = openExit;
        map[12][1].type = openExit;
    }
    system("cls");
    SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN);
    draw_map();
}

//turn a lamp off and turn one on
void action_JS() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
    printf("ENTER NUMBER OF LAMP YOU WANT TO TURN OFF: ");
    int temp1;
    scanf("%d", &temp1);
    //trying to turn off an off lamp
    while (map[lamp_coordinate[temp1 - 1].x][lamp_coordinate[temp1 - 1].y].type == lightOff) {
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
        printf("WRONG INPUT \a");
        scanf("%d", &temp1);
    }
    int x_lamp = lamp_coordinate[temp1 - 1].x;
    int y_lamp = lamp_coordinate[temp1 - 1].y;
    map[x_lamp][y_lamp].type = lightOff;
    system("cls");
    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE);
    draw_map();
    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);

    printf("ENTER NUMBER OF LAMP YOU WANT TO TURN ON: ");
    int temp2;
    scanf("%d", &temp2);
    //trying to turn on an on lamp or turn on the lamp that was turned off now
    while (map[lamp_coordinate[temp2 - 1].x][lamp_coordinate[temp2 - 1].y].type == lightOn || temp1 == temp2) {
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
        printf("WRONG INPUT \a");
        scanf("%d", &temp2);
    }
    x_lamp = lamp_coordinate[temp2 - 1].x;
    y_lamp = lamp_coordinate[temp2 - 1].y;
    map[x_lamp][y_lamp].type = lightOn;
    system("cls");
    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE);
    draw_map();
}

//show one of the suspected cards and delete that from list
void action_SH(int turn, int round, int *num_cards) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
    if ((round % 2 == 1 && (turn == 1 || turn == 4)) || (round % 2 == 0 && (turn == 2 || turn == 3)))
        printf("Mr.Jack MUST NOT SEE THIS MESSAGE\n");
    else if ((round % 2 == 0 && (turn == 1 || turn == 4)) || (round % 2 == 1 && (turn == 2 || turn == 3)))
        printf("DETECTIVE MUST NOT SEE THIS MESSAGE\n");
    sleep(2);
    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
    //choose first node of shuffled list
    printf("THIS CARD IS:%s\n", suspected->name);
    sleep(4);
    system("cls");
    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE);
    draw_map();
    delete_node_suspected(&suspected, suspected->name, num_cards);
}

//open a hole and close one
void action_JB() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
    printf("ENTER NUMBER OF HOLE YOU WANT TO BLOCK: ");
    int temp1;
    scanf("%d", &temp1);
    //trying to block a blocked hole
    while (map[hole_coordinate[temp1 - 1].x][hole_coordinate[temp1 - 1].y].type == blockedHole) {
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
        printf("WRONG INPUT \a");
        scanf("%d", &temp1);
    }
    int x_hole = hole_coordinate[temp1 - 1].x;
    int y_hole = hole_coordinate[temp1 - 1].y;
    map[x_hole][y_hole].type = blockedHole;
    system("cls");
    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE);
    draw_map();
    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
    printf("ENTER NUMBER OF HOLE YOU WANT TO OPEN: ");
    int temp2;
    scanf("%d", &temp2);
    //trying to open an open hole or open the hole that was blocked now
    while (map[hole_coordinate[temp2 - 1].x][hole_coordinate[temp2 - 1].y].type == openHole || temp1 == temp2) {
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
        printf("WRONG INPUT \a");
        scanf("%d", &temp2);
    }
    x_hole = hole_coordinate[temp2 - 1].x;
    y_hole = hole_coordinate[temp2 - 1].y;
    map[x_hole][y_hole].type = openHole;
    system("cls");
    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE);
    draw_map();
}

//find distance between destination of a character with SG and origin of the character with SG and compare
int is_correct_SG(int x_sg, int y_sg, int x_first, int y_first, int x_end, int y_end) {
    int delta_x_first = (int) pow((x_first - x_sg), 2);
    int delta_y_first = (int) pow((y_first - y_sg), 2);
    int delta_x_end = (int) pow((x_end - x_sg), 2);
    int delta_y_end = (int) pow((y_end - y_sg), 2);
    if ((delta_x_end + delta_y_end) < (delta_x_first + delta_y_first))
        return 1;
    return 0;
}

//like move func
void move_action_sg(int n, int temp, int *x, int *y, int character, int x_first, int y_first) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    int go_hole = 0;
    for (int i = 0; i < n; ++i) {
        if (i != n && map[*x][*y].type == openHole && go_hole == 0) {
            go_hole = move_hole(x, y, i, n, character);
            if (go_hole)
                continue;
        }
        int direction;
        SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
        printf("IN WHICH DIRECTION DO U WANT TO MOVE?\n1 NORTH, 2 NORTH-EAST, 3 SOUTH-EAST, 4 SOUTH, 5 SOUTH-WEST, 6 NORTH-WEST\n");
        scanf("%d", &direction);
        if (direction == North) {
            if (is_possible(x_first, y_first, *x, *y + 1, character, n, i, -1, 1)) {
                set_character(*x, *y, "   ");
                (*y)++;
                draw_move(*x, *y, character, temp);
            } else
                i--;
        } else if (direction == NorthEast) {
            if (*x % 2 == 0) {
                if (is_possible(x_first, y_first, *x + 1, *y, character, n, i, -1, 1)) {
                    set_character(*x, *y, "   ");
                    (*x)++;
                    draw_move(*x, *y, character, temp);
                } else
                    i--;
            } else {
                if (is_possible(x_first, y_first, *x + 1, *y + 1, character, n, i, -1, 1)) {
                    set_character(*x, *y, "   ");
                    (*x)++;
                    (*y)++;
                    draw_move(*x, *y, character, temp);
                } else
                    i--;
            }
        } else if (direction == SouthEast) {
            if (*x % 2 == 0) {
                if (is_possible(x_first, y_first, *x + 1, *y - 1, character, n, i, -1, 1)) {
                    set_character(*x, *y, "   ");
                    (*x)++;
                    (*y)--;
                    draw_move(*x, *y, character, temp);
                } else
                    i--;
            } else {
                if (is_possible(x_first, y_first, *x + 1, *y + 1, character, n, i, -1, 1)) {
                    set_character(*x, *y, "   ");
                    (*x)++;
                    draw_move(*x, *y, character, temp);
                } else
                    i--;
            }
        } else if (direction == South) {
            if (is_possible(x_first, y_first, *x, *y - 1, character, n, i, -1, 1)) {
                set_character(*x, *y, "   ");
                (*y)--;
                draw_move(*x, *y, character, temp);
            } else
                i--;
        } else if (direction == SouthWest) {
            if (*x % 2 == 0) {
                if (is_possible(x_first, y_first, *x - 1, *y - 1, character, n, i, -1, 1)) {
                    set_character(*x, *y, "   ");
                    (*x)--;
                    (*y)--;
                    draw_move(*x, *y, character, temp);
                } else
                    i--;
            } else {
                if (is_possible(x_first, y_first, *x - 1, *y, character, n, i, -1, 1)) {
                    set_character(*x, *y, "   ");
                    (*x)--;
                    draw_move(*x, *y, character, temp);
                } else
                    i--;
            }
        } else if (direction == NorthWest) {
            if (*x % 2 == 0) {
                if (is_possible(x_first, y_first, *x - 1, *y, character, n, i, -1, 1)) {
                    set_character(*x, *y, "   ");
                    (*x)--;
                    draw_move(*x, *y, character, temp);
                } else
                    i--;
            } else {
                if (is_possible(x_first, y_first, *x - 1, *y + 1, character, n, i, -1, 1)) {
                    set_character(*x, *y, "   ");
                    (*x)--;
                    (*y)++;
                    draw_move(*x, *y, character, temp);
                } else
                    i--;
            }
        }
    }
}

//choose some characters and move them
void action_SG() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    int sum = 0;
    do {
        char *name = (char *) malloc(4 * sizeof(char));
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE);
        printf("ENTER THE NAME OF CHARACTER U WANT TO BE CLOSER TO U ");
        scanf("%s", name);
        int character = num_character(name);
        int x = coordinate[character].x;
        int y = coordinate[character].y;
        //keep coordinate of origin of character
        int x_first = x, y_first = y;
        int x_sg = coordinate[SG].x;
        int y_sg = coordinate[SG].y;
        getchar();
        int n;
        SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
        printf("HOW MANY HOMES DO U WANT TO MOVE %s ? ", name);
        scanf("%d", &n);
        while (n > 3 || n < 1) {
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
            printf("WRONG INPUT!U HAVE JUST 3 MOVES! TRY AGAIN \a");
            scanf("%d", &n);
        }
        sum += n;
        //sum of moves of characters must not be more than 3
        while (sum > 3) {
            sum -= n;
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
            printf("WRONG INPUT!U HAVE JUST 3 MOVES! TRY AGAIN \a");
            scanf("%d", &n);
            sum += n;
        }
        int temp = check(map[x][y].character);
        //move character
        move_action_sg(n, temp, &x, &y, character, x_first, y_first);
        //while player was getting character far from Sg show error and get input again
        while (!is_correct_SG(x_sg, y_sg, x_first, y_first, x, y)) {
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
            printf("U DID NOT GET %s CLOSER TO SG! TRY AGAIN \a", name);
            sleep(4);
            set_character(x, y, "   ");
            draw_move(x_first, y_first, character, temp);
            x = x_first;
            y = y_first;
            sum -= n;
            SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
            printf("HOW MANY HOMES DO U WANT TO MOVE %s ? ", name);
            scanf("%d", &n);
            while (n > 3 || n < 1) {
                SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
                printf("WRONG INPUT!U HAVE JUST 3 MOVES! TRY AGAIN \a");
                scanf("%d", &n);
            }
            sum += n;
            //sum of moves of characters must not be more than 3
            while (sum > 3) {
                sum -= n;
                SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
                printf("WRONG INPUT!U HAVE JUST 3 MOVES! TRY AGAIN \a");
                scanf("%d", &n);
                sum += n;
            }
            move_action_sg(n, temp, &x, &y, character, x_first, y_first);
        }
        if (sum == 3)
            break;
    } while (1);
}