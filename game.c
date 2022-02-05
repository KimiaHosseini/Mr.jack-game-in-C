//
// Created by Lenovo on 1/25/2022.
//
#include <stdio.h>
#include <stdlib.h>
#include "action.h"
#include "cards.h"
#include <windows.h>
#include <time.h>
#include <unistd.h>

FILE *information;
FILE *suspected_cards;
FILE *play;
FILE *type_homes;
FILE *mapchars;
FILE *char_coordinate;
FILE *lampcoordinate;
FILE *holecoordinate;
FILE *description;

struct info {
    int jack;
    int num_card;
    int num_round;
};
struct info info;

void save() {
    printf("                                          "
           "                         IF U WANT TO CONTINUE THE GAME ,ENTER 1 ELSE IF U WANT TO SAVE THE GAME AND EXIT,ENTER 2");
    int save;
    scanf("%d", &save);
    if (save == 2) {
        //write character of jack , number of suspected cards , number of round and
        //information of action of JW(x,y,way)
        // (it will be used for determining on or off characters )
        information = fopen("C:\\Users\\Lenovo\\Desktop\\project1\\jack.txt", "wb");
        fwrite(&info.jack, sizeof(int), 1, information);
        fwrite(&info.num_card, sizeof(int), 1, information);
        fwrite(&info.num_round, sizeof(int), 1, information);
        fwrite(&flashlight, sizeof(flashlight), 1, information);
        fclose(information);
        /////////////////////////
        //save suspected cards
        struct card *temp1 = suspected;
        suspected_cards = fopen("C:\\Users\\Lenovo\\Desktop\\project1\\suspect.txt", "wb");
        char *s = (char *) malloc(3 * sizeof(char));
        while (temp1 != NULL) {
            strcpy(s, temp1->name);
            fwrite(s, sizeof(s), 1, suspected_cards);
            temp1 = temp1->next;
        }
        fclose(suspected_cards);
        ////////////////////////////////////////
        //if we are in even round there is no need to save play cards
        if (info.num_card % 2 == 1) {
            struct card *temp2 = play_cards;
            play = fopen("C:\\Users\\Lenovo\\Desktop\\project1\\playcard.txt", "wb");
            char *s1 = (char *) malloc(3 * sizeof(char));
            while (temp2 != NULL) {
                strcpy(s1, temp2->name);
                fwrite(s1, sizeof(s1), 1, play);
                temp2 = temp2->next;
            }
            fclose(play);
        }
        ////////////////////////////
        //if we are in even round there is no need to save play cards
        if (info.num_card % 2 == 1) {
            struct card *temp3 = play_cards;
            description = fopen("C:\\Users\\Lenovo\\Desktop\\project1\\description.txt", "wb");
            while (temp3 != NULL) {
                fwrite(temp3->description, sizeof(temp3->description), 1, description);
                temp3 = temp3->next;
            }
            fclose(description);
        }
        //////////////////////////////////////////
        //save type of each home
        type_homes = fopen("C:\\Users\\Lenovo\\Desktop\\project1\\typehomes.txt", "wb");
        for (int i = 0; i < 13; ++i) {
            for (int j = 0; j < 9; ++j) {
                fwrite(&map[i][j].type, sizeof(int), 1, type_homes);
            }
        }
        fclose(type_homes);
        ///////////////////////////////////
        //save character of each home
        mapchars = fopen("C:\\Users\\Lenovo\\Desktop\\project1\\mapchars.txt", "wb");
        for (int i = 0; i < 13; ++i) {
            for (int j = 0; j < 9; ++j) {
                fwrite(&map[i][j].character, sizeof(map[i][j].character), 1, mapchars);
            }
        }
        fclose(mapchars);
        ////////////////////
        //save coordinate of each character
        char_coordinate = fopen("C:\\Users\\Lenovo\\Desktop\\project1\\coordinate.txt", "wb");
        for (int i = 0; i < 8; ++i) {
            fwrite(&coordinate[i], sizeof(coordinate[0]), 1, char_coordinate);
        }
        fclose(char_coordinate);
        ///////////////////////
        //save coordinate of each lamp
        lampcoordinate = fopen("C:\\Users\\Lenovo\\Desktop\\project1\\lampcoordinate.txt", "wb");
        for (int i = 0; i < 8; ++i) {
            fwrite(&lamp_coordinate[i], sizeof(lamp_coordinate[0]), 1, lampcoordinate);
        }
        fclose(lampcoordinate);
        ////////////////////
        //save coordinate of each hole
        holecoordinate = fopen("C:\\Users\\Lenovo\\Desktop\\project1\\holecoordinate.txt", "wb");
        for (int i = 0; i < 8; ++i) {
            fwrite(&hole_coordinate[i], sizeof(hole_coordinate[0]), 1, holecoordinate);
        }
        fclose(holecoordinate);
        exit(0);
    } else
        return;
}

struct info load() {
    //load character of jack , number of suspected cards , number of round and
    //information about action of JW(x,y,way)
    // (it will be used for determining on or off characters )
    information = fopen("C:\\Users\\Lenovo\\Desktop\\project1\\jack.txt", "rb");
    fread(&info.jack, sizeof(int), 1, information);
    fread(&info.num_card, sizeof(int), 1, information);
    fread(&info.num_round, sizeof(int), 1, information);
    fread(&flashlight, sizeof(flashlight), 1, information);
    fclose(information);
    ////////////////////////////////
    //load type of each home
    type_homes = fopen("C:\\Users\\Lenovo\\Desktop\\project1\\typehomes.txt", "rb");
    for (int i = 0; i < 13; ++i) {
        for (int j = 0; j < 9; ++j) {
            fread(&map[i][j].type, sizeof(int), 1, type_homes);
        }
    }
    fclose(type_homes);
    /////////////////////////////
    //load character of each home
    mapchars = fopen("C:\\Users\\Lenovo\\Desktop\\project1\\mapchars.txt", "rb");
    for (int i = 0; i < 13; ++i) {
        for (int j = 0; j < 9; ++j) {
            fread(&map[i][j].character, sizeof(map[i][j].character), 1, mapchars);
        }
    }
    fclose(mapchars);
    ////////////////////
    //load coordinate of each character
    char_coordinate = fopen("C:\\Users\\Lenovo\\Desktop\\project1\\coordinate.txt", "rb");
    for (int i = 0; i < 8; ++i) {
        fread(&coordinate[i], sizeof(coordinate[0]), 1, char_coordinate);
    }
    fclose(char_coordinate);
    ///////////////////////
    //load coordinate of each lamp
    lampcoordinate = fopen("C:\\Users\\Lenovo\\Desktop\\project1\\lampcoordinate.txt", "rb");
    for (int i = 0; i < 8; ++i) {
        fread(&lamp_coordinate[i], sizeof(lamp_coordinate[0]), 1, lampcoordinate);
    }
    fclose(lampcoordinate);
    ////////////////////
    //load coordinate of each hole
    holecoordinate = fopen("C:\\Users\\Lenovo\\Desktop\\project1\\holecoordinate.txt", "rb");
    for (int i = 0; i < 8; ++i) {
        fread(&hole_coordinate[i], sizeof(hole_coordinate[0]), 1, holecoordinate);
    }
    fclose(holecoordinate);

    return info;
}

void load_suspected_card(int num_cards) {
    //load suspected cards
    suspected_cards = fopen("C:\\Users\\Lenovo\\Desktop\\project1\\suspect.txt", "rb");
    suspected = NULL;
    char *s = (char *) malloc(3 * sizeof(char));
    for (int i = 0; i < num_cards; ++i) {
        fread(s, sizeof(s), 1, suspected_cards);
        make_suspected_cards2(&suspected, s);
    }
    fclose(suspected_cards);
}

void load_play_cards() {
    //load play cards
    play = fopen("C:\\Users\\Lenovo\\Desktop\\project1\\playcard.txt", "rb");
    description = fopen("C:\\Users\\Lenovo\\Desktop\\project1\\description.txt", "rb");
    char *s1 = (char *) malloc(3 * sizeof(char));
    char s3[100];
    play_cards = NULL;
    for (int i = 0; i < 8; ++i) {
        fread(s1, sizeof(s1), 1, play);
        fread(s3, sizeof(s3), 1, description);
        make_play_cards2(&play_cards, s1, s3);
    }
    fclose(play);
}

struct info new() {
    //set default features of game
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, BACKGROUND_BLUE | FOREGROUND_RED |FOREGROUND_BLUE|FOREGROUND_GREEN);
    //some rules of game
    system("cls");
    printf("HOMES WITH '******' TYPE ARE EXTRA HOMES SO IMPOSSIBLE TO PASS THEM\n"
           "HOMES WITH 'HOUSE' TYPE ARE BUILDINGS IN THE MAP\n"
           "OPEN EXIT WAYS ARE MARKED WITH 'EXIT'\nAND BLOCKED EXIT WAYS WITH '******'\n"
           "IN THE MAP LIGHTS WITH '+' MEANS THAT THIS LIGHT IS ON\n"
           "LIGHTS WITH '-' MEANS THAT LIGHT IS OFF\nHOLES WITH '+' MARK MEANS THAT HOLE IS OPEN\n"
           "HOLES WITH '-' MEANS THAT HOLE IS BLOCKED\n"
           "CHARACTERS WHICH HAS '#' ARE ""NOT SUSPECTED"" ANYMORE FOR DETECTIVE\n");
    printf("WHEN U MOVE OR DO STH WRONG U WILL HERE AN ERROR SOUND\n"
           "AND WHEN U MOVE ON THE MAP CORRECTLY U WILL HERE A SOUND LIKE FOOTSTEPS!\n");
    int temp;
    //ask from players that who will be jack
    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
    printf("Who will be Mr.Jack?\n1)PLAYER1\n2)PLAYER2\n");
    scanf("%d", &temp);
    ///////////////////////////////////
    //make suspected cards
    suspected = NULL;
    make_suspected_cards(&suspected, "SH");
    make_suspected_cards(&suspected, "JW");
    make_suspected_cards(&suspected, "JS");
    make_suspected_cards(&suspected, "IL");
    make_suspected_cards(&suspected, "MS");
    make_suspected_cards(&suspected, "SG");
    make_suspected_cards(&suspected, "WG");
    make_suspected_cards(&suspected, "JB");
    //make play cards
    set_play_cards();

    /*shuffle suspected cards
     * 5 times generate two random numbers n1,n2
     * and swap n1th node with n2th node of list*/
    time_t t = time(NULL);
    srand(t);
    int n1, n2;
    for (int i = 0; i < 5; ++i) {
        n1 = rand() % 8;
        n2 = rand() % 8;
        swap_node_suspected(&suspected, n1, n2);
    }

    SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
    if (temp == 1)
        //if player 1 chose to be Jack
        printf("\nPLAYER 2(detective) MUST NOT SEE THIS MESSAGE\n");
    else
        //if player 2 chose to be Jack
        printf("\nPLAYER 1(detective) MUST NOT SEE THIS MESSAGE\n");
    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
    sleep(1);
    //the first card of suspected cards after shuffling is Mr.Jack
    printf("\nMr.JACK'S CHARACTER IS %s\n", suspected->name);
    sleep(3);

    info.jack = num_character(suspected->name);
    info.num_card = 8;
    info.num_round = 0;

    //delete card of character of Jack from suspected cards
    delete_node_suspected(&suspected, suspected->name, &info.num_card);

    //drawing map:
    //read the text file (type) to set types of each home based on default map
    FILE *type = fopen("C:\\Users\\Lenovo\\Desktop\\project1\\type.txt", "r");
    type_map(type);

    //set the characters in all houses to "   "
    for (int i = 0; i < 13; ++i) {
        for (int j = 0; j < 9; ++j) {
            set_character(i, j, "   ");
            map[i][j].character[3] = '\0';
        }
    }

    //set characters based on default map
    set_character(0, 4, "JW ");
    set_character(4, 8, "WG ");
    set_character(4, 4, "IL ");
    set_character(6, 6, "JS ");
    set_character(6, 3, "SH ");
    set_character(8, 1, "MS ");
    set_character(8, 5, "JB ");
    set_character(12, 5, "SG ");

    //set coordinate of our 8 lamps based on default map
    lamp_coordinate[0].x = 5;
    lamp_coordinate[0].y = 7;
    lamp_coordinate[1].x = 2;
    lamp_coordinate[1].y = 7;
    lamp_coordinate[2].x = 11;
    lamp_coordinate[2].y = 6;
    lamp_coordinate[3].x = 7;
    lamp_coordinate[3].y = 5;
    lamp_coordinate[4].x = 5;
    lamp_coordinate[4].y = 3;
    lamp_coordinate[5].x = 1;
    lamp_coordinate[5].y = 2;
    lamp_coordinate[6].x = 10;
    lamp_coordinate[6].y = 2;
    lamp_coordinate[7].x = 7;
    lamp_coordinate[7].y = 1;

    //set coordinate of our 8 holes based on default map
    hole_coordinate[0].x = 5;
    hole_coordinate[0].y = 8;
    hole_coordinate[1].x = 11;
    hole_coordinate[1].y = 7;
    hole_coordinate[2].x = 0;
    hole_coordinate[2].y = 6;
    hole_coordinate[3].x = 5;
    hole_coordinate[3].y = 5;
    hole_coordinate[4].x = 12;
    hole_coordinate[4].y = 4;
    hole_coordinate[5].x = 7;
    hole_coordinate[5].y = 3;
    hole_coordinate[6].x = 2;
    hole_coordinate[6].y = 2;
    hole_coordinate[7].x = 7;
    hole_coordinate[7].y = 0;

    //set coordinate of our 8 characters based on default map
    coordinate[0].x = 0;
    coordinate[0].y = 4;
    coordinate[1].x = 4;
    coordinate[1].y = 8;
    coordinate[2].x = 4;
    coordinate[2].y = 4;
    coordinate[3].x = 6;
    coordinate[3].y = 6;
    coordinate[4].x = 6;
    coordinate[4].y = 3;
    coordinate[5].x = 8;
    coordinate[5].y = 1;
    coordinate[6].x = 8;
    coordinate[6].y = 5;
    coordinate[7].x = 12;
    coordinate[7].y = 5;

    return info;
}