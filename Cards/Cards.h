
#ifndef COMP345RISKGAME_CARDS_H
#define COMP345RISKGAME_CARDS_H

#include <iostream>
#include <vector>
#include <string>
#include "../Map/Map.h"

// Forward declare the Player class
namespace Players {
    class Player;

    std::ostream &operator<<(std::ostream &out, const Player &player);
}

namespace Cards {
    class Card;

    class Bomb;

    class Reinforcement;

    class Blockade;

    class Airlift;

    class Diplomacy;

    class Hand;

    class Deck;

    std::ostream &operator<<(std::ostream &out, const Card &card);

    std::ostream &operator<<(std::ostream &out, const Hand &hand);

    std::ostream &operator<<(std::ostream &out, const Deck &deck);
}

// Card class is an abstract class that will be implemented by subclasses
class Cards::Card {
public:
    friend std::ostream &operator<<(std::ostream &out, const Card &card);

    virtual std::string getType() const = 0;

    virtual Card *clone() = 0;

    virtual ~Card();
};


// Card of type Bomb
class Cards::Bomb : public Card {
private:
    std::string type;
public:
    Bomb();

    Bomb(const Bomb &bomb);

    Bomb &operator=(const Bomb &bomb);

    Card *clone() override;

    std::string getType() const override;

    void play(Players::Player* player, Deck* deck, Graph::Territory *target, Graph::Map *map);

    ~Bomb() override;

};


// Card of type Reinforcement
class Cards::Reinforcement : public Card {
private:
    std::string type;
public:
    Reinforcement();

    Reinforcement(const Reinforcement &r);

    Reinforcement &operator=(const Reinforcement &r);

    Card *clone() override;

    std::string getType() const override;

    void play(Players::Player *player, Deck *deck, Graph::Territory *target);

    ~Reinforcement() override;
};


// Card of type Blockade
class Cards::Blockade : public Card {
private:
    std::string type;
public:
    Blockade();

    Blockade(const Blockade &blockade);

    Blockade &operator=(const Blockade &blockade);

    Card *clone() override;

    std::string getType() const override;

    void play(Players::Player *player, Deck *deck, Graph::Territory *target, std::vector<Players::Player *> &players);

    ~Blockade() override;
};


// Card of type Airlift
class Cards::Airlift : public Card {
private:
    std::string type;
public:
    Airlift();

    Airlift(const Airlift &airlift);

    Airlift &operator=(const Airlift &airlift);

    Card *clone() override;

    std::string getType() const override;

    void play(Players::Player *player, Deck *deck, Graph::Territory *source, Graph::Territory *target, int armies);

    ~Airlift() override;
};


// Card of type Diplomacy
class Cards::Diplomacy : public Card {
private:
    std::string type;
public:
    Diplomacy();

    Diplomacy(const Diplomacy &diplomacy);

    Diplomacy &operator=(const Diplomacy &diplomacy);

    Card *clone() override;

    std::string getType() const override;

    void play(Players::Player *player, Players::Player *target, Deck *deck);

    ~Diplomacy() override;
};


class Cards::Hand {
public:
    std::vector<Card *> cards;

    Hand();

    Hand(const Hand &hand);

    Hand &operator=(const Hand &hand);

    friend std::ostream &operator<<(std::ostream &out, const Hand &hand);

    ~Hand();
};


class Cards::Deck {
public:
    std::vector<Card *> cards;

    Deck();

    Deck(const Deck &deck);

    Deck &operator=(const Deck &deck);

    friend std::ostream &operator<<(std::ostream &out, const Deck &deck);

    Card *draw();

    void fillDeckWithCards();

    ~Deck();

};

#endif //COMP345RISKGAME_CARDS_H
