#include <iostream>
#include <array>
#include <set>
#include <list>
#include "Player.h"


Player::Player(const Player &p) = default;

Player::~Player() = default;


Player::Player(){
    name = "No Name";
    territories = new map<int, Territory*>;
    cards = new list<TestCards*>;
    orders = new list<TestOrders*>;
}


//constructor for Player class to initialize the name and the collection of territories, cards, and orders
Player::Player(string newName){
    name = newName;
    territories = new map<int, Territory*>;
    cards = new list<TestCards*>;
    orders = new list<TestOrders*>;
}


// copy constructor(not tested)
Player::Player(const Player &player){
    this->name = player.name;
    // delete, create a new mapping of territories and copy all territories from other player
    delete territories;
    territories = new map<int, Territory*>;
    for(map<int,Territory*>::iterator it = player.territories->begin(); it != player.territories->end(); it++){
        territories->insert(pair<int, Territory*>(it->second->countryNumber, it->second));
        // cout << (it->second->name) << endl;
    }
    // delete, create a new list of cards and copy all cards from other player
    delete cards;
    cards = new list<TestCards*>;
    for(list<TestCards*>::iterator it = player.cards->begin(); it != player.cards->end(); it++){
        list<TestCards*>::iterator itCards = cards->begin();
        cards->insert(itCards, *it);
        itCards++;
        // cout << (it->second->name) << endl;
    }
    // delete, create a new list of orders and copy all orders from other player
    delete orders;
    orders = new list<TestOrders*>;
    for(list<TestOrders*>::iterator it = player.orders->begin(); it != player.orders->end(); it++){
        list<TestOrders*>::iterator itOrders = orders->begin();
        orders->insert(itOrders, *it);
        itOrders++;
        // cout << (it->second->name) << endl;
    }
}


// destructor
Player::~Player(){
    delete territories;
    delete cards;
    delete orders;
}


// assignment operator(not tested)
Player &Player::operator=(const Player &player){
    if (this == &player) return *this;
    
    this->name = player.name;
    delete this->territories;
    this->territories = new map<int, Territory*>;
    for(map<int, Territory*>::iterator it = player.territories->begin(); it != player.territories->end(); it++){
        this->territories->insert(pair<int, Territory*>(it->second->countryNumber, it->second);
    }
    delete this->cards;
    this->cards = new list<TestCards*>;
    for(list<TestCards*>::iterator it = player.cards->begin(); it != player.cards->end(); it++){
        list<TestCards*>::iterator itCards = cards->begin();
        this->cards->insert(itCards, *it);
        itCards++;
    }
    delete this->orders;
    this->orders = new list<TestOrders*>;
    for(list<TestOrders*>::iterator it = player.orders->begin(); it != player.orders->end(); it++){
        list<TestOrders*>::iterator itOrders = orders->begin();
        this->orders->insert(itOrders, *it);
        itOrders++;
    }

    return *this;
}

// ostream operator
ostream& operator<<(ostream &out, const Player &player){
    out << player.name;
    // for(map<int, Territory*>::iterator it = player.territories->begin(); it != player.territories->end(); it++){
    //     out << it->second->name << endl;
    // }
    return out;
}

// accessor for territories
map<int, Territory*>* Player::getTerritories() {
    return this->territories;
}


// return a list of territories that can be defended by the player
map<int, Territory*>* Player::toDefend(){
    map<int, Territory*> *territoriesToDefend = new map<int, Territory*>;
    for(map<int,Territory*>::iterator it = territories->begin(); it != territories->end(); it++){
        territoriesToDefend->insert(pair<int, Territory*>(it->second->countryNumber, (it->second)));
    }
    return territoriesToDefend;
}


// return a list of territories that can be attacked by the player
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

// inserts an order to the player's list of orders(not tested)
void Player::issueOrder(TestOrders *order){
    (*orders).insert(orders->end(),order);
}

