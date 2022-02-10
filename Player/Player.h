
#ifndef COMP345RISKGAME_PLAYER_H
#define COMP345RISKGAME_PLAYER_H

#include <map>
#include <list>
#include <string>
#include <set>
#include "../Map/Map.h"
#include "../Cards/Cards.h"
using namespace std;

//test classes for territories, orders, and cards
class Territory;
class Edge;
class TestOrders{};
class Hand;

// Represents a single player which owns a collection of territories, a hand of cards and a list of orders.
class Player{
    private:
        string *name; // name of player
        list<TestOrders*> orders; // list of orders

    public:
        map<int, Territory*> territories; // collection of territories
        Hand *hand; // collection of cards

        Player();   // default constructor
        Player(string &newName); 
        Player(const Player &player);   // copy constructor
        ~Player(); // destructor

        // assignment operator
        Player& operator=(const Player &player); 
        // ostream operator
        friend ostream& operator<<(ostream& out, const Player& player);

        // accessor method for territories
        map<int, Territory*> getTerritories();

        //returns a list of territories to be defended
        map<int, Territory*> toDefend();

        //returns a list of territories to be attacked
        map<int, Territory*> toAttack(list<Edge*> &edges);

        //creates an order object and adds it to the player's list of orders
        void issueOrder(TestOrders *order);
};



#endif //COMP345RISKGAME_PLAYER_H