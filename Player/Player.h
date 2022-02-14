
#ifndef COMP345RISKGAME_PLAYER_H
#define COMP345RISKGAME_PLAYER_H

#include <map>
#include <list>
#include <string>
#include "../Map/Map.h"
#include "../Orders/Orders.h"
#include "../Cards/Cards.h"

// Declare Players namespace for the Player class
namespace Players {
    class Player;
    std::ostream& operator<<(std::ostream& out, const Player& player);
}

// Represents a single player which owns a collection of territories, a hand of cards and a list of orders.
class Players::Player {
    private:
        std::string name; // name of player

    public:
        std::map<int, Graph::Territory*> territories; // collection of territories
        Cards::Hand *hand; // collection of cards
        Orders::OrdersList *orders; // list of orders

        Player();   // default constructor
        Player(std::string newName);
        Player(const Player &player);   // copy constructor
        ~Player(); // destructor
        static void playerDriver();

        // assignment operator
        Player& operator=(const Player &player); 
        // ostream operator
        friend std::ostream& operator<<(std::ostream& out, const Player& player);

        //returns a list of territories to be defended
        std::map<int, Graph::Territory*> toDefend();

        //returns a list of territories to be attacked
        std::map<int, Graph::Territory*> toAttack(std::list<Graph::Edge*> &edges);

        //creates an order object and adds it to the player's list of orders
        void issueOrder(const std::string& orderType) const; // orderType: deploy, advance, bomb, blockade, airlift, negotiate

        // accessor method for name
        std::string getName();

        // mutator method for name
        void setName(std::string newName);

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
};


#endif //COMP345RISKGAME_PLAYER_H