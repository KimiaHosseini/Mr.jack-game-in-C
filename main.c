 /*Kimia Hosseini
 * 40031014
*/

#include <stdio.h>
#include "map.h"
#include "cards.h"
#include "game.h"
#include <windows.h>
#include <unistd.h>
#include "round.h"

///types of homes:
/*0 : simple
 * 1:house
 * 2:on light
 * 3:off light
 * 4:open hole
 * 5:blocked hole
 * 6:extra place
 * 7:open exit way
 * 8:blocked exit way*/
///characters
/* 0 JW : 1)1to3 move -> 2) choose a place to light its way
 * 1 WG : 1to3 move or replace its place with another one
 * 2 IL : 1)1to3 move  -> 2) close an exit way
 * 3 JS : 1to3 move  <-> turn a light on and turn a light off
 * 4 SH : 1)1to3 move  -> 2) see a card
 * 5 MS : 1to4 move : also can pass all buildings
 * 6 JB : 1to3 move  <-> close a hole
 * 7 SG : 1to3 move  <-> 3 move for other characters
 */

///////////////////////////////////////
int main() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
    //start a new game or load the last game
    printf("IF U WANT TO START A NEW GAME ENTER 1 ELSE IF U WANT TO LOAD YOUR LAST GAME ENTER 2 ");
    int start;
    scanf("%d", &start);
    if (start == 2) {
        //load last round
        info = load();
        load_play_cards();
        load_suspected_card(info.num_card);
    } else if (start == 1) {
        //load default features
        info = new();
    }

    struct card *play_cards_odd;
    struct card *play_cards_even;

    for (int i = info.num_round + 1; i <= 8; ++i) {
        //odd rounds
        if (i % 2 == 1) {
            //shuffle play_cards
            play_cards = shuffle_play_cards();
            //make odd_cards
            play_cards_odd = odd();
            system("cls");
            system("color 07");
            SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
            printf("                                                       ROUND %d", i);
            SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
            draw_map();
            //play a round
            round_odd(i, info.jack, &info.num_card, play_cards_odd);
            printf("characters with # are not suspected:");
            sleep(2);
            system("cls");
            //mark characters that we are sure they can not be mr.jack with #
            step2(info.jack, i);

            //step 3(this part is not in project description)
            if (i == 1)
                //in round 1 : turn light 6 off
                map[lamp_coordinate[5].x][lamp_coordinate[5].y].type = 3;
            else if (i == 3)
                //in round 3 : turn light 2 off
                map[lamp_coordinate[1].x][lamp_coordinate[1].y].type = 3;
            system("cls");
            draw_map();
            sleep(3);
            system("cls");
            system("color 75");
            printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n                                                         "
                   "                                          ROUND %d FINISHED\n", i);
            info.num_round = i;
            save();
            sleep(2);
        } else {
            //set rest of play cards as even play cards
            play_cards_even = even();
            system("cls");
            system("color 07");
            SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
            printf("                                                      ROUND %d", i);
            SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
            draw_map();
            //play a round
            round_even(i, info.jack, &info.num_card, play_cards_even);
            printf("characters with # are not suspected:");
            sleep(2);
            system("cls");
            //mark characters that we are sure they can not be mr.jack with #
            step2(info.jack, i);
            //step3(this part is not in project description)
            if (i == 2)
                //in round 2 turn the light 3 off
                map[lamp_coordinate[2].x][lamp_coordinate[2].y].type = 3;
            else if (i == 4)
                //in round 4 turn the light 7 off
                map[lamp_coordinate[6].x][lamp_coordinate[6].y].type = 3;
            system("cls");
            system("color 07");
            draw_map();
            sleep(3);
            system("cls");
            system("color 75");
            printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n                                     "
                   "                                                 ROUND %d FINISHED\n", i);
            info.num_round = i;
            save();
            sleep(2);
        }
    }
    //if after 8 rounds game was not terminated
    for (int i = 0; i < 22; ++i) {
        for (int j = 0; j < 15; ++j) {
            printf("MR.JACK WON!              ");
        }
        printf("\n");
    }
    color();

    getchar();
    return 0;
}
