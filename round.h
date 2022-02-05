#ifndef MAIN_C_ROUND_H
#define MAIN_C_ROUND_H
#include "cards.h"

//save visibility of jack in every round
int visible_jack[8];

//in phase 1: move and do actions of characters
void step1(char x[3],int turn,int round,int,int,int *);

void round_odd(int,int,int*,struct card *);

void round_even(int,int,int *,struct card *);

int is_near_character(int character);

int is_near_lamp(int character);

//check that a character is visible by flashlight of JW
int flashlight_JW(int character);

//in phase 2: mark characters that are not suspected
void step2(int jack,int );
#endif //MAIN_C_ROUND_H
