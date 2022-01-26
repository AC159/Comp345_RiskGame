
#ifndef COMP345RISKGAME_PLAYER_H
#define COMP345RISKGAME_PLAYER_H

#include <map>
#include <list>
#include <string>
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
//main class for players
class Player{
    public:
        //collection of territories, cards, and orders
        map<int, TestTerritories> *territories;
        list<TestCards> *cards;
        list<TestOrders> *orders;
        //name of player
        string *name;
        //constructor with name
        Player(string *newName);
        //display territories that can be defended(territories that are occupied by the player)
        void toDefend();
        //display territories that can be attacked(territories that are adjacent to the player's territories)
        void toAttack();
        void issueOrder(TestOrders order);
    private:
};


#endif //COMP345RISKGAME_PLAYER_H
