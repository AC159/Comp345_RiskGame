
#ifndef COMP345RISKGAME_PLAYERSTRATEGIES_H
#define COMP345RISKGAME_PLAYERSTRATEGIES_H

#include <iostream>
#include <map>
#include <vector>
#include "../Map/Map.h"
#include "../Cards/Cards.h"
#include "../Player/Player.h"
#include <experimental/random>

void PlayerStrategiesDriver();

class PlayerStrategies {
public:
    Players::Player *player;
    virtual std::multimap<int, Graph::Territory *> toDefend(const std::vector<Graph::Edge *> &mapEdges) = 0;
    virtual std::multimap<int, Graph::Territory *> toAttack(const std::vector<Graph::Edge *> &edges) = 0;
    virtual void issueOrder(Cards::Deck *deck, Graph::Map *map) = 0;

    PlayerStrategies(const PlayerStrategies &ps);
    PlayerStrategies(Players::Player *p);
    virtual ~PlayerStrategies();
};


class BenevolentPlayerStrategy : public PlayerStrategies {
public:
    std::multimap<int, Graph::Territory *> toDefend(const std::vector<Graph::Edge *> &mapEdges) override;
    std::multimap<int, Graph::Territory *> toAttack(const std::vector<Graph::Edge *> &edges) override;
    void issueOrder(Cards::Deck *deck, Graph::Map *map) override;

    BenevolentPlayerStrategy& operator=(const BenevolentPlayerStrategy &b);
    friend std::ostream& operator<<(std::ostream &out, const BenevolentPlayerStrategy &b);

    BenevolentPlayerStrategy(Players::Player *p);
    BenevolentPlayerStrategy(const BenevolentPlayerStrategy &b);
    ~BenevolentPlayerStrategy() override;
};

class CheaterPlayerStrategy : public PlayerStrategies {
public:
    std::multimap<int, Graph::Territory *> toDefend(const std::vector<Graph::Edge *> &mapEdges) override;
    std::multimap<int, Graph::Territory *> toAttack(const std::vector<Graph::Edge *> &edges) override;
    void issueOrder(Cards::Deck *deck, Graph::Map *map) override;

    CheaterPlayerStrategy& operator=(const CheaterPlayerStrategy &cheater);
    friend std::ostream& operator<<(std::ostream &out, const CheaterPlayerStrategy &cheater);

    CheaterPlayerStrategy(Players::Player *p);
    CheaterPlayerStrategy(const CheaterPlayerStrategy &cheater);
    ~CheaterPlayerStrategy() override;
};

#endif //COMP345RISKGAME_PLAYERSTRATEGIES_H
