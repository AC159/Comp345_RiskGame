
#ifndef COMP345RISKGAME_PLAYER_H
#define COMP345RISKGAME_PLAYER_H

#include <map>
#include <list>
#include <string>
#include <set>
using namespace std;

//test classes for territories, orders, and cards
class TestTerritories{
    public:
        int *id;
        int *adjacentTerritories;
        TestTerritories();
        TestTerritories(int *newID, int *newAT);
};
class TestCards{};
class TestOrders{};

// Represents a single player which owns a collection of territories, a hand of cards and a list of orders.
class Player{
    private:
        map<int, TestTerritories> *territories;
        list<TestCards> *cards;
        list<TestOrders> *orders;
        string *name; // name/identifier of the player

    public:
        Player(string *newName);

        map<int, TestTerritories>* getTerritories();

        //returns and displays arbitrary territories to be defended
        set<int> toDefend();

        //returns and displays arbitrary territories to be attacked
        set<int> toAttack();

        //creates an order object and adds it to the player's list of orders
        void issueOrder(TestOrders order);
};

#endif //COMP345RISKGAME_PLAYER_H