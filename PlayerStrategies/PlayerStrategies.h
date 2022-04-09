
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
    virtual void issueOrder(const GameEngine &game) = 0;

    PlayerStrategies(const PlayerStrategies &ps);
    PlayerStrategies(Players::Player *p);
    virtual ~PlayerStrategies();
};


class BenevolentPlayerStrategy : public PlayerStrategies {
public:
    std::multimap<int, Graph::Territory *> toDefend(const std::vector<Graph::Edge *> &mapEdges) override;
    std::multimap<int, Graph::Territory *> toAttack(const std::vector<Graph::Edge *> &edges) override;
    void issueOrder(const GameEngine &game) override;

    BenevolentPlayerStrategy& operator=(const BenevolentPlayerStrategy &b);
    friend std::ostream& operator<<(std::ostream &out, const BenevolentPlayerStrategy &b);

    BenevolentPlayerStrategy(Players::Player *p);
    BenevolentPlayerStrategy(const BenevolentPlayerStrategy &b);
    ~BenevolentPlayerStrategy() override;
};

class HumanPlayerStrategy : public PlayerStrategies {
public:
    std::multimap<int, Graph::Territory *> toDefend(const std::vector<Graph::Edge *> &edges) override;
    std::multimap<int, Graph::Territory *> toAttack(const std::vector<Graph::Edge *> &edges) override;
    void issueOrder(const GameEngine &game) override;

    HumanPlayerStrategy& operator=(const HumanPlayerStrategy &h);
    friend std::ostream& operator<<(std::ostream &out, const HumanPlayerStrategy &h);

    explicit HumanPlayerStrategy(Players::Player *p);
    HumanPlayerStrategy(const HumanPlayerStrategy &h);
    ~HumanPlayerStrategy() override;

private:
    static void printTerritories(const multimap<int, Graph::Territory *> &territories);
    void printTerritoriesWithNeighbors(const vector<Graph::Edge *> &, const multimap<int, Graph::Territory *> &) const;
};

#endif //COMP345RISKGAME_PLAYERSTRATEGIES_H
