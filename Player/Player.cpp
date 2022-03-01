#include <iostream>
#include <list>
#include <map>
#include <utility>
#include "Player.h"

using namespace std;
using namespace Players;
using namespace Graph;

// default constructor
Player::Player(){
    name = "No name";
    hand = new Cards::Hand();
    orders = new Orders::OrdersList();
    armyReinforcementPool = 0;
}


// constructor for Player class to initialize the name and the collection of territories, cards, and orders
Player::Player(string newName){
    name = std::move(newName);
    hand = new Cards::Hand();
    orders = new Orders::OrdersList();
}


Player::Player(const Player &player){
    this->name = player.name;

    // create a new mapping of territories and copy all territories from other player
    for(map<int, Territory*>::const_iterator it = player.territories.begin(); it != player.territories.end(); it++){
        territories.insert(pair<int, Territory*>(it->second->countryNumber, it->second));
    }

    // create a new list of cards and use copy constructor of Hand class to copy the hand of the other player
    this->hand = new Cards::Hand(*(player.hand));

    // create a new list of orders and use copy constructor to copy orders from other player
    this->orders = new Orders::OrdersList(*(player.orders));
}


// destructor
Player::~Player(){
    territories.erase(territories.begin(),territories.end()); // remove all territories on player's list of territories but not destroy them
    delete orders;
    delete hand;
}


// assignment operator
Player &Player::operator=(const Player &player){

    if (this == &player) return *this;
    
    // delete the previous and copy the name from the other player
    this->name = player.name;

    // clear the list of territories and copy all territories from other player
    territories.clear();
    for(auto territory : player.territories){
        this->territories.insert(pair<int, Territory*>(territory.second->countryNumber, territory.second));
    }
    
    // delete the current Hand and create a deep copy of the other player's hand object
    delete this->hand;
    this->hand = new Cards::Hand(*player.hand);

    // delete the current list of orders, create a new ordersList then issue all orders from the other player
    delete this->orders;
    this->orders = new Orders::OrdersList(*player.orders);
    return *this;
}


// ostream operator displays the player's territories, hand, and orders
ostream& Players::operator<<(ostream &out, const Player &player){
    out << "Player " << player.name << endl;
    out << "territories: " << endl;
    for(auto territory : player.territories){
        out << "\t" << territory.second->name << endl;
    }
    out << "hand: " << endl;
    out << *player.hand;

    out << "orders: " << endl;
    out << "\t" << *player.orders;
    return out;
}


// return a list of territories that can be defended by the player
map<int, Territory*> Player::toDefend(){
    map<int, Territory*> territoriesToDefend;   // a list of territories to defend
    // put all the elements from this object's territories to the territoriesToDefend
    for(map<int, Territory*>::iterator it = territories.begin(); it != territories.end(); it++){
        territoriesToDefend.insert(pair<int, Territory*>(it->second->countryNumber, (it->second)));
    }
    return territoriesToDefend;
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


// inserts an order to the player's list of orders
// orderType: deploy, advance, bomb, blockade, airlift, negotiate
void Player::issueOrder(const string& orderType) const {
    Orders::Order *newOrder;
    if(orderType == "deploy"){ newOrder = new Orders::Deploy(); }
    else if(orderType == "advance"){ newOrder = new Orders::Advance(); }
    else if(orderType == "bomb"){ newOrder = new Orders::Bomb(); }
    else if(orderType == "blockade"){ newOrder = new Orders::Blockade(); }
    else if(orderType == "airlift"){ newOrder = new Orders::Airlift(); }
    else if(orderType == "negotiate"){ newOrder = new Orders::Negotiate(); }
    orders->add(newOrder);
}


// accessor method for name
string Player::getName(){
    return name;
}


// mutator method for name
void Player::setName(string newName){
    name = std::move(newName);
}

// displays player's territories
void Player::displayTerritories(){
    cout << name << "'s territories: " << endl;
    for(map<int, Territory*>::const_iterator it = territories.begin(); it != territories.end(); it++){
        cout << "\t" << it->second->name << endl;
    }
}


// add territory to player's territories
void Player::addTerritory(Territory &territory){
    territories.insert(pair<int, Territory*>(territory.countryNumber, &territory));
}


// remove territory from player's territories
void Player::removeTerritory(Territory &territory){
    map<int, Territory*>::iterator it = territories.find(territory.countryNumber);
    // notifies user that player does not own territory if player does not own it
    if(it == territories.end()){
        cout << name << " does not own " << territory.name << "." << endl;
    }
    // otherwise, remove it
    else{
        territories.erase(it);
    }
}


// display player's cards
void Player::displayCards(){
    cout << name << "'s hand:" << endl;
    for(vector<Cards::Card*>::const_iterator it = hand->cards.begin(); it != hand->cards.end(); it++){
        cout << "\t" << (*it)->getType() << endl;
    }
}


// add a card to player's hand
void Player::addCard(Cards::Card &card){
    hand->cards.emplace_back(&card);
}


// remove a card from player's hand
void Player::removeCard(Cards::Card &card){
    for(vector<Cards::Card*>::iterator it = hand->cards.begin(); it != hand->cards.end(); it++){
        if(*it == &card){
            hand->cards.erase(it);
            return;
        }
    }
    cout << name << " does not own a " << card.getType() << " card." << endl;
}


// display player's orders
void Player::displayOrders(){
    cout << name << "'s orders:" << endl;
    cout << "\t" << *orders;
}