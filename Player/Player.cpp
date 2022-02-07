#include <iostream>
#include <array>
#include <set>
#include <list>
#include "Player.h"


// default constructor
Player::Player(){
    name = new string("No name");
    hand = new Hand();
    // orders = new list<TestOrders*>;
}


//constructor for Player class to initialize the name and the collection of territories, cards, and orders
Player::Player(string &newName){
    name = &newName;
    hand = new Hand();
    // orders = new list<TestOrders*>;
}


// copy constructor(not tested)
Player::Player(const Player &player){

    cout << "**Copy constructor" << endl;

    // initialize and copy the name from the other player
    this->name = new string();
    *(this->name) = *(player.name);

    // create a new mapping of territories and copy all territories from other player
    for(map<int, Territory*>::const_iterator it = player.territories.begin(); it != player.territories.end(); it++){
        territories.insert(pair<int, Territory*>(it->second->countryNumber, it->second));}

    // create a new list of cards and copy all cards from other player
    this->hand = new Hand();
    // creates a deep copy by making new objects from the other hand of cards
    for(vector<Card*>::iterator it = player.hand->cards.begin(); it != player.hand->cards.end(); it++){
        if((*it)->getType().compare("bomb")){ this->hand->cards.emplace_back(new Bomb(*(static_cast<Bomb*>(*it))));}    
        else if((*it)->getType().compare("reinforcement")){ this->hand->cards.emplace_back(new Reinforcement(*(static_cast<Reinforcement*>(*it))));}
        else if((*it)->getType().compare("blockade")){ this->hand->cards.emplace_back(new Blockade(*(static_cast<Blockade*>(*it))));}
        else if((*it)->getType().compare("airlift")){ this->hand->cards.emplace_back(new Airlift(*(static_cast<Airlift*>(*it))));}
        else if((*it)->getType().compare("diplomacy")){ this->hand->cards.emplace_back(new Diplomacy(*(static_cast<Diplomacy*>(*it))));}
    }

    // create a new list of orders and copy all orders from other player
    // delete orders;
    // orders.clear();
    // for(list<TestOrders*>::iterator it = player.orders->begin(); it != player.orders->end(); it++){
    //     list<TestOrders*>::iterator itOrders = orders->begin();
    //     orders->insert(itOrders, *it);
    //     itOrders++;
    //     // cout << (it->second->name) << endl;
    // }
}


// destructor
Player::~Player(){
    territories.erase(territories.begin(),territories.end());
    hand->cards.clear();
    delete hand;
    // orders.clear();
    delete name;
}


// assignment operator(not tested)
Player &Player::operator=(const Player &player){

    cout << "**Assignment op" << endl;
    if (this == &player) return *this;
    
    // delete the previous and copy the name from the other player
    delete this->name;
    this->name = new string();
    *(this->name) = *(player.name);

    // clear the list of territories and copy all territories from other player
    territories.erase(territories.begin(), territories.end());
    for(map<int, Territory*>::const_iterator it = player.territories.begin(); it != player.territories.end(); it++){
        this->territories.insert(pair<int, Territory*>(it->second->countryNumber, it->second));}
    
    // delete the current collection of cards and create a new list of cards and copy all cards from other player
    this->hand->cards.clear();
    delete this->hand;
    this->hand = new Hand();
    // creates a deep copy by making new objects from the other hand of cards
    for(vector<Card*>::iterator it = player.hand->cards.begin(); it != player.hand->cards.end(); it++){
        if((*it)->getType().compare("bomb")){ this->hand->cards.emplace_back(new Bomb(*(static_cast<Bomb*>(*it))));}
        else if((*it)->getType().compare("reinforcement")){ this->hand->cards.emplace_back(new Reinforcement(*(static_cast<Reinforcement*>(*it))));}
        else if((*it)->getType().compare("blockade")){ this->hand->cards.emplace_back(new Blockade(*(static_cast<Blockade*>(*it))));}
        else if((*it)->getType().compare("airlift")){ this->hand->cards.emplace_back(new Airlift(*(static_cast<Airlift*>(*it))));}
        else if((*it)->getType().compare("diplomacy")){ this->hand->cards.emplace_back(new Diplomacy(*(static_cast<Diplomacy*>(*it))));}
    }
    // orders.clear();
    // this->orders = new list<TestOrders*>;
    // for(list<TestOrders*>::iterator it = player.orders->begin(); it != player.orders->end(); it++){
    //     list<TestOrders*>::iterator itOrders = orders->begin();
    //     this->orders->insert(itOrders, *it);
    //     itOrders++;
    // }

    return *this;
}

// ostream operator
ostream& operator<<(ostream &out, const Player &player){
    out << *(player.name);
    // for(map<int, Territory*>::iterator it = player.territories->begin(); it != player.territories->end(); it++){
    //     out << it->second->name << endl;
    // }
    return out;
}

// accessor for territories
map<int, Territory*> Player::getTerritories() {
    return territories;
}


// return a list of territories that can be defended by the player
map<int, Territory*> Player::toDefend(){
    map<int, Territory*> *territoriesToDefend = new map<int, Territory*>;   // a list of territories to defend
    // put all the elements from this object's territories to the territoriesToDefend
    for(map<int,Territory*>::iterator it = territories.begin(); it != territories.end(); it++){
        territoriesToDefend->insert(pair<int, Territory*>(it->second->countryNumber, (it->second)));
    }
    return *territoriesToDefend;
}


// return a list of territories that can be attacked by the player
map<int, Territory*> Player::toAttack(list<Edge*> &edges){
    map<int, Territory*> territoriesToAttack;   // a list of territories to attack
    // put all adjacent territories of this object's territories into territoriesToAttack
    for(list<Edge*>::iterator itEdges = edges.begin(); itEdges != edges.end(); itEdges++){
        // put the destination of the edge if the source is this object's territory and the destination is not
        if(territories.find((*itEdges)->source->countryNumber) != territories.end() && territories.find((*itEdges)->destination->countryNumber) == territories.end()){
            territoriesToAttack.insert(pair<int, Territory*>((*itEdges)->destination->countryNumber, (*itEdges)->destination));
        }
        // put the source of the edge if the destination is this object's territory and the source is not
        else if(territories.find((*itEdges)->destination->countryNumber) != territories.end() && territories.find((*itEdges)->source->countryNumber) == territories.end()){
            territoriesToAttack.insert(pair<int, Territory*>((*itEdges)->source->countryNumber, (*itEdges)->source));
        }
    }
    return territoriesToAttack;
}

// inserts an order to the player's list of orders(not tested)
void Player::issueOrder(TestOrders *order){
    // orders.insert(orders->end(),order);
}