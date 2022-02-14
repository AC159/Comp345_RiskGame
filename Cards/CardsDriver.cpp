#include <iostream>
#include "Cards.h"
#include "../Player/Player.h"

using namespace std;
using namespace Cards;

//int main(int argc, char *argv[]) {
void Cards::Card::cardsDriver() {

    // Create a set of different cards (these cards will be deleted by the Deck class)
    Card *c1 = new Bomb();
    Card *c2 = new Bomb();
    Card *c3 = new Reinforcement();
    Card *c4 = new Reinforcement();
    Card *c5 = new Blockade();
    Card *c6 = new Blockade();
    Card *c7 = new Airlift();
    Card *c8 = new Airlift();
    Card *c9 = new Diplomacy();
    Card *c10 = new Diplomacy();

    Deck deck{};
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

    Deck deck2{};
    Card *c11 = new Airlift();
    Card *c12 = new Airlift();
    Card *c13 = new Diplomacy();
    Card *c14 = new Diplomacy();

    deck2.cards.push_back(c11);
    deck2.cards.push_back(c12);
    deck2.cards.push_back(c13);
    deck2.cards.push_back(c14);

    // make a deep copy of all the cards of deck and delete all the cards of deck2
    cout << "Creating another deck of cards from the current deck by making a deep copy of all cards..." << endl;
    deck2 = deck;
    cout << "Deck 2: " << endl;
    cout << deck2;

    cout << "Creating a hand of cards and drawing cards from the deck..." << endl;
    Hand *hand = new Hand();

    // Draw cards from deck and add them to the hand object
    hand->cards.push_back(deck2.draw());
    hand->cards.push_back(deck2.draw());
    hand->cards.push_back(deck2.draw());

    cout << "Cards in the hand: " << endl;
    cout << *hand << endl;

    cout << "Creating a player and assigning the hand to the player...by making a deep copy of the hand" << endl
         << endl;
    Players::Player *p = new Players::Player{"one"};
    *p->hand = *hand;
    while (!p->hand->cards.empty()) {
        Card *card = p->hand->cards.at(0);
        card->play(*p, deck2);
    }

    delete p;
    delete hand;
}
//    return 0;
//}
