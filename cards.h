#ifndef MAIN_C_CARDS_H
#define MAIN_C_CARDS_H
struct card {
    char name[3];
    char description[100];
    struct card *next;
};
struct card *suspected;
struct card *play_cards;

//add each card to the top of list
void make_suspected_cards(struct card **, char *);

//add each card to the end of list
void make_suspected_cards2(struct card **head, char *s);

//add each card to the top of the list
void make_play_cards(struct card **, char *, char *);

//add each card to the end of list
void make_play_cards2(struct card **, char *, char *);

//set name of cards and their description in play cards list
void set_play_cards();

//swap 2 strings
void swap(char *, char *);

//swap data of 2 nodes in suspected cards
void swap_node_suspected(struct card **, int, int);

//swap data of 2 nodes in play cards
void swap_node_play(struct card **, int, int);

//(after shuffling the play cards)set first 4 cards as play_cards_odd
struct card *odd();

//(after shuffling the play cards and choosing play_cards_odd)set second 4 cards as play_cards_even
struct card *even();

//print data of all nodes of the list
void print_play_cards(struct card *);

//delete a card from play cards
void delete_node(struct card **, char *);

//delete a card from suspected cards and subtract from num_cards one
void delete_node_suspected(struct card **, char *, int *);

/*shuffle play cards
* 5 times generate two random numbers n1,n2
* and swap n1th node with n2th node of list*/
struct card *shuffle_play_cards();

#endif //MAIN_C_CARDS_H
