#include <iostream>
#include <list>
#include <map>
#include <utility>
#include "Player.h"
#include "../PlayerStrategies/PlayerStrategies.h"

using namespace std;
using namespace Players;
using namespace Graph;

// default constructor
Player::Player() {
    name = "No name";
    hand = new Cards::Hand();
    orders = new Orders::OrdersList();
    reinforcementPool = 0;
    receivesCard = false;
    isEliminated = false;
}


// constructor for Player class to initialize the name and the collection of territories, cards, and orders
Player::Player(const string& newName) {
    name = newName;
    hand = new Cards::Hand();
    orders = new Orders::OrdersList();
    reinforcementPool = 0;
    receivesCard = false;
    isEliminated = false;
}


Player::Player(const Player &player) {
    this->name = player.name;
    this->reinforcementPool = player.reinforcementPool;
    this->receivesCard = player.receivesCard;
    this->isEliminated = player.isEliminated;

    // create a new mapping of territories and copy all territories from other player
    for (map<int, Territory *>::const_iterator it = player.territories.begin(); it != player.territories.end(); it++) {
        territories.insert(pair<int, Territory *>(it->second->countryNumber, it->second));
    }

    // create a new list of cards and use copy constructor of Hand class to copy the hand of the other player
    this->hand = new Cards::Hand(*(player.hand));

    // create a new list of orders and use copy constructor to copy orders from other player
    this->orders = new Orders::OrdersList(*(player.orders));
}


// destructor
Player::~Player() {
    // remove all territories on player's list of territories but not destroy them
    territories.erase(territories.begin(), territories.end());
    delete orders;
    delete hand;
    delete ps;
}


// assignment operator
Player &Player::operator=(const Player &player) {

    if (this == &player) return *this;

    // delete the previous and copy the name from the other player
    this->name = player.name;
    this->reinforcementPool = player.reinforcementPool;
    this->receivesCard = player.receivesCard;

    // clear the list of territories and copy all territories from other player
    territories.clear();
    for (auto territory: player.territories) {
        this->territories.insert(pair<int, Territory *>(territory.second->countryNumber, territory.second));
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
ostream &Players::operator<<(ostream &out, const Player &player) {
    out << "Player " << player.name << endl;
    out << "territories: " << endl;
    for (auto territory: player.territories) {
        out << "\t" << territory.second->name << endl;
    }
    out << "hand: " << endl;
    out << *player.hand;

    out << "orders: " << endl;
    out << "\t" << *player.orders;
    return out;
}

/**
 * @param mapEdges the list of edges from the map used in the current game
 * @return map of territories to defend, ordered by priority
 *     detailed explanation:
 *     map values hold the territories to defend
 *         - all territories owned by this player which have at least one adjacent enemy territory are contained
 *     map keys correspond to the difference between the value's armies and armies on adjacent enemy territories
 *     map is ordered in descending order such that territories with more adjacent enemy armies are defended first
 */
std::multimap<int, Territory *> Player::toDefend(const vector<Edge *> &mapEdges) const {
    return this->ps->toDefend(mapEdges);
}

// return a list of territories that can be attacked by the player in order of priority
multimap<int, Territory *> Player::toAttack(const vector<Edge *> &edges) const {
    return this->ps->toAttack(edges);
}

// creates all orders for the player's turn and places them in the player's list of orders
void Player::issueOrder(GameEngine &game) const {
    return this->ps->issueOrder(game);
}

// accessor method for name
string Player::getName() const {
    return name;
}

// displays player's territories
void Player::displayTerritories() {
    cout << name << "'s territories: " << endl;
    for (map<int, Territory *>::const_iterator it = territories.begin(); it != territories.end(); it++) {
        cout << "\t" << it->second->name << endl;
    }
}


// add territory to player's territories
void Player::addTerritory(Territory &territory) {
    territories.insert(pair<int, Territory *>(territory.countryNumber, &territory));
}


// remove territory from player's territories
void Player::removeTerritory(Territory &territory) {
    map<int, Territory *>::iterator it = territories.find(territory.countryNumber);
    // notifies user that player does not own territory if player does not own it
    if (it == territories.end()) {
        cout << name << " does not own " << territory.name << "." << endl;
    }
        // otherwise, remove it
    else {
        territories.erase(it);
    }
}


// display player's cards
void Player::displayCards() {
    if (hand->cards.empty()) {
        cout << name << "'s hand is empty." << endl;
        return;
    }
    cout << name << "'s hand:" << endl;
    for (int i = 0; i < hand->cards.size(); i++) {
        cout << " [" << i << "] " << hand->cards[i]->getType() << endl;
    }
}


// add a card to player's hand
void Player::addCard(Cards::Card &card) {
    hand->cards.emplace_back(&card);
}


// remove a card from player's hand
void Player::removeCard(Cards::Card &card) {
    for (vector<Cards::Card *>::iterator it = hand->cards.begin(); it != hand->cards.end(); it++) {
        if (*it == &card) {
            hand->cards.erase(it);
            return;
        }
    }
    cout << name << " does not own a " << card.getType() << " card." << endl;
}


// display player's orders
void Player::displayOrders() {
    cout << name << "'s orders:" << endl;
    cout << "\t" << *orders;
}

// returns the list contents of cannotAttack
string Player::cannotAttackString() const {
    string contents = name + "'s 'cannotAttack' list size = " + to_string(cannotAttack.size()) + " contents = {";
    string delimiter;
    for (const auto &playerName: cannotAttack) {
        contents += delimiter + playerName;
        delimiter = ", ";
    }
    return contents += "}\n";
}
