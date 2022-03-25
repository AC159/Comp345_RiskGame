#include <iostream>
#include <cmath>
#include <utility>
#include "Orders.h"
#include "../Player/Player.h"

using namespace std;
using namespace Orders;

// ====================== Order class ======================

// sole constructor to be used by subclasses
Order::Order(Players::Player *issuer, std::string type) : issuer(issuer), type(std::move(type)) {}

// copy constructor creates shallow copy due to circular dependency
Order::Order(const Order &order) : type(order.type) {
    this->issuer = order.issuer;
    this->orderEffect = order.orderEffect;
}

// memory de-allocation should be handled externally
Order::~Order() = default;

// creates shallow copy via the assignment operator
Order &Order::operator=(const Order &order) {
    if (this != &order) {
        this->issuer = order.issuer;
        this->orderEffect = order.orderEffect;
    }
    return *this;
}

// overloads the insertion operator with a basic string representation of the object
ostream &Orders::operator<<(ostream &out, const Order &order) {
    return order.write(out);
}

// returns whether the two players are actively negotiating due to diplomacy card
bool Order::hasNegotiation(Players::Player *player1, Players::Player *player2) {
    for (auto playerName: player1->cannotAttack)
        if (playerName == player2->getName())
            return true;

    return false;
}

// ====================== Deploy class ======================
Deploy::Deploy() : Order(nullptr, "deploy"), target(nullptr), armies(0) {
    cout << "Created a Deploy order." << endl;
}

/** creates an order with all members initialized through parameters
 * @param issuer the player whose turn it is
 * @param target the territory where armies will be deployed (must belong to issuer for validity)
 * @param armies the number of armies to deploy (must be available in the issuer's reinforcement pool for validity)
 */
Deploy::Deploy(Players::Player *issuer, Graph::Territory *target, int armies) : Order(issuer, "deploy"), target(target),
                                                                                armies(armies) {}

// copy constructor creates shallow copy due to circular dependency
Deploy::Deploy(const Deploy &deploy) : Order(deploy) {
    this->target = deploy.target;
    this->armies = deploy.armies;
}

// memory de-allocation should be handled externally
Deploy::~Deploy() = default;

// returns a string to be output to the log file when notified
std::string Deploy::stringToLog() const {
    return this->orderEffect;
}

// creates shallow copy via the assignment operator
Deploy &Deploy::operator=(const Deploy &deploy) {
    if (this != &deploy) {
        this->issuer = deploy.issuer;
        this->target = deploy.target;
        this->armies = deploy.armies;
    }
    return *this;
}

// overloads the insertion operator with a basic string representation of the object
ostream &Orders::operator<<(ostream &out, const Deploy &deploy) {
    return deploy.write(out);
}

/* is valid if the target territory belongs to the player that issued the order
 * and if this player has sufficient armies in his reinforcement pool*/
bool Deploy::validate() {
    if (issuer != nullptr && target != nullptr && target->owner == issuer && armies <= issuer->reinforcementPool) {
        return true;
    }
    return false;
}

// if valid, the number of armies are taken from the player's reinforcement pool and added to their target territory
void Deploy::execute() {
    if (this->validate()) {
        this->orderEffect = "Deploy " + to_string(armies) + " to " + target->name;
        cout << this->orderEffect << endl;
        issuer->reinforcementPool -= armies;
        target->numberOfArmies += armies;
        notify(*this);
    } else {
        this->orderEffect = "An invalid Deploy order could not be executed";
        cout << this->orderEffect << endl;
        notify(*this);
    }
}

// dynamically returns a clone of the calling class
Deploy *Deploy::clone() const {
    return new Deploy(*this);
}

// helper for the stream insertion operator to function as a virtual
std::ostream &Deploy::write(ostream &out) const {
    return target == nullptr ? out << "Deploy" : out << "Deploy " << armies << " to " << target->name;
}

// ====================== Advance class ======================
Advance::Advance() : Order(nullptr, "advance"), map(nullptr), source(nullptr), target(nullptr), armies(0) {
    cout << "Created an Advance order." << endl;
}

/** creates an order with all members initialized through parameters
 * @param issuer the player whose turn it is
 * @param map the map in which the source and target are located in
 * @param source the territory where the armies will be taken from (must belong to issuer for validity)
 * @param target the territory where armies will transfer to or attack (must be adjacent to source for validity)
 * @param armies the number of armies to attack with or to transfer (must be available in source for validity)
 */
Advance::Advance(Players::Player *issuer, Graph::Map *map, Graph::Territory *source, Graph::Territory *target,
                 int armies) : Order(issuer, "advance"), map(map), source(source), target(target), armies(armies) {}

// copy constructor creates shallow copy due to circular dependency
Advance::Advance(const Advance &advance) : Order(advance) {
    this->map = advance.map;
    this->source = advance.source;
    this->target = advance.target;
    this->armies = advance.armies;
}

// memory de-allocation should be handled externally
Advance::~Advance() = default;

// returns a string to be output to the log file when notified
std::string Advance::stringToLog() const {
    return this->orderEffect;
}

// creates shallow copy via the assignment operator
Advance &Advance::operator=(const Advance &advance) {
    if (this != &advance) {
        this->map = advance.map;
        this->issuer = advance.issuer;
        this->source = advance.source;
        this->target = advance.target;
        this->armies = advance.armies;
    }
    return *this;
}

// overloads the insertion operator with a basic string representation of the object
ostream &Orders::operator<<(ostream &out, const Advance &advance) {
    return advance.write(out);
}

/* is valid if the source territory has sufficient armies & belongs to the issuing player, if the target territory
 * neighbors the source, and if the target's owner isn't negotiating with the issuer */
bool Advance::validate() {
    if (source != nullptr && source->owner == issuer && source->numberOfArmies >= armies && armies > 0 &&
        map->edgeExists(source, target)) {
        return true;
    }
    return false;
}

/* only performs action if the order is valid: if the target territory belongs to the issuer, the armies are
 * transferred there; otherwise the armies attack the target territory*/
void Advance::execute() {
    if (validate()) {
        if (source->owner == target->owner) { // perform simple army transfer
            this->orderEffect = to_string(armies) + " transferred to " + target->name + " from " + source->name;
            cout << this->orderEffect << endl;
            source->numberOfArmies -= armies;
            target->numberOfArmies += armies;
        } else { // initiate an attack
            if (hasNegotiation(issuer, target->owner)) {
                this->orderEffect = "INVALID ADVANCE ORDER: cannot advance on target player's territory; negotiation in effect";
                cout << this->orderEffect << endl;
                notify(*this);
                return;
            }

            int defendersKilled = static_cast<int>(round(armies * 0.6));
            int attackersKilled = static_cast<int>(round(target->numberOfArmies * 0.7));

            if (target->numberOfArmies > defendersKilled || armies <= attackersKilled) { // failed to conquer
                this->orderEffect = to_string(armies) + " failed to take " + target->name + " from " + source->name;
                cout << this->orderEffect << endl;

                // update territories' armies
                target->numberOfArmies -= defendersKilled;
                source->numberOfArmies -= armies < attackersKilled ? armies : attackersKilled;
            } else { // territory successfully conquered
                this->orderEffect = to_string(armies) + " captured " + target->name + " from " + source->name;
                cout << this->orderEffect << endl;
                target->transferOwnership(issuer); // issuer now owns the territory

                // update territories' armies
                source->numberOfArmies -= armies;
                target->numberOfArmies = armies - attackersKilled;

                // the conquering player receives a card as a reward at the end of the turn
                issuer->receivesCard = true;
            }
        }
        notify(*this);
    } else {
        this->orderEffect = "An invalid Advance order could not be executed";
        cout << this->orderEffect << endl;
        notify(*this);
    }
}

// dynamically returns a clone of the calling class
Advance *Advance::clone() const {
    return new Advance(*this);
}

// helper for the stream insertion operator to function as a virtual
std::ostream &Advance::write(ostream &out) const {
    return target == nullptr ?
           out << "Advance" : out << armies << " from " << source->name << " will attack/transfer " << target->name;
}

// ====================== Bomb class ======================
Bomb::Bomb() : Order(nullptr, "bomb"), target(nullptr), map(nullptr) {
    cout << "Created a Bomb order." << endl;
}

/** creates an order with all members initialized through parameters
 * @param issuer the player whose turn it is
 * @param target the territory to bomb (must be adjacent to one of issuer's territories & have diff owner for validity)
 */
Bomb::Bomb(Players::Player *issuer, Graph::Territory *target, Graph::Map *map) : Order(issuer, "bomb"), target(target),
                                                                                 map(map) {}

// copy constructor creates shallow copy due to circular dependency
Bomb::Bomb(const Bomb &bomb) : Order(bomb) {
    this->target = bomb.target;
    this->map = bomb.map;
}

// memory de-allocation should be handled externally
Bomb::~Bomb() = default;

// returns a string to be output to the log file when notified
std::string Bomb::stringToLog() const {
    return this->orderEffect;
}

// creates shallow via the assignment operator
Bomb &Bomb::operator=(const Bomb &bomb) {
    if (this != &bomb) {
        this->issuer = bomb.issuer;
        this->target = bomb.target;
    }
    return *this;
}

// overloads the insertion operator with a basic string representation of the object
ostream &Orders::operator<<(ostream &out, const Bomb &bomb) {
    return bomb.write(out);
}

// is valid if the target territory is not owned by the issuer and is adjacent to at least one of issuer's territories
bool Bomb::validate() {
    if (hasNegotiation(issuer, target->owner)) {
        cout << "INVALID ORDER: cannot bomb target player's territory; negotiation in effect" << endl;
        return false;
    }

    // checking if the target territory belongs to the issuing player
    if (this->issuer == target->owner) {
        cout << "INVALID ORDER: cannot bomb your own territory" << endl;;
        return false;
    }

    // loop to check if the target territory is adjacent to any of the issuing player's territories
    for (auto it = issuer->territories.begin();
         it != issuer->territories.end(); it++) {
        if (this->map->edgeExists(it->second, target))
            return true;
    }

    cout << "INVALID ORDER: territory is not adjacent to any of your territories" << endl;
    return false;
}

// if valid, half of the armies are removed from the target territory
void Bomb::execute() {
    if (this->validate()) {
        this->orderEffect = "Executed a Bomb Order: " + issuer->getName() + " bombs " + target->name;
        cout << this->orderEffect << endl;
        target->numberOfArmies /= 2;

        notify(*this);
    } else {
        this->orderEffect = "An invalid Bomb order could not be executed";
        cout << this->orderEffect << endl;
        notify(*this);
    }
}

// dynamically returns a clone of the calling class
Bomb *Bomb::clone() const {
    return new Bomb(*this);
}

// helper for the stream insertion operator to function as a virtual
std::ostream &Bomb::write(ostream &out) const {
    return target == nullptr ? out << "Bomb" : out << "Bomb " << target->name;
}

// ====================== Blockade class ======================
Blockade::Blockade() : Order(nullptr, "blockade"), target(nullptr) {
    cout << "Created a Blockade order." << endl;
}

/** creates an order with all members initialized through parameters
 * @param issuer the player whose turn it is
 * @param target the territory to turn into a neutral blockade (must belong to issuer for validity)
 */
Blockade::Blockade(Players::Player *issuer, Graph::Territory *target) : Order(issuer, "blockade"), target(target) {}

// copy constructor creates shallow copy due to circular dependency
Blockade::Blockade(const Blockade &blockade) : Order(blockade) {
    this->target = blockade.target;
}

// memory de-allocation should be handled externally
Blockade::~Blockade() = default;

// returns a string to be output to the log file when notified
std::string Blockade::stringToLog() const {
    return this->orderEffect;
}

// creates shallow copy via the assignment operator
Blockade &Blockade::operator=(const Blockade &blockade) {
    if (this != &blockade) {
        this->issuer = blockade.issuer;
        this->target = blockade.target;
    }
    return *this;
}

// overloads the insertion operator with a basic string representation of the object
ostream &Orders::operator<<(ostream &out, const Blockade &blockade) {
    return blockade.write(out);
}

// is valid if the target territory is owned by the issuing player
bool Blockade::validate() {
    if (target->owner != this->issuer) {
        cout << "INVALID ORDER: cannot create blockade on a territory you do not own" << endl;
        return false;
    }

    cout << "Validated a Blockade order." << endl;
    return true;
}

// if valid, the number of armies on the target territory are doubled and transferred to the neutral player
void Blockade::execute() {
    if (this->validate()) {
        this->orderEffect = "Executed a Blockade Order: " + issuer->getName() + " blockades " + target->name;
        cout << this->orderEffect << endl;
        target->numberOfArmies *= 2;
        target->transferOwnership(Players::Player::neutralPlayer);

        notify(*this);
    } else {
        this->orderEffect = "An invalid Blockade order could not be executed";
        cout << this->orderEffect << endl;
        notify(*this);
    }
}

// dynamically returns a clone of the calling class
Blockade *Blockade::clone() const {
    return new Blockade(*this);
}

// helper for the stream insertion operator to function as a virtual
std::ostream &Blockade::write(ostream &out) const {
    return target == nullptr ? out << "Blockade" : out << "Blockade " << target->name;
}

// ====================== Airlift class ======================
Airlift::Airlift() : Order(nullptr, "airlift"), source(nullptr), target(nullptr), armies(0) {
    cout << "Created an Airlift order." << endl;
}

/** creates an order with all members initialized through parameters
 * @param issuer the player whose turn it is
 * @param source the territory where the armies will be taken from (must belong to issuer for validity)
 * @param target the territory where the armies will be transferred to (must belong to issuer for validity)
 * @param armies the number of armies to transfer (must be available in source for validity)
 */
Airlift::Airlift(Players::Player *issuer, Graph::Territory *source, Graph::Territory *target, int armies)
        : Order(issuer, "airlift"), source(source), target(target), armies(armies) {}

// copy constructor creates shallow copy due to circular dependency
Airlift::Airlift(const Airlift &airlift) : Order(airlift) {
    this->source = airlift.source;
    this->target = airlift.target;
    this->armies = airlift.armies;
}

// memory de-allocation should be handled externally
Airlift::~Airlift() = default;

// returns a string to be output to the log file when notified
std::string Airlift::stringToLog() const {
    return this->orderEffect;
}

// creates shallow copy via the assignment operator
Airlift &Airlift::operator=(const Airlift &airlift) {
    if (this != &airlift) {
        this->issuer = airlift.issuer;
        this->source = airlift.source;
        this->target = airlift.target;
    }
    return *this;
}

// overloads the insertion operator with a basic string representation of the object
ostream &Orders::operator<<(ostream &out, const Airlift &airlift) {
    return airlift.write(out);
}

// is valid if both the source and target territories belong to the issuer and the source has sufficient armies
bool Airlift::validate() {
    if (source != nullptr && target != nullptr &&
        source->owner == issuer && target->owner == issuer && source->numberOfArmies >= armies) {
        return true;
    }
    return false;
}

// if the order is valid, the selected number of armies are moved from the source to the target territories
void Airlift::execute() {
    if (this->validate()) {
        this->orderEffect =
                "Executed Airlift order: " + issuer->getName() + " airlifts " + to_string(armies) + " armies from " +
                source->name + " to " + target->name;
        cout << this->orderEffect << endl;
        source->numberOfArmies -= armies;
        target->numberOfArmies += armies;
        notify(*this);
    } else {
        this->orderEffect = "An invalid Airlift order could not be executed";
        cout << this->orderEffect << endl;
        notify(*this);
    }
}

// dynamically returns a clone of the calling class
Airlift *Airlift::clone() const {
    return new Airlift(*this);
}

// helper for the stream insertion operator to function as a virtual
std::ostream &Airlift::write(ostream &out) const {
    return target == nullptr ? out << "Airlift" : out << "Airlift " << armies << " armies from " << source->name
                                                      << " to " << target->name;
}

// ====================== Negotiate class ======================
Negotiate::Negotiate() : Order(nullptr, "negotiate"), target(nullptr) {
    cout << "Created a Negotiate order." << endl;
}

/** creates an order with all members initialized through parameters
 * @param issuer the player whose turn it is
 * @param target the player to negotiate with (must not be the issuer nor the neutral player validity)
 */
Negotiate::Negotiate(Players::Player *issuer, Players::Player *target) : Order(issuer, "negotiate"), target(target) {}

// copy constructor creates shallow copy due to circular dependency
Negotiate::Negotiate(const Negotiate &negotiate) : Order(negotiate) {
    this->target = negotiate.target;
}

// memory de-allocation should be handled externally
Negotiate::~Negotiate() = default;

// returns a string to be output to the log file when notified
std::string Negotiate::stringToLog() const {
    return this->orderEffect;
}

// creates shallow copy via the assignment operator
Negotiate &Negotiate::operator=(const Negotiate &negotiate) {
    if (this != &negotiate) {
        this->issuer = negotiate.issuer;
        this->target = negotiate.target;
    }
    return *this;
}

// overloads the insertion operator with a basic string representation of the object
ostream &Orders::operator<<(ostream &out, const Negotiate &negotiate) {
    return negotiate.write(out);
}

// is valid if the target player is neither neutral nor the issuing player
bool Negotiate::validate() {
    if (issuer == target) {
        cout << "INVALID ORDER: cannot negotiate with yourself" << endl;
        return false;
    } else if (target->getName() == "neutral") {
        cout << "INVALID ORDER: cannot negotiate with neutral player" << endl;
        return false;
    }

    cout << "Validated a Negotiate order." << endl;
    return true;
}

// if valid, prevents all attacks between the issuer and the target player for the remainder of the turn
void Negotiate::execute() {
    if (this->validate()) {
        issuer->cannotAttack.push_back(target->getName());
        target->cannotAttack.push_back(issuer->getName());

        this->orderEffect = "Executed a Negotiate Order";
        cout << this->orderEffect << endl;
        notify(*this);
    } else {
        this->orderEffect = "An invalid Negotiate order could not be executed";
        cout << this->orderEffect << endl;
        notify(*this);
    }
}

// dynamically returns a clone of the calling class
Negotiate *Negotiate::clone() const {
    return new Negotiate(*this);
}

// helper for the stream insertion operator to function as a virtual
std::ostream &Negotiate::write(ostream &out) const {
    return target == nullptr ? out << "Negotiate" : out << "Negotiate with " << target->getName();
}

// ====================== OrdersList class ======================
OrdersList::OrdersList() = default;

// creates a deep copy of the given OrdersList and of all the Order objects it contains
OrdersList::OrdersList(const OrdersList &ordersList) {
    orders.reserve(ordersList.orders.size());
    for (Order *order: ordersList.orders) {
        orders.push_back(order->clone());
    }
}

// destructor deallocates memory to prevent memory leaks
OrdersList::~OrdersList() {
    for (Order *order: orders) {
        delete order;
    }
}

// returns a string to be output to the log file when notified
std::string OrdersList::stringToLog() const {
    Order *order = this->orders.back();
    return order->issuer->getName() + " has added a new " + order->type + " order to the list of orders";
}

// creates true deep copy via the assignment operator
OrdersList &OrdersList::operator=(const OrdersList &ordersList) {
    if (this != &ordersList) {
        // deallocate memory from lhs
        for (Order *order: orders) {
            delete order;
        }
        orders.clear();

        // create deep copy of rhs into lhs
        orders.reserve(ordersList.orders.size());
        for (Order *order: ordersList.orders) {
            orders.push_back(order->clone());
        }
    }
    return *this;
}

// overloads the insertion operator with a basic string representation of the object
ostream &Orders::operator<<(ostream &out, const OrdersList &ordersList) {
    out << "{" << endl;
    int i = 0;
    for (auto order: ordersList.orders) {
        out << "\t[" << i << "] = " << *order << endl;
        i++;
    }
    out << "}" << endl;
    return out;
}

// removes and deletes the element in the list located at the given index
void OrdersList::remove(int index) {
    // validate index
    if (index < 0 || index >= orders.size()) {
        return;
    }

    delete orders[index];
    orders.erase(orders.begin() + index);
}

// moves the element from the given oldIndex to the given newIndex while preserving the order of other elements
[[maybe_unused]] void OrdersList::move(int oldIndex, int newIndex) {
    // validate indices
    if (oldIndex < 0 || newIndex < 0 || oldIndex == newIndex || oldIndex >= orders.size() ||
        newIndex >= orders.size()) {
        return;
    }

    if (oldIndex < newIndex) {
        auto beginIt = orders.begin();
        rotate(beginIt + oldIndex, beginIt + oldIndex + 1, beginIt + newIndex + 1);
    } else {
        auto rendIt = orders.rend();
        rotate(rendIt - oldIndex - 1, rendIt - oldIndex, rendIt - newIndex);
    }
}

// adds an element to the back of the list
void OrdersList::add(Order *const newOrder) {
    orders.push_back(newOrder);
    notify(*this);
}

// returns the current number of elements in the list
size_t OrdersList::length() const {
    return orders.size();
}

// returns the element at the given index
Order *OrdersList::element(size_t index) const {
    // validate index
    if (index >= orders.size()) {
        return nullptr;
    }

    return orders[index];
}
