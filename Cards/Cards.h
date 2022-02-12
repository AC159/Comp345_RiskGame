
#ifndef COMP345RISKGAME_CARDS_H
#define COMP345RISKGAME_CARDS_H

#include <iostream>
#include <vector>
#include <string>
#include "../Player/Player.h"

using namespace std;

namespace Cards {
    class Card;
    class Bomb;
    class Reinforcement;
    class Blockade;
    class Airlift;
    class Diplomacy;
    class Hand;
    class Deck;
}

// Card class is an abstract class that will be implemented by subclasses
class Cards::Card {
public:
    friend ostream& operator<<(ostream &out, const Card &card);
    virtual string getType() const = 0;
    virtual void play(Player &player, Deck &deck) = 0;
    virtual Card* clone() = 0;
    virtual ~Card();
};


// Card of type Bomb
class Cards::Bomb : public Card {
private:
    string type;
public:
    Bomb();
    Bomb(const Bomb &bomb);

    Bomb& operator=(const Bomb &bomb);

    Card* clone() override;
    string getType() const override;
    void play(Player &player, Deck &deck) override;

    ~Bomb() override;

    void play(Player &player, Deck &deck);
};


// Card of type Reinforcement
class Cards::Reinforcement : public Card {
private:
    string type;
public:
    Reinforcement();
    Reinforcement(const Reinforcement &r);

    Reinforcement& operator=(const Reinforcement &r);

    Card* clone() override;
    string getType() const override;
    void play(Player &player, Deck &deck) override;

    ~Reinforcement() override;
};


// Card of type Blockade
class Cards::Blockade : public Card {
private:
    string type;
public:
    Blockade();
    Blockade(const Blockade &blockade);

    Blockade& operator=(const Blockade &blockade);

    Card* clone() override;
    string getType() const override;
    void play(Player &player, Deck &deck) override;

    ~Blockade() override;
};


// Card of type Airlift
class Cards::Airlift : public Card {
private:
    string type;
public:
    Airlift();
    Airlift(const Airlift &airlift);

    Airlift& operator=(const Airlift &airlift);

    Card* clone() override;
    string getType() const override;
    void play(Player &player, Deck &deck) override;

    ~Airlift() override;
};


// Card of type Diplomacy
class Cards::Diplomacy : public Card {
private:
    string type;
public:
    Diplomacy();
    Diplomacy(const Diplomacy &diplomacy);

    Diplomacy& operator=(const Diplomacy &diplomacy);

    Card* clone() override;
    string getType() const override;
    void play(Player &player, Deck &deck) override;

    ~Diplomacy() override;
};


class Cards::Hand {
public:
    vector<Card*> cards;

    Hand();
    Hand(const Hand &hand);

    Hand& operator=(const Hand &hand);
    friend ostream& operator<<(ostream &out, const Hand &hand);

    ~Hand();
};


class Cards::Deck {
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
