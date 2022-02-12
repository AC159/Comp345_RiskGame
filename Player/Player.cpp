#include <iostream>
#include <list>
#include "Player.h"

using namespace Cards;
using namespace Orders;
using namespace Players;

// default constructor
Player::Player(){
    name = new string("No name");
    hand = new Hand();
    orders = new Orders::OrdersList();
}


//constructor for Player class to initialize the name and the collection of territories, cards, and orders
Player::Player(string &newName){
    name = &newName;
    hand = new Hand();
    orders = new Orders::OrdersList();
}


// copy constructor(not tested)
Player::Player(const Player &player){

    cout << "**Copy constructor" << endl;

    // initialize and copy the name from the other player
    this->name = new string();
    *(this->name) = *(player.name);

    // create a new mapping of territories and copy all territories from other player
    for(map<int, Territory*>::const_iterator it = player.territories.begin(); it != player.territories.end(); it++){
        territories.insert(pair<int, Territory*>(it->second->countryNumber, it->second));
    }

    // create a new list of cards and use copy constructor of Hand class to copy the hand of the other player
    this->hand = new Hand(*(player.hand));

    // create a new list of orders and use copy constructor to copy orders from other player
    this->orders = new Orders::OrdersList(*(player.orders));
}


// destructor
Player::~Player(){
    territories.erase(territories.begin(),territories.end()); // remove all territories on player's list of territories but not destroy them
}


// assignment operator
Player &Player::operator=(const Player &player){

    if (this == &player) return *this;
    
    // delete the previous and copy the name from the other player
    delete this->name;
    this->name = new string();
    *(this->name) = *(player.name);

    // clear the list of territories and copy all territories from other player
    territories.erase(territories.begin(), territories.end());
    for(map<int, Territory*>::const_iterator it = player.territories.begin(); it != player.territories.end(); it++){
        this->territories.insert(pair<int, Territory*>(it->second->countryNumber, it->second));
    }
    
    // delete the current collection of cards and use 
    this->hand->cards.clear();
    delete this->hand;
    this->hand = new Hand(*(player.hand));

    // delete the currect list of orders, create a new orderslist then issue all orders from the other player
    delete this->orders;
    this->orders = new Orders::OrdersList(*(player.orders));

    return *this;
}


// ostream operator displays the player's terrritories, hand, and orders
ostream& operator<<(ostream &out, const Player &player){
    out << "Player " << *(player.name) << "'s" << endl;
    out << "territories: " << endl;
    for(map<int, Territory*>::const_iterator it = player.territories.begin(); it != player.territories.end(); it++){
        out << "\t" << it->second->name << endl;
    }
    out << "hand: " << endl;
    for(vector<Card*>::const_iterator it = player.hand->cards.begin(); it != player.hand->cards.end(); it++){
        out << "\t" << (*it)->getType() << endl;
    }
    out << "orders: " << endl;
    out << "\t" << *player.orders;
    return out;
}


// return a list of territories that can be defended by the player
map<int, Territory*> Player::toDefend(){
    map<int, Territory*> *territoriesToDefend = new map<int, Territory*>;   // a list of territories to defend
    // put all the elements from this object's territories to the territoriesToDefend
    for(map<int, Territory*>::iterator it = territories.begin(); it != territories.end(); it++){
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


// inserts an order to the player's list of orders
// orderType: deploy, advance, bomb, blockade, airlift
void Player::issueOrder(string orderType){
    Order *newOrder;
    if(orderType.compare("deploy")){ newOrder = new Orders::Deploy(); }
    else if(orderType.compare("advance")){ newOrder = new Orders::Advance(); }
    else if(orderType.compare("bomb")){ newOrder = new Orders::Bomb(); }
    else if(orderType.compare("blockade")){ newOrder = new Orders::Blockade(); }
    else if(orderType.compare("airlift")){ newOrder = new Orders::Airlift(); }
    orders->add(newOrder);
}


// acessor method for name
string Player::getName(){
    return *name;
}


// mutator method for name
void Player::setName(string newName){
    delete name;
    *name = newName;
}

// displays player's territories
void Player::displayTerritories(){
    cout << *name << "'s territories: " << endl;
    for(map<int, Territory*>::const_iterator it = territories.begin(); it != territories.end(); it++){
        cout << "\t" << it->second->name << endl;
    }
}


// add territory to player's territories
void Player::addTerritory(Territory &territory){
    territories.insert(pair<int, Territory*>(territory.countryNumber, &territory));
}


// remove territory from player's territories
void Player::removeTerritory(Graph::Territory &territory){
    map<int, Graph::Territory*>::iterator it = territories.find(territory.countryNumber);
    // notifies user that player does not own territory if player does not own it
    if(it == territories.end()){
        cout << *name << " does not own " << territory.name << "." << endl;
    }
    // otherwise, remove it
    else{
        territories.erase(it);
    }
}


// display player's cards
void Player::displayCards(){
    cout << *name << "'s hand:";
    for(vector<Card*>::const_iterator it = hand->cards.begin(); it != hand->cards.end(); it++){
        cout << "\t" << (*it)->getType() << endl;
    }
}


// add a card to player's hand
void Player::addCard(Cards::Card *card){
    hand->cards.emplace_back(card);
}


// remove a card from player's hand
void Player::removeCard(Cards::Card &card){
    for(vector<Cards::Card*>::iterator it = hand->cards.begin(); it != hand->cards.end(); it++){
        if(*it == &card){
            hand->cards.erase(it);
            return;
        }
    }
    cout << *name << "'s does not own a " << card.getType() << " card." << endl;
}


// display player's orders
void Player::displayOrders(){
    cout << *name << "'s orders:";
    for(int i = 0; i < orders->length(); i++){
        cout << "\t" << orders->element(i) << endl;
    }
}