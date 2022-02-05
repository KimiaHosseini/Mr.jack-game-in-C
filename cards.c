#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct card {
    char name[3];
    char description[100];
    struct card *next;
};

struct card *suspected;
struct card *play_cards;

//add each card to the top of list
void make_suspected_cards(struct card **head, char *s) {
    struct card *NewNode = (struct card *) malloc(sizeof(struct card));
    strcpy(NewNode->name, s);
    NewNode->next = *head;
    *head = NewNode;
}

//add each card to the end of list
void make_suspected_cards2(struct card **head, char *s) {
    struct card *NewNode = (struct card *) malloc(sizeof(struct card));
    struct card *last = *head;
    strcpy(NewNode->name, s);
    NewNode->next = NULL;
    //if the list is empty
    if (*head == NULL) {
        *head = NewNode;
        return;
    }
    //find the last node in the list
    while (last->next != NULL)
        last = last->next;
    last->next = NewNode;
}

//add each card to the top of the list
void make_play_cards(struct card **head, char *s1, char *s2) {
    struct card *NewNode = (struct card *) malloc(sizeof(struct card));
    strcpy(NewNode->name, s1);
    strcpy(NewNode->description, s2);
    NewNode->next = *head;
    *head = NewNode;
}

//add each card to the end of list
void make_play_cards2(struct card **head, char *s1, char *s2) {
    struct card *NewNode = (struct card *) malloc(sizeof(struct card));
    struct card *last = *head;
    strcpy(NewNode->name, s1);
    strcpy(NewNode->description, s2);
    NewNode->next = NULL;
    //if the list is empty
    if (*head == NULL) {
        *head = NewNode;
        return;
    }
    //find the last node in the list
    while (last->next != NULL)
        last = last->next;
    last->next = NewNode;
}

//set name of cards and their description in play cards list
void set_play_cards() {
    make_play_cards(&play_cards, "SH", "1)1to3 move  -> 2) see a suspected card");
    make_play_cards(&play_cards, "JW", "1)1to3 move -> 2) choose a place to light its way");
    make_play_cards(&play_cards, "JS", "1to3 move  <-> turn a light on and turn a light off");
    make_play_cards(&play_cards, "IL", "1)1to3 move  -> 2) close an exit way");
    make_play_cards(&play_cards, "MS", "1to4 move : also can pass all buildings");
    make_play_cards(&play_cards, "SG", "1to3 move  <-> 3 move to get some characters closer to him");
    make_play_cards(&play_cards, "WG", "1to3 move or replace its place with another one");
    make_play_cards(&play_cards, "JB", "1to3 move  <-> open a hole and close another one");
}

//swap 2 strings
void swap(char *s1, char *s2) {
    char temp[100];
    strcpy(temp, s2);
    strcpy(s2, s1);
    strcpy(s1, temp);
}

//swap data of 2 nodes in suspected cards
void swap_node_suspected(struct card **head, int n1, int n2) {
    struct card *current1 = *head;
    struct card *current2 = *head;
    //shift current1 until n1th node
    for (int i = 0; i < n1; ++i) {
        current1 = current1->next;
    }
    //shift current2 until n2th node
    for (int i = 0; i < n2; ++i) {
        current2 = current2->next;
    }
    //swap data of n1th node of suspected cards with n2th node
    swap(current1->name, current2->name);
}

//swap data of 2 nodes in play cards
void swap_node_play(struct card **head, int n1, int n2) {
    struct card *current1 = *head;
    struct card *current2 = *head;
    //shift current1 until n1th node
    for (int i = 0; i < n1; ++i) {
        current1 = current1->next;
    }
    //shift current2 until n2th node
    for (int i = 0; i < n2; ++i) {
        current2 = current2->next;
    }
    //swap data of n1th node of play cards with n2th node
    swap(current1->name, current2->name);
    swap(current1->description, current2->description);
}

//(after shuffling the play cards)set first 4 cards as play_cards_odd
struct card *odd() {
    struct card *play_cards_odd = NULL;
    make_play_cards(&play_cards_odd, play_cards->name, play_cards->description);
    make_play_cards(&play_cards_odd, play_cards->next->name, play_cards->next->description);
    make_play_cards(&play_cards_odd, play_cards->next->next->name, play_cards->next->next->description);
    make_play_cards(&play_cards_odd, play_cards->next->next->next->name, play_cards->next->next->next->description);
    return play_cards_odd;
}

//(after shuffling the play cards and choosing play_cards_odd)set second 4 cards as play_cards_even
struct card *even() {
    struct card *play_cards_even = play_cards;
    for (int i = 0; i < 4; ++i) {
        play_cards_even = play_cards_even->next;
    }
    return play_cards_even;
}

//print data of all nodes of the list
void print_play_cards(struct card *x) {
    struct card *current = x;
    while (current != NULL) {
        printf(" %s: %s\n", current->name, current->description);
        current = current->next;
    }
}

//delete a card from play cards
void delete_node(struct card **head, char *s) {
    struct card *temp = *head, *prev;
    //if the card we want to delete is the first node
    if (temp != NULL && strcmp(s, temp->name) == 0) {
        *head = temp->next;
        free(temp);
        return;
    }
    //shift prev and temp until finding the node we want to delete
    do {
        prev = temp;
        temp = temp->next;
    } while (strcmp(temp->name, s) != 0);
    //ignore that node in the list by :
    prev->next = temp->next;
    free(temp);
}

//delete a card from suspected cards and subtract from num_cards one
void delete_node_suspected(struct card **head, char *s, int *num_cards) {
    struct card *temp = *head, *prev;
    //if the card we want to delete is the first node
    if (temp != NULL && strcmp(s, temp->name) == 0) {
        *head = temp->next;
        free(temp);
        (*num_cards)--;
        return;
    }
    //shift prev and temp until finding the node we want to delete
    do {
        prev = temp;
        temp = temp->next;
    } while (strcmp(temp->name, s) != 0);
    //ignore that node in the list by :
    prev->next = temp->next;
    free(temp);
    (*num_cards)--;
}

/*shuffle play cards
* 5 times generate two random numbers n1,n2
* and swap n1th node with n2th node of list*/
struct card *shuffle_play_cards() {
    time_t t = time(NULL);
    srand(t);
    int n1, n2;
    play_cards = NULL;
    set_play_cards();
    //shuffle play cards
    for (int i = 0; i < 5; ++i) {
        n1 = rand() % 8;
        n2 = rand() % 8;
        swap_node_play(&play_cards, n1, n2);
    }
    return play_cards;
}