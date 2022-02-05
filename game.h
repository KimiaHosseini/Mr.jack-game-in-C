#ifndef MAIN_C_GAME_H
#define MAIN_C_GAME_H

#include <stdio.h>
#include "action.h"

struct info {
    int jack;
    int num_card;
    int num_round;
};
struct info info;

//set default features of game
struct info new();

//ask players that if they want to exit,save the game & exit
void save();

//load num of character of jack,num of suspected cards,num of round that is loading in struct info
//and information about action of JW(x,y,way)
//(it will be used for determining on or off characters ) in struct flashlight
struct info load();

//load play cards
void load_play_cards();

//load suspected cards
void load_suspected_card(int);

#endif //MAIN_C_GAME_H
