#include <iostream>
#include "Orders.h"
#include "../Player/Player.h"

using namespace std;
using namespace Orders;

// ====================== Order class ======================
Order::Order() : issuer(nullptr) {}

//creates an order with all members initialized through parameters
Order::Order(Players::Player *issuer) : issuer(issuer) {}

//copy constructor creates shallow copy due to circular dependency
Order::Order(const Order &order) {
    this->issuer = order.issuer;
}

//destructor prevents memory leaks
Order::~Order() {
    delete issuer;
}

//creates shallow copy via the assignment operator
Order & Order::operator=(const Order &order) {
    if (this != &order) {
        //delete or no?... circular dependencies
        this->issuer = order.issuer;
    }
    return *this;
}

//overloads the insertion operator with a basic string representation of the object
ostream & operator<<(ostream &out, const Order &order) {
    out << "Order object";
    return out;
}

// ====================== Deploy class ======================
Deploy::Deploy() : target(nullptr), armies(0) {
    cout << "Created a Deploy order." << endl;
}

//creates an order with all members initialized through parameters
Deploy::Deploy(Players::Player *issuer, Graph::Territory *target, int armies)
    : Order(issuer), target(target), armies(armies) {}

//copy constructor creates shallow copy due to circular dependency
Deploy::Deploy(const Deploy &deploy)  : Order(deploy) {
    this->target = deploy.target;
    this->armies = deploy.armies;
}

//destructor prevents memory leaks
Deploy::~Deploy() {
    delete target;
}

//creates shallow copy via the assignment operator
Deploy & Deploy::operator=(const Deploy &deploy) {
    if (this != &deploy) {
        //delete or no?... circular dependencies
        this->issuer = deploy.issuer;
        this->target = deploy.target;
        this->armies = deploy.armies;
    }
    return *this;
}

//overloads the insertion operator with a basic string representation of the object
ostream & operator<<(ostream &out, const Deploy &deploy) {
    out << "Deploy order object";
    return out;
}

//returns whether the order is valid - behaviour is arbitrary for now
bool Deploy::validate() {
    cout << "Validated a Deploy order.";
    return true;
}

//performs the order action if it's valid - action is arbitrary for now
void Deploy::execute() {
    if (this->validate()) {
        cout << " Executed a Deploy Order." << endl;
    } else {
        cout << "Invalid order could not be executed." << endl;
    }
}

//polymorphically returns the name of the calling class
string Deploy::className() const {
    return "Deploy";
}

//polymorphically returns a clone of the calling class
Deploy * Deploy::clone() const {
    return new Deploy(*this);
}

// ====================== Advance class ======================
Advance::Advance() : source(nullptr), target(nullptr), armies(0) {
    cout << "Created an Advance order." << endl;
}

/** creates an order with all members initialized through parameters
 * @param issuer the player whose turn it is
 * @param source the territory where the armies will be taken from (must belong to issuer for validity)
 * @param target the where armies will transfer to or attack (must be adjacent to source for validity)
 * @param armies the number of armies to attack with or to transfer (must be <= armies in source for validity)
 */
Advance::Advance(Players::Player *issuer, Graph::Territory *source, Graph::Territory *target, int armies)
    : Order(issuer), source(source), target(target), armies(armies) {}

//copy constructor creates shallow copy due to circular dependency
Advance::Advance(const Advance &advance) : Order(advance) {
    this->source = advance.source;
    this->target = advance.target;
    this->armies = advance.armies;
}

//destructor prevents memory leaks
Advance::~Advance() {
    delete source;
    delete target;
}

//creates shallow copy via the assignment operator
Advance &Advance::operator=(const Advance &advance) {
    if (this != &advance) {
        //delete or no?... circular dependencies
        this->issuer = advance.issuer;
        this->source = advance.source;
        this->target = advance.target;
        this->armies = advance.armies;
    }
    return *this;
}

//overloads the insertion operator with a basic string representation of the object
ostream & operator<<(ostream &out, const Advance &advance) {
    out << "Advance order object";
    return out;
}

/* is valid if the source territory belongs to the player that issued the order and has sufficient armies,
 * and if the target territory is adjacent to the source territory*/
bool Advance::validate() {
    //TODO: make sense of adjacent territories - determined in map or player or new function?
    if (source != nullptr && source->owner == issuer && source->numberOfArmies >= armies && armies > 0 /*& adjacent*/) {
        return true;
    }
    return false;
}

/* only performs action if the order is valid: if the target territory belongs to the issuer, the armies are
 * transferred there; otherwise the armies attack the target territory*/
void Advance::execute() {
    //TODO: test the execution of multiple advance orders for verification
    if (validate()) {
        if (source->owner == target->owner) { //perform simple army transfer
            source->numberOfArmies -= armies;
            target->numberOfArmies += armies;
        } else { //initiate an attack
            //TODO: skip attack if source & target owners are negotiating due to diplomacy card
            int defendersKilled = armies * 60 / 100;
            int attackersKilled = target->numberOfArmies * 70 / 100;

            if (target->numberOfArmies >= defendersKilled) { //attacking armies failed to conquer the territory
                //update territories' armies
                target->numberOfArmies -= defendersKilled;
                armies < attackersKilled ? source->numberOfArmies -= armies : source->numberOfArmies -= attackersKilled;
            } else { //territory successfully conquered
                //transfer ownership
                target->owner->removeTerritory(*target);
                target->owner = issuer;
                issuer->addTerritory(*target);

                //update territories' armies
                source->numberOfArmies -= armies;
                target->numberOfArmies = armies - attackersKilled;

                //the conquering player receives a card as a reward
                //TODO: check if should add deck attribute to advance order or if deck's attribute should be static
                /*issuer->hand->cards.push_back(Cards::Deck::draw());*/ //should I convert to static or??
            }
        }
    } else {
        cout << "An invalid advance order could not be executed." << endl;
    }
}

//polymorphically returns the name of the calling class
string Advance::className() const {
    return "Advance";
}

//polymorphically returns a clone of the calling class
Advance * Advance::clone() const {
    return new Advance(*this);
}

// ====================== Bomb class ======================
Bomb::Bomb() : target(nullptr) {
    cout << "Created a Bomb order." << endl;
}

//creates an order with all members initialized through parameters
Bomb::Bomb(Players::Player *issuer, Graph::Territory *target) : Order(issuer), target(target) {}

//copy constructor creates shallow copy due to circular dependency
Bomb::Bomb(const Bomb &bomb) : Order(bomb) {
    this->target = bomb.target;
}

//destructor prevents memory leaks
Bomb::~Bomb() {
    delete target;
}

//creates shallow via the assignment operator
Bomb &Bomb::operator=(const Bomb &bomb) {
    if (this != &bomb) {
        //delete or no?... circular dependencies
        this->issuer = bomb.issuer;
        this->target = bomb.target;
    }
    return *this;
}

//overloads the insertion operator with a basic string representation of the object
ostream & operator<<(ostream &out, const Bomb &bomb) {
    out << "Bomb order object";
    return out;
}

//returns whether the order is valid - behaviour is arbitrary for now
bool Bomb::validate() {
    cout << "Validated a Bomb order.";
    return true;
}

//performs the order action if it's valid - action is arbitrary for now
void Bomb::execute() {
    if (this->validate()) {
        cout << " Executed a Bomb order." << endl;
    } else {
        cout << "Invalid order could not be executed." << endl;
    }
}

//polymorphically returns the name of the calling class
string Bomb::className() const {
    return "Bomb";
}

//polymorphically returns a clone of the calling class
Bomb * Bomb::clone() const {
    return new Bomb(*this);
}

// ====================== Blockade class ======================
Blockade::Blockade() : target(nullptr) {
    cout << "Created a Blockade order." << endl;
}

//creates an order with all members initialized through parameters
Blockade::Blockade(Players::Player *issuer, Graph::Territory *target) : Order(issuer), target(target) {}

//copy constructor creates shallow copy due to circular dependency
Blockade::Blockade(const Blockade &blockade) : Order(blockade) {
    this->target = blockade.target;
}

//destructor prevents memory leaks
Blockade::~Blockade() {
    delete target;
}

//creates shallow copy via the assignment operator
Blockade &Blockade::operator=(const Blockade &blockade) {
    if (this != &blockade) {
        //delete or no?... circular dependencies
        this->issuer = blockade.issuer;
        this->target = blockade.target;
    }
    return *this;
}

//overloads the insertion operator with a basic string representation of the object
ostream & operator<<(ostream &out, const Blockade &blockade) {
    out << "Blockade order object";
    return out;
}

//returns whether the order is valid - behaviour is arbitrary for now
bool Blockade::validate() {
    cout << "Validated a Blockade order.";
    return true;
}

//performs the order action if it's valid - action is arbitrary for now
void Blockade::execute() {
    if (this->validate()) {
        cout << " Executed a Blockade order." << endl;
    } else {
        cout << "Invalid order could not be executed." << endl;
    }
}

//polymorphically returns the name of the calling class
string Blockade::className() const {
    return "Blockade";
}

//polymorphically returns a clone of the calling class
Blockade * Blockade::clone() const {
    return new Blockade(*this);
}

// ====================== Airlift class ======================
Airlift::Airlift() : source(nullptr), target(nullptr), armies(0){
    cout << "Created an Airlift order." << endl;
}

/** creates an order with all members initialized through parameters
 * @param issuer the player whose turn it is
 * @param source the territory where the armies will be taken from (must belong to issuer for validity)
 * @param target the territory where the armies will be transferred to (must belong to issuer for validity)
 * @param armies the number of armies to transfer (must be <= armies in source for validity)
 */
Airlift::Airlift(Players::Player *issuer, Graph::Territory *source, Graph::Territory *target, int armies)
    : Order(issuer), source(source), target(target), armies(armies) {}

//copy constructor creates shallow copy due to circular dependency
Airlift::Airlift(const Airlift &airlift) : Order(airlift) {
    this->source = airlift.source;
    this->target = airlift.target;
    this->armies = airlift.armies;
}

//destructor prevents memory leaks
Airlift::~Airlift() {
    delete source;
    delete target;
}

//creates shallow copy via the assignment operator
Airlift &Airlift::operator=(const Airlift &airlift) {
    if (this != &airlift) {
        //delete or no?... circular dependencies
        this->issuer = airlift.issuer;
        this->source = airlift.source;
        this->target = airlift.target;
    }
    return *this;
}

//overloads the insertion operator with a basic string representation of the object
ostream & operator<<(ostream &out, const Airlift &airlift) {
    out << "Airlift order object";
    return out;
}

//is valid if both the source and target territories belong to the issuer and the source has sufficient armies
bool Airlift::validate() {
    if (source != nullptr && target != nullptr &&
        source->owner == issuer && target->owner == issuer && source->numberOfArmies >= armies) {
        return true;
    }
    return false;
}

//if the order is valid, the selected number of armies are moved from the source to the target territories
void Airlift::execute() {
    if (this->validate()) {
        source->numberOfArmies -= armies;
        target->numberOfArmies += armies;
    } else {
        cout << "An invalid airlift order could not be executed." << endl;
    }
}

//polymorphically returns the name of the calling class
string Airlift::className() const {
    return "Airlift";
}

//polymorphically returns a clone of the calling class
Airlift * Airlift::clone() const {
    return new Airlift(*this);
}

// ====================== Negotiate class ======================
Negotiate::Negotiate() : target(nullptr) {
    cout << "Created a Negotiate order." << endl;
}

//creates an order with all members initialized through parameters
Negotiate::Negotiate(Players::Player *issuer, Players::Player *target) : Order(issuer), target(target) {}

//copy constructor creates shallow copy due to circular dependency
Negotiate::Negotiate(const Negotiate &negotiate) : Order(negotiate) {
    this->target = negotiate.target;
}

//destructor prevents memory leaks
Negotiate::~Negotiate() {
    delete target;
}

//creates shallow copy via the assignment operator
Negotiate & Negotiate::operator=(const Negotiate &negotiate) {
    if (this != &negotiate) {
        //delete or no?... circular dependencies
        this->issuer = negotiate.issuer;
        this->target = negotiate.target;
    }
    return *this;
}

//overloads the insertion operator with a basic string representation of the object
ostream &operator<<(ostream &out, const Negotiate &negotiate) {
    out << "Negotiate order object";
    return out;
}

//returns whether the order is valid - behaviour is arbitrary for now
bool Negotiate::validate() {
    cout << "Validated a Negotiate order.";
    return true;
}

//performs the order action if it's valid - action is arbitrary for now
void Negotiate::execute() {
    if (this->validate()) {
        cout << " Executed a Negotiate Order." << endl;
    } else {
        cout << "Invalid order could not be executed." << endl;
    }
}

//polymorphically returns the name of the calling class
string Negotiate::className() const {
    return "Negotiate";
}

//polymorphycally returns a clone of the calling class
Negotiate * Negotiate::clone() const {
    return new Negotiate(*this);
}

// ====================== OrdersList class ======================
OrdersList::OrdersList() = default;

//creates a deep copy of the given OrdersList and of all the Order objects it contains
OrdersList::OrdersList(const OrdersList &ordersList) {
    orders.reserve(ordersList.orders.size());
    for (Order *order: ordersList.orders) {
        orders.push_back(order->clone());
    }
}

//destructor deallocates memory to prevent memory leaks
OrdersList::~OrdersList() {
    for (Order *order : orders) {
        delete order;
    }
}

//creates true deep copy via the assignment operator
OrdersList & OrdersList::operator=(const OrdersList &ordersList) {
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

//overloads the insertion operator with a basic string representation of the object
ostream& Orders::operator<<(ostream &out, const OrdersList &ordersList) {
    out << "{";
    if (!ordersList.orders.empty()) {
        out << "[0]=" << ordersList.orders[0]->className();
        for (int i = 1; i < ordersList.orders.size(); i++) {
            out << ", [" << i << "]=" << ordersList.orders[i]->className();
        }
    }
    out << "}" << endl;
    return out;
}

//removes the element in the list located at the given index
void OrdersList::remove(int index) {
    //validate index
    if (index < 0 || index >= orders.size()) {
        return;
    }

    delete orders[index];
    orders.erase(orders.begin() + index);
}

//moves the element from the given oldIndex to the given newIndex while preserving the order of other elements
void OrdersList::move(int oldIndex, int newIndex) {
    //validate indices
    if (oldIndex < 0 || newIndex < 0 || oldIndex == newIndex || oldIndex >= orders.size() || newIndex >= orders.size()) {
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

//adds an element to the back of the list
void OrdersList::add(Order *const newOrder) {
    orders.push_back(newOrder);
}

//returns the current number of elements in the list
size_t OrdersList::length() const{
    return orders.size();
}

//returns the element at the given index
Order * OrdersList::element(size_t index) const {
    //validate index
    if (index >= orders.size()) {
        return nullptr;
    }

    return orders[index];
}
