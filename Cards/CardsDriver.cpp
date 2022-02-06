#include <iostream>
#include "Cards.h"

using namespace std;

int main(int argc, char *argv[]) {

    // Create a set of different cards (these cards will be deleted by the Deck class)
    Card* c1 = new Bomb();
    Card* c2 = new Bomb();
    Card* c3 = new Reinforcement();
    Card* c4 = new Reinforcement();
    Card* c5 = new Blockade();
    Card* c6 = new Blockade();
    Card* c7 = new Airlift();
    Card* c8 = new Airlift();
    Card* c9 = new Diplomacy();
    Card* c10 = new Diplomacy();

    Deck deck {};
    deck.cards.push_back(c1);
    deck.cards.push_back(c2);
    deck.cards.push_back(c3);
    deck.cards.push_back(c4);
    deck.cards.push_back(c5);
    deck.cards.push_back(c6);
    deck.cards.push_back(c7);
    deck.cards.push_back(c8);
    deck.cards.push_back(c9);
    deck.cards.push_back(c10);

    cout << "Deck 1: " << endl;
    cout << deck << endl;

    Deck deck2 {};
    Card* c11 = new Airlift();
    Card* c12 = new Airlift();
    Card* c13 = new Diplomacy();
    Card* c14 = new Diplomacy();

    deck2.cards.push_back(c11);
    deck2.cards.push_back(c12);
    deck2.cards.push_back(c13);
    deck2.cards.push_back(c14);

    // make a deep copy of all the cards of deck and delete all the cards of deck2
    deck2 = deck;
    cout << "Deck 2: " << endl;
    cout << deck2;

    return 0;
}
