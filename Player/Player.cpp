#include <iostream>
#include <array>
#include <set>
#include <list>
#include "Player.h"

TestTerritories::TestTerritories(){
    id = new int(0);
    adjacentTerritories = new int[5] {0, 0, 0, 0, 0};
}

TestTerritories::TestTerritories(int *newID, int *newAT){
    id = newID;
    adjacentTerritories = newAT;
}

//constructor for Player class to initialize the name and the collection of territories, cards, and orders
Player::Player(string *newName){
    territories = new map<int, TestTerritories>;
    cards = new list<TestCards>;
    orders = new list<TestOrders>;
    name = newName;
}

map<int, TestTerritories>* Player::getTerritories() {
    return this->territories;
}

//return and display all territories that can be defended
set<int> Player::toDefend(){
    set<int> *territoriesToDefend = new set<int>;
    cout << "Player " << *name << " can defend:" << endl;
    for(map<int,TestTerritories>::iterator it = territories->begin(); it != territories->end(); it++){
        territoriesToDefend->insert(*(it->second.id));
        cout << *(it->second.id) << endl;
    }

    return *territoriesToDefend;
}

//return and display all territories that can be attacked
set<int> Player::toAttack(){
    cout << "Player " << *name << " can attack:" << endl;
    set<int> *temp = new set<int>;  //used to collect all unique adjacent territories
    for(map<int,TestTerritories>::iterator it = territories->begin(); it != territories->end(); ++it){
        int sizeOfATArray = sizeof(it->second.adjacentTerritories)/sizeof(int);
        
        //inserting all adjacent territories of the player's territories to a set
        for(int i = 0; i < sizeOfATArray; i++){
            temp->insert((it->second.adjacentTerritories)[i]);
        }
    }

    //iterating through all the adjacent territories
    for(set<int>::iterator it = temp->begin(); it != temp->end(); ++it){
        cout << *it << endl;
    }

    return *temp;
}

void Player::issueOrder(TestOrders order){
    (*orders).insert(orders->end(),order);
}