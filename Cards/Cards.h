
#ifndef COMP345RISKGAME_CARDS_H
#define COMP345RISKGAME_CARDS_H

#include <iostream>
#include <vector>
#include <string>
#include "../Player/Player.h"

using namespace std;

// Card class is an abstract class that will be implemented by subclasses
class Card {
public:
    friend ostream& operator<<(ostream &out, const Card &card);
    virtual string getType() const = 0;
    virtual void play(Player &player) = 0;
    virtual Card* clone() = 0;
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

    Card* clone() override;
    string getType() const override;
    void play(Player &player) override;

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

    Card* clone() override;
    string getType() const override;
    void play(Player &player) override;

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

    Card* clone() override;
    string getType() const override;
    void play(Player &player) override;

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

    Card* clone() override;
    string getType() const override;
    void play(Player &player) override;

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

    Card* clone() override;
    string getType() const override;
    void play(Player &player) override;

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
