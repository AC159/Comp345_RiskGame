
#ifndef COMP345RISKGAME_PLAYERSTRATEGIES_H
#define COMP345RISKGAME_PLAYERSTRATEGIES_H

#include <iostream>
#include <map>
#include <vector>
#include "../Map/Map.h"
#include "../Cards/Cards.h"
#include "../Player/Player.h"


class PlayerStrategies {
protected:
    Players::Player *player;
public:
    virtual std::multimap<int, Graph::Territory *, std::greater<>> toDefend(const std::vector<Graph::Edge *> &mapEdges) = 0;
    virtual std::multimap<int, Graph::Territory *> toAttack(const std::vector<Graph::Edge *> &edges) = 0;
    virtual void issueOrder(Cards::Deck *deck, Graph::Map *map) = 0;

    PlayerStrategies(const PlayerStrategies &ps);
    PlayerStrategies(Players::Player *p);
    virtual ~PlayerStrategies();
};


#endif //COMP345RISKGAME_PLAYERSTRATEGIES_H
