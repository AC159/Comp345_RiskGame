#include <iostream>
#include <array>
#include <set>
#include <list>
#include "Player.h"


Player::Player(){}

//constructor for Player class to initialize the name and the collection of territories, cards, and orders
Player::Player(string newName){
    territories = new map<int, Territory*>;
    cards = new list<TestCards*>;
    orders = new list<TestOrders*>;
    name = newName;
}

Player::Player(const Player &player){
    map<int, Territory*>* otherT = new map<int, Territory*>;
    for(map<int,Territory*>::iterator it = player.territories->begin(); it != player.territories->end(); it++){
        otherT->insert(pair<int, Territory*>(it->second->countryNumber, it->second));
        cout << (it->second->name) << endl;
    }
}

Player &Player::operator=(const Player &player){

    if (this == &player) return *this;

    this->name = player.name;
    delete this->territories;
    this->territories = player.territories;
    // for(map<int, Territory*>::iterator it = player.territories->begin(); it != player.territories->end(); it++){
    //     this->territories->insert(it->first,it->second);
    // }
    delete this->orders;
    this->orders = player.orders;
    delete this->cards;
    this->cards = player.cards;

    return *this;
}

ostream& operator<<(ostream &out, const Player &player){
    out << player.name;
    // for(map<int, Territory*>::iterator it = player.territories->begin(); it != player.territories->end(); it++){
    //     out << it->second->name << endl;
    // }
    return out;
}

map<int, Territory*>* Player::getTerritories() {
    return this->territories;
}

//return and display all territories that can be defended
map<int, Territory*>* Player::toDefend(){
    map<int, Territory*> *territoriesToDefend = new map<int, Territory*>;
    for(map<int,Territory*>::iterator it = territories->begin(); it != territories->end(); it++){
        territoriesToDefend->insert(pair<int, Territory*>(it->second->countryNumber, (it->second)));
    }
    return territoriesToDefend;
}

//return and display all territories that can be attacked
map<int, Territory*>* Player::toAttack(list<Edge*> &edges){
    map<int, Territory*>* temp = new map<int, Territory*>;
    for(list<Edge*>::iterator itEdges = edges.begin(); itEdges != edges.end(); itEdges++){
        if(territories->find((*itEdges)->source->countryNumber) != territories->end() && territories->find((*itEdges)->destination->countryNumber) == territories->end()){
            temp->insert(pair<int, Territory*>((*itEdges)->destination->countryNumber, (*itEdges)->destination));
        }
        else if(territories->find((*itEdges)->destination->countryNumber) != territories->end() && territories->find((*itEdges)->source->countryNumber) == territories->end()){
            temp->insert(pair<int, Territory*>((*itEdges)->source->countryNumber, (*itEdges)->source));
        }
    }
    return temp;
}

void Player::issueOrder(TestOrders *order){
    (*orders).insert(orders->end(),order);
}