
#ifndef COMP345RISKGAME_PLAYER_H
#define COMP345RISKGAME_PLAYER_H

#include <map>
#include <list>
#include <string>
#include "..\Map\Map.h"
#include "..\Cards\Cards.h"
#include "..\Orders\Orders.h"
using namespace std;
using namespace Graph;
// using namespace Cards;
// using namespace Orders;

// class Territory;
// class Edge;
// class Hand;
class OrdersList;   

namespace Players{
    class Player;
}

// Represents a single player which owns a collection of territories, a hand of cards and a list of orders.
class Player{
    private:
        string *name; // name of player

    public:
        map<int, Territory*> territories; // collection of territories
        Hand *hand; // collection of cards
        OrdersList *orders; // list of orders

        Player();   // default constructor
        Player(string &newName); 
        Player(const Player &player);   // copy constructor
        ~Player(); // destructor

        // assignment operator
        Player& operator=(const Player &player); 
        // ostream operator
        friend ostream& operator<<(ostream& out, const Player& player);

        //returns a list of territories to be defended
        map<int, Territory*> toDefend();

        //returns a list of territories to be attacked
        map<int, Territory*> toAttack(list<Edge*> &edges);

        //creates an order object and adds it to the player's list of orders
        void issueOrder(string orderType); // orderType: deploy, advance, bomb, blockade, airlift


        // accessor method for name
        string getName();

        // mutator method for name
        void setName(string newName);

        // display player's territories
        void displayTerritories();

        // add a territory
        void addTerritory(Territory &territory);

        // remove a territory
        void removeTerritory(Territory &territory);

        // display player's cards
        void displayCards();

        // add card
        void addCard(Card *card);

        // remove card
        void removeCard(Card &card);

        // display player's orders
        void displayOrders();
};


#endif //COMP345RISKGAME_PLAYER_H