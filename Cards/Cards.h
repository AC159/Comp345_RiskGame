
#ifndef COMP345RISKGAME_CARDS_H
#define COMP345RISKGAME_CARDS_H

#include <iostream>
#include <vector>
#include <string>

using namespace std;

// Card class is an abstract class that will be implemented by subclasses
class Card {
public:
    friend ostream& operator<<(ostream &out, const Card &card);
    virtual string getType() = 0;
    virtual void play() = 0;
    virtual ~Card();
};


// Card of type Bomb
class Bomb : public Card {
private:
    string type;
public:
    Bomb();
    Bomb(const Bomb &bomb);

    Bomb& operator=(const Bomb &bomb);

    string getType() override;
    void play() override;

    ~Bomb() override;
};


// Card of type Reinforcement
class Reinforcement : public Card {
private:
    string type;
public:
    Reinforcement();
    Reinforcement(const Reinforcement &r);

    Reinforcement& operator=(const Reinforcement &r);

    string getType() override;
    void play() override;

    ~Reinforcement() override;
};


// Card of type Blockade
class Blockade : public Card {
private:
    string type;
public:
    Blockade();
    Blockade(const Blockade &blockade);

    Blockade& operator=(const Blockade &blockade);

    string getType() override;
    void play() override;

    ~Blockade() override;
};


// Card of type Airlift
class Airlift : public Card {
private:
    string type;
public:
    Airlift();
    Airlift(const Airlift &airlift);

    Airlift& operator=(const Airlift &airlift);

    string getType() override;
    void play() override;

    ~Airlift() override;
};


// Card of type Diplomacy
class Diplomacy : public Card {
private:
    string type;
public:
    Diplomacy();
    Diplomacy(const Diplomacy &diplomacy);

    Diplomacy& operator=(const Diplomacy &diplomacy);

    string getType() override;
    void play() override;

    ~Diplomacy() override;
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
