
#ifndef COMP345RISKGAME_PLAYERSTRATEGIES_H
#define COMP345RISKGAME_PLAYERSTRATEGIES_H

#include <iostream>
#include <map>
#include <vector>
#include "../Map/Map.h"
#include "../Cards/Cards.h"
#include "../Player/Player.h"


class PlayerStrategies {
public:
    Players::Player *player;
    virtual std::multimap<int, Graph::Territory *, std::greater<>> toDefend(const std::vector<Graph::Edge *> &mapEdges) = 0;
    virtual std::multimap<int, Graph::Territory *> toAttack(const std::vector<Graph::Edge *> &edges) = 0;
    virtual void issueOrder(Cards::Deck *deck, Graph::Map *map) = 0;

    PlayerStrategies(const PlayerStrategies &ps);
    PlayerStrategies(Players::Player *p);
    virtual ~PlayerStrategies();
};


class Benevolent : public PlayerStrategies {
public:
    std::multimap<int, Graph::Territory *, std::greater<>> toDefend(const std::vector<Graph::Edge *> &mapEdges);
    std::multimap<int, Graph::Territory *> toAttack(const std::vector<Graph::Edge *> &edges);
    void issueOrder(Cards::Deck *deck, Graph::Map *map);

    Benevolent& operator=(const Benevolent &b);
    friend std::ostream& operator<<(std::ostream &out, const Benevolent &b);

    Benevolent(Players::Player *p);
    Benevolent(const Benevolent &b);
    ~Benevolent() override;
};

#endif //COMP345RISKGAME_PLAYERSTRATEGIES_H
