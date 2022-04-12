
#ifndef COMP345RISKGAME_PLAYER_H
#define COMP345RISKGAME_PLAYER_H

#include <map>
#include <list>
#include <string>
#include "../Map/Map.h"
#include "../Orders/Orders.h"
#include "../Cards/Cards.h"
#include "../GameEngine/GameEngine.h"

// forward declare needed classes
class PlayerStrategies;
class GameEngine;

// Declare Players namespace for the Player class
namespace Players {
    class Player;
    std::ostream &operator<<(std::ostream &out, const Player &player);
}

// Represents a single player which owns a collection of territories, a hand of cards and a list of orders.
class Players::Player {
private:
    std::string name;

public:
    PlayerStrategies *ps;
    std::map<int, Graph::Territory *> territories; // collection of territories
    Cards::Hand *hand; // collection of cards
    Orders::OrdersList *orders; // list of orders
    int reinforcementPool; //the armies currently available for deployment
    bool receivesCard; //returns whether player has conquered territory and should receive card at the end of turn
    bool isEliminated;
    std::vector<std::string> cannotAttack; //list of players the player cannot attack due to diplomacy card

    Player();   // default constructor
    Player(const std::string& newName);

    Player(const Player &player);   // copy constructor
    ~Player(); // destructor

    // assignment operator
    Player &operator=(const Player &player);

    // ostream operator
    friend std::ostream &operator<<(std::ostream &out, const Player &player);

    //returns a list of territories to be defended in order of priority
    std::multimap<int, Graph::Territory *> toDefend(const std::vector<Graph::Edge *> &mapEdges) const;

    //returns a list of territories to be attacked in order of priority
    std::multimap<int, Graph::Territory *> toAttack(const std::vector<Graph::Edge *> &edges) const;

    //creates an order object and adds it to the player's list of orders
    void issueOrder(GameEngine &game) const;

    // accessor method for name
    [[nodiscard]] std::string getName() const;

    // display player's territories
    void displayTerritories();

    // add a territory
    void addTerritory(Graph::Territory &territory);

    // remove a territory
    void removeTerritory(Graph::Territory &territory);

    // display player's cards
    void displayCards();

    // add card
    void addCard(Cards::Card &card);

    // remove card
    void removeCard(Cards::Card &card);

    // display player's orders
    void displayOrders();

    // returns the list contents of cannotAttack
    [[nodiscard]] std::string cannotAttackString() const;
};


#endif //COMP345RISKGAME_PLAYER_H