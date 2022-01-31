
#ifndef COMP345RISKGAME_PLAYER_H
#define COMP345RISKGAME_PLAYER_H

#include <map>
#include <list>
#include <string>
#include <set>
#include "..\Map\Map.h"
using namespace std;

//test classes for territories, orders, and cards
class Territory;
class Edge;
class TestCards{};
class TestOrders{};

// Represents a single player which owns a collection of territories, a hand of cards and a list of orders.
class Player{
    private:
        map<int, Territory*> *territories;
        list<TestCards*> *cards;
        list<TestOrders*> *orders;
        string name;

    public:
        Player();   // default constructor
        Player(string newName);
        Player(const Player &player);   // copy constructor

        Player& operator=(const Player &player); // assignment operator
        friend ostream& operator<<(ostream& out, const Player& player);

        map<int, Territory*>* getTerritories();

        //returns and displays arbitrary territories to be defended
        map<int, Territory*>* toDefend();

        //returns and displays arbitrary territories to be attacked
        map<int, Territory*>* toAttack(list<Edge*> &edges);

        //creates an order object and adds it to the player's list of orders
        void issueOrder(TestOrders *order);
};



#endif //COMP345RISKGAME_PLAYER_H