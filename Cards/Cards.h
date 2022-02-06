
#ifndef COMP345RISKGAME_CARDS_H
#define COMP345RISKGAME_CARDS_H

#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Card {
public:

    Card();
    Card(const Card &card);

    Card& operator=(const Card &card);
    friend ostream& operator<<(ostream &out, const Card &card);

    void play();

    ~Card();
};


// Card of type Bomb
class Bomb : public Card {
public:
    Bomb();
    Bomb(const Bomb &bomb);

    Bomb& operator=(const Bomb &bomb);
    friend ostream& operator<<(ostream &out, const Bomb &bomb);

    void play();

    ~Bomb();
};


// Card of type Reinforcement
class Reinforcement : public Card {
public:
    Reinforcement();
    Reinforcement(const Reinforcement &r);

    Reinforcement& operator=(const Reinforcement &r);
    friend ostream& operator<<(ostream &out, const Reinforcement &r);

    void play();

    ~Reinforcement();
};


// Card of type Blockade
class Blockade : public Card {
public:
    Blockade();
    Blockade(const Blockade &blockade);

    Blockade& operator=(const Blockade &blockade);
    friend ostream& operator<<(ostream &out, const Blockade &blockade);

    void play();

    ~Blockade();
};


// Card of type Airlift
class Airlift : public Card {
public:
    Airlift();
    Airlift(const Airlift &airlift);

    Airlift& operator=(const Airlift &airlift);
    friend ostream& operator<<(ostream &out, const Airlift &airlift);

    void play();

    ~Airlift();
};


// Card of type Diplomacy
class Diplomacy : public Card {
public:
    Diplomacy();
    Diplomacy(const Diplomacy &diplomacy);

    Diplomacy& operator=(const Diplomacy &diplomacy);
    friend ostream& operator<<(ostream &out, const Diplomacy &diplomacy);

    void play();

    ~Diplomacy();
};


class Hand {
public:
    vector<Card*> cards;

    Hand();
    Hand(const Hand &hand);

    Hand& operator=(const Hand &hand);
    friend ostream& operator<<(ostream &out, const Hand &hand);

    ~Hand();
};


class Deck {
public:
    vector<Card*> cards;

    Deck();
    Deck(const Deck &deck);

    Deck& operator=(const Deck &deck);
    friend ostream& operator<<(ostream &out, const Deck &deck);

    Card* draw();

    ~Deck();

};

#endif //COMP345RISKGAME_CARDS_H
