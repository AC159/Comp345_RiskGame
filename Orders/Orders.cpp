#include <iostream>
#include "Orders.h"
#include "../Player/Player.h"

using namespace std;
using namespace Orders;

// ====================== Order class ======================
Order::Order() : issuer(nullptr) {
    cout << "Created an Order base class. ";
}

//creates an order with all members initialized through parameters
Order::Order(Players::Player *issuer) : issuer(issuer) {}

//copy constructor creates deep copy of given object
Order::Order(const Order &order) {
    this->issuer = new Players::Player(*(order.issuer));
}

//destructor prevents memory leaks
Order::~Order() {
    delete issuer;
}

//creates deep copy via the assignment operator
Order & Order::operator=(const Order &order) {
    if (this != &order) {
        delete issuer;
        this->issuer = new Players::Player(*(order.issuer));
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

//copy constructor creates deep copy of given object
Deploy::Deploy(const Deploy &deploy)  : Order(deploy) {
    this->target = new Graph::Territory(*(deploy.target));
    this->armies = deploy.armies;
}

//destructor prevents memory leaks
Deploy::~Deploy() {
    delete target;
}

//creates deep copy via the assignment operator
Deploy & Deploy::operator=(const Deploy &deploy) {
    if (this != &deploy) {
        delete issuer;
        delete target;
        this->issuer = new Players::Player(*(deploy.issuer));
        this->target = new Graph::Territory(*(deploy.target));
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

//creates an order with all members initialized through parameters
Advance::Advance(Players::Player *issuer, Graph::Territory *source, Graph::Territory *target, int armies)
    : Order(issuer), source(source), target(target), armies(armies) {}

//copy constructor creates deep copy of given object
Advance::Advance(const Advance &advance) : Order(advance) {
    this->source = new Graph::Territory(*(advance.source));
    this->target = new Graph::Territory(*(advance.target));
    this->armies = advance.armies;
}

//destructor prevents memory leaks
Advance::~Advance() {
    delete source;
    delete target;
}

//creates deep copy via the assignment operator
Advance &Advance::operator=(const Advance &advance) {
    if (this != &advance) {
        delete issuer;
        delete source;
        delete target;
        this->issuer = new Players::Player(*(advance.issuer));
        this->source = new Graph::Territory(*(advance.source));
        this->target = new Graph::Territory(*(advance.target));
        this->armies = advance.armies;
    }
    return *this;
}

//overloads the insertion operator with a basic string representation of the object
ostream & operator<<(ostream &out, const Advance &advance) {
    out << "Advance order object";
    return out;
}

//returns whether the order is valid - behaviour is arbitrary for now
bool Advance::validate() {
    cout << "Validated an Advance order.";
    return true;
}

//performs the order action if it's valid - action is arbitrary for now
void Advance::execute() {
    if (this->validate()) {
        cout << " Executed an Advance Order." << endl;
    } else {
        cout << "Invalid order could not be executed." << endl;
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

//copy constructor creates deep copy of given object
Bomb::Bomb(const Bomb &bomb) : Order(bomb) {
    this->target = new Graph::Territory(*(bomb.target));
}

//destructor prevents memory leaks
Bomb::~Bomb() {
    delete target;
}

//creates deep copy via the assignment operator
Bomb &Bomb::operator=(const Bomb &bomb) {
    if (this != &bomb) {
        delete issuer;
        delete target;
        this->issuer = new Players::Player(*(bomb.issuer));
        this->target = new Graph::Territory(*(bomb.target));
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

//copy constructor creates deep copy of given object
Blockade::Blockade(const Blockade &blockade) : Order(blockade) {
    this->target = new Graph::Territory(*(blockade.target));
}

//destructor prevents memory leaks
Blockade::~Blockade() {
    delete target;
}

//creates deep copy via the assignment operator
Blockade &Blockade::operator=(const Blockade &blockade) {
    if (this != &blockade) {
        delete issuer;
        delete target;
        this->issuer = new Players::Player(*(blockade.issuer));
        this->target = new Graph::Territory(*(blockade.target));
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

//creates an order with all members initialized through parameters
Airlift::Airlift(Players::Player *issuer, Graph::Territory *source, Graph::Territory *target, int armies)
    : Order(issuer), source(source), target(target), armies(armies) {}

//copy constructor creates deep copy of given object
Airlift::Airlift(const Airlift &airlift) : Order(airlift) {
    this->source = new Graph::Territory(*(airlift.source));
    this->target = new Graph::Territory(*(airlift.target));
    this->armies = airlift.armies;
}

//destructor prevents memory leaks
Airlift::~Airlift() {
    delete source;
    delete target;
}

//creates deep copy via the assignment operator
Airlift &Airlift::operator=(const Airlift &airlift) {
    if (this != &airlift) {
        delete issuer;
        delete source;
        delete target;
        this->issuer = new Players::Player(*(airlift.issuer));
        this->source = new Graph::Territory(*(airlift.source));
        this->target = new Graph::Territory(*(airlift.target));
    }
    return *this;
}

//overloads the insertion operator with a basic string representation of the object
ostream & operator<<(ostream &out, const Airlift &airlift) {
    out << "Airlift order object";
    return out;
}

//returns whether the order is valid - behaviour is arbitrary for now
bool Airlift::validate() {
    cout << "Validated an Airlift order.";
    return true;
}

//performs the order action if it's valid - action is arbitrary for now
void Airlift::execute() {
    if (this->validate()) {
        cout << " Executed an Airlift Order." << endl;
    } else {
        cout << "Invalid order could not be executed." << endl;
    }
}

//polymorphically returns the name of the calling class
string Airlift::className() const {
    return "Airlift";
}

//polymorphycally returns a clone of the calling class
Airlift * Airlift::clone() const {
    return new Airlift(*this);
}

// ====================== Negotiate class ======================
Negotiate::Negotiate() : target(nullptr) {
    cout << "Created a Negotiate order." << endl;
}

//creates an order with all members initialized through parameters
Negotiate::Negotiate(Players::Player *issuer, Players::Player *target) : Order(issuer), target(target) {}

//copy constructor creates deep copy of given object
Negotiate::Negotiate(const Negotiate &negotiate) : Order(negotiate) {
    this->target = new Players::Player(*(negotiate.target));
}

//destructor prevents memory leaks
Negotiate::~Negotiate() {
    delete target;
}

//creates deep copy via the assignment operator
Negotiate & Negotiate::operator=(const Negotiate &negotiate) {
    if (this != &negotiate) {
        delete issuer;
        delete target;
        this->issuer = new Players::Player(*(negotiate.issuer));
        this->target = new Players::Player(*(negotiate.target));
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
    if (index >= orders.size()) {
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
