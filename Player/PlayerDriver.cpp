#include "Player.h"
#include <iostream>
#include <map>
using namespace std;

int main(){
    int *id = new int(1);
    int *at = new int[2] {2, 7};
    TestTerritories *one = new TestTerritories(id, at);
    id = new int(2);
    at = new int[3] {1, 7, 8};
    TestTerritories *two = new TestTerritories(id, at);
    id = new int(3);
    at = new int[3] {2, 4, 9};
    TestTerritories *three = new TestTerritories(id, at);
    id = new int(7);
    at = new int[3] {1, 2, 8};
    TestTerritories *seven = new TestTerritories(id, at);
    id = new int(8);
    at = new int[2] {2, 7};
    TestTerritories *eight = new TestTerritories(id, at);

    Player *playerOne = new Player(new string("One"));

    playerOne->getTerritories()->insert(pair<int, TestTerritories>(*(one->id), *one));
    playerOne->getTerritories()->insert(pair<int, TestTerritories>(*(eight->id), *eight));

    playerOne->toDefend();

    playerOne->toAttack();

    return 0;
}
