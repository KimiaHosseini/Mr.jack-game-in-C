#include <stdio.h>
#include <windows.h>
#include <unistd.h>
#include "action.h"
#include "cards.h"
#include "round.h"

//in phase 1: move and do actions of characters
void step1(char x[3], int turn, int round, int jack, int arrest, int *num_cards) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (strncmp(x, "JW", 2) == 0) {
        move(JW, jack, round, arrest);
        action_JW();
    } else if (strncmp(x, "WG", 2) == 0) {
        SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_RED);
        printf("IF U WANT TO MOVE ENTER 1 ELSE IF U WANT TO CHANGE YOUR PLACE WITH ANOTHER CHARACTER ENTER 2\n");
        int act;
        scanf("%d", &act);
        getchar();
        if (act == 1)
            move(WG, jack, round, arrest);
        else
            action_WG();
    } else if (strncmp(x, "IL", 2) == 0) {
        move(IL, jack, round, arrest);
        action_IL();
    } else if (strncmp(x, "JS", 2) == 0) {
        SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_RED);
        printf("IF U WANT TO FIRST MOVE THEN DO YOUR ACTION ENTER 1 ELSE ENTER 2\n");
        int temp1;
        scanf("%d", &temp1);
        if (temp1 == 1) {
            move(JS, jack, round, arrest);
            action_JS();
        } else {
            action_JS();
            move(JS, jack, round, arrest);
        }
    } else if (strncmp(x, "SH", 2) == 0) {
        move(SH, jack, round, arrest);
        action_SH(turn, round, num_cards);
    } else if (strncmp(x, "MS", 2) == 0)
        move(MS, jack, round, arrest);
    else if (strncmp(x, "JB", 2) == 0) {
        SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_RED);
        printf("IF U WANT TO FIRST MOVE THEN DO YOUR ACTION ENTER 1 ELSE ENTER 2\n");
        int temp1;
        scanf("%d", &temp1);
        if (temp1 == 1) {
            move(JB, jack, round, arrest);
            action_JB();
        } else {
            action_JB();
            move(JB, jack, round, arrest);
        }
    } else if (strncmp(x, "SG", 2) == 0) {
        SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_RED);
        printf("IF U WANT TO FIRST MOVE THEN DO YOUR ACTION ENTER 1 ELSE ENTER 2\n");
        int temp1;
        scanf("%d", &temp1);
        if (temp1 == 1) {
            move(SG, jack, round, arrest);
            action_SG();
        } else {
            action_SG();
            move(SG, jack, round, arrest);
        }
    }
}


void round_odd(int round, int jack, int *num_cards, struct card *play_cards_odd) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
    printf("DETECTIVE CHOOSE ONE CARD & PLAY!\n");
    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
    print_play_cards(play_cards_odd);
    char x[3];
    scanf("%s", x);
    int arrest = 0;
    if (round != 1) {
        printf("IF U WANT TO ARREST MR.JACK ENTER 1 ELSE ENTER 0\n");
        getchar();
        scanf("%d", &arrest);
        while (arrest != 1 && arrest != 0) {
            printf("wrong input");
            scanf("%d", &arrest);
        }
    }
    delete_node(&play_cards_odd, x);
    int temp = 1;
    step1(x, temp, round, jack, arrest, num_cards);
    for (int i = 0; i < 2; ++i) {
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
        printf("MR.JACK CHOOSE ONE CARD & PLAY!\n");
        SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
        print_play_cards(play_cards_odd);
        getchar();
        scanf("%s", x);
        delete_node(&play_cards_odd, x);
        temp++;
        step1(x, temp, round, jack, 0, num_cards);
    }
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
    printf("DETECTIVE CHOOSE ONE CARD & PLAY!\n");
    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
    print_play_cards(play_cards_odd);
    getchar();
    scanf("%s", x);
    printf("IF U WANT TO ARREST MR.JACK ENTER 1 ELSE ENTER 0\n");
    getchar();
    scanf("%d", &arrest);
    while (arrest != 1 && arrest != 0) {
        printf("wrong input");
        scanf("%d", &arrest);
    }
    delete_node(&play_cards_odd, x);
    temp++;
    step1(x, temp, round, jack, arrest, num_cards);
}

void round_even(int round, int jack, int *num_cards, struct card *play_cards_even) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
    printf("MR.JACK CHOOSE ONE CARD & PLAY!\n");
    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
    int temp;
    print_play_cards(play_cards_even);
    char x[3];
    scanf("%s", x);
    delete_node(&play_cards_even, x);
    temp = 1;
    step1(x, temp, round, jack, 0, num_cards);
    for (int i = 0; i < 2; ++i) {
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
        printf("DETECTIVE CHOOSE ONE CARD & PLAY!\n");
        SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
        print_play_cards(play_cards_even);
        getchar();
        scanf("%s", x);
        printf("IF U WANT TO ARREST MR.JACK ENTER 1 ELSE ENTER 0\n");
        getchar();
        int arrest;
        scanf("%d", &arrest);
        while (arrest != 1 && arrest != 0) {
            printf("wrong input");
            scanf("%d", &arrest);
        }
        delete_node(&play_cards_even, x);
        temp++;
        step1(x, temp, round, jack, arrest, num_cards);
    }
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
    printf("MR.JACK CHOOSE ONE CARD & PLAY!\n");
    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
    print_play_cards(play_cards_even);
    getchar();
    scanf("%s", x);
    delete_node(&play_cards_even, x);
    temp++;
    step1(x, temp, round, jack, 0, num_cards);
    getchar();
}

int is_near_lamp(int character) {
    int x = coordinate[character].x;
    int y = coordinate[character].y;
    if (x % 2 == 0) {
        if (x == 0) {
            //we should not check northwest and southwest
            if (map[x][y + 1].type == lightOn || map[x + 1][y].type == lightOn ||
                map[x + 1][y - 1].type == lightOn || map[x][y - 1].type == lightOn)
                return 1;
            else
                return 0;
        } else if (x == 12) {
            //we should not check northeast and southeast
            if (map[x][y + 1].type == lightOn || map[x][y - 1].type == lightOn
                || map[x - 1][y - 1].type == lightOn || map[x - 1][y].type == lightOn)
                return 1;
            else
                return 0;
        } else if (y == 8) {
            //we should not check north
            if (map[x + 1][y].type == lightOn || map[x + 1][y - 1].type == lightOn
                || map[x][y - 1].type == lightOn || map[x - 1][y - 1].type == lightOn
                || map[x - 1][y].type == lightOn)
                return 1;
            else
                return 0;
        } else {
            if (map[x][y + 1].type == lightOn || map[x + 1][y].type == lightOn ||
                map[x + 1][y - 1].type == lightOn || map[x][y - 1].type == lightOn
                || map[x - 1][y - 1].type == lightOn || map[x - 1][y].type == lightOn)
                return 1;
            else
                return 0;
        }
    } else {
        if (y == 0) {
            //we should not check south
            if (map[x][y + 1].type == lightOn || map[x + 1][y + 1].type == lightOn
                || map[x + 1][y].type == lightOn || map[x - 1][y].type == lightOn
                || map[x - 1][y + 1].type == lightOn)
                return 1;
            else
                return 0;
        } else {
            if (map[x][y + 1].type == lightOn || map[x + 1][y + 1].type == lightOn
                || map[x + 1][y].type == lightOn || map[x][y - 1].type == lightOn
                || map[x - 1][y].type == lightOn || map[x - 1][y + 1].type == lightOn)
                return 1;
            else
                return 0;
        }
    }
}

int is_near_character(int character) {
    int x = coordinate[character].x;
    int y = coordinate[character].y;
    if (x % 2 == 0) {
        if (x == 0) {
            //we should not check northwest and southwest
            if (strcmp(map[x][y + 1].character, "   ") != 0 || strcmp(map[x + 1][y].character, "   ") != 0 ||
                strcmp(map[x + 1][y - 1].character, "   ") != 0 || strcmp(map[x][y - 1].character, "   ") != 0)
                return 1;
            else
                return 0;
        } else if (x == 12) {
            //we should not check northeast and southeast
            if (strcmp(map[x][y + 1].character, "   ") != 0 || strcmp(map[x][y - 1].character, "   ") != 0
                || strcmp(map[x - 1][y - 1].character, "   ") != 0 || strcmp(map[x - 1][y].character, "   ") != 0)
                return 1;
            else
                return 0;
        } else if (y == 8) {
            //we should not check north
            if (strcmp(map[x + 1][y].character, "   ") != 0 || strcmp(map[x + 1][y - 1].character, "   ") != 0
                || strcmp(map[x][y - 1].character, "   ") != 0 || strcmp(map[x - 1][y - 1].character, "   ") != 0
                || strcmp(map[x - 1][y].character, "   ") != 0)
                return 1;
            else
                return 0;
        } else {
            if (strcmp(map[x][y + 1].character, "   ") != 0 || strcmp(map[x + 1][y].character, "   ") != 0 ||
                strcmp(map[x + 1][y - 1].character, "   ") != 0 || strcmp(map[x][y - 1].character, "   ") != 0
                || strcmp(map[x - 1][y - 1].character, "   ") != 0 || strcmp(map[x - 1][y].character, "   ") != 0)
                return 1;
            else
                return 0;
        }
    } else {
        if (y == 0) {
            //we should not check south
            if (strcmp(map[x][y + 1].character, "   ") != 0 || strcmp(map[x + 1][y + 1].character, "   ") != 0
                || strcmp(map[x + 1][y].character, "   ") != 0 || strcmp(map[x - 1][y].character, "   ") != 0
                || strcmp(map[x - 1][y + 1].character, "   ") != 0)
                return 1;
            else
                return 0;
        } else {
            if (strcmp(map[x][y + 1].character, "   ") != 0 || strcmp(map[x + 1][y + 1].character, "   ") != 0
                || strcmp(map[x + 1][y].character, "   ") != 0 || strcmp(map[x][y - 1].character, "   ") != 0
                || strcmp(map[x - 1][y].character, "   ") != 0 || strcmp(map[x - 1][y + 1].character, "   ") != 0)
                return 1;
            else
                return 0;
        }
    }
}

//check that a character is visible by flashlight of JW
int flashlight_JW(int character) {
    int x = flashlight.x;
    int y = flashlight.y;
    int way = flashlight.way;
    int on = 0;
    if (way == North) {
        do {
            y++;
            if (y > 8 || map[x][y].type == house || map[x][y].type == extra)
                break;
            if (strncmp(map[x][y].character, name(character), 2) == 0) {
                on = 1;
                break;
            }
        } while (1);
        if (on)
            return 1;
        return 0;
    } else if (way == NorthEast) {
        do {
            if (x % 2 == 1)
                y++;
            x++;
            if (y > 8 || x > 12 || map[x][y].type == house || map[x][y].type == extra)
                break;
            if (strncmp(map[x][y].character, name(character), 2) == 0) {
                on = 1;
                break;
            }
        } while (1);
        if (on)
            return 1;
        return 0;
    } else if (way == SouthEast) {
        do {
            if (x % 2 == 0)
                y--;
            x++;
            if (y < 0 || x > 12 || map[x][y].type == house || map[x][y].type == extra)
                break;
            if (strncmp(map[x][y].character, name(character), 2) == 0) {
                on = 1;
                break;
            }
        } while (1);
        if (on)
            return 1;
        return 0;
    } else if (way == South) {
        do {
            y--;
            if (y < 0 || map[x][y].type == house || map[x][y].type == extra)
                break;
            if (strncmp(map[x][y].character, name(character), 2) == 0) {
                on = 1;
                break;
            }
        } while (1);
        if (on)
            return 1;
        return 0;
    } else if (way == SouthWest) {
        do {
            if (x % 2 == 0)
                y--;
            x--;
            if (y < 0 || x < 0 || map[x][y].type == house || map[x][y].type == extra)
                break;
            if (strncmp(map[x][y].character, name(character), 2) == 0) {
                on = 1;
                break;
            }
        } while (1);
        if (on)
            return 1;
        return 0;
    } else if (way == NorthWest) {
        do {
            if (x % 2 == 1)
                y++;
            x++;
            if (y > 8 || x < 0 || map[x][y].type == house || map[x][y].type == extra)
                break;
            if (strncmp(map[x][y].character, name(character), 2) == 0) {
                on = 1;
                break;
            }
        } while (1);
        if (on)
            return 1;
        return 0;
    }
}

//save visibility of jack in every round
int visible_jack[8];

//in phase 2: mark characters that are not suspected
void step2(int jack, int round) {
    int on;
    if (is_near_character(jack) == 1 || is_near_lamp(jack) == 1 || flashlight_JW(jack) == 1)
        on = 1;
    else
        on = 0;
    visible_jack[round - 1] = on;
    if (on == 1) {
        //if jack is visible
        //check our 8 characters and add # to the end of the name of invisible characters
        for (int i = JW; i <= SG; ++i) {
            int x = coordinate[i].x;
            int y = coordinate[i].y;
            if (flashlight_JW(i) == 0 && is_near_lamp(i) == 0 && is_near_character(i) == 0)
                map[x][y].character[2] = '#';
        }
    } else {
        //if jack is invisible
        //check our 8 characters and add # to the end of the name of visible characters
        for (int i = JW; i <= SG; ++i) {
            int x = coordinate[i].x;
            int y = coordinate[i].y;
            if (flashlight_JW(i) == 1 || is_near_lamp(i) == 1 || is_near_character(i) == 1)
                map[x][y].character[2] = '#';
        }
    }
    system("cls");
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN);
    system("cls");
    draw_map();
    sleep(5);
}