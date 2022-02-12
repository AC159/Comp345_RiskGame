
#include "Orders.h"
#include <iostream>
using namespace std;

using namespace Orders;

// ====================== Order class ======================
Order::Order() {
    cout << "Created an Order base class. ";
}

//copy constructor is currently empty because class has no attributes yet
Order::Order(const Order &order) = default;

//creates deep copy via the assignment operator (currently no attributes to copy)
Order & Order::operator=(const Order &) = default;

//overloads the insertion operator with a basic string representation of the object
ostream & operator<<(ostream &out, const Order &order) {
    out << "Order object";
    return out;
}

// ====================== Deploy class ======================
Deploy::Deploy() {
    cout << "Created a Deploy order." << endl;
}

Deploy::Deploy(const Deploy &deploy) {
    //copy constructor is currently empty because class has no attributes yet
}

//creates deep copy via the assignment operator (currently no attributes to copy)
Deploy & Deploy::operator=(const Deploy &deploy) {
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

//polymorphycally returns a clone of the calling class
Deploy * Deploy::clone() const {
    return new Deploy(*this);
}

// ====================== Advance class ======================
Advance::Advance() {
    cout << "Created an Advance order." << endl;
}

Advance::Advance(const Advance &advance) {
    //copy constructor is currently empty because class has no attributes yet
}

//creates deep copy via the assignment operator (currently no attributes to copy)
Advance &Advance::operator=(const Advance &advance) {
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

//polymorphycally returns a clone of the calling class
Advance * Advance::clone() const {
    return new Advance(*this);
}

// ====================== Bomb class ======================
Bomb::Bomb() {
    cout << "Created a Bomb order." << endl;
}

Bomb::Bomb(const Bomb &bomb) {
    //copy constructor is currently empty because class has no attributes yet
}

//creates deep copy via the assignment operator (currently no attributes to copy)
Bomb &Bomb::operator=(const Bomb &bomb) {
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

//polymorphycally returns a clone of the calling class
Bomb * Bomb::clone() const {
    return new Bomb(*this);
}

// ====================== Blockade class ======================
Blockade::Blockade() {
    cout << "Created a Blockade order." << endl;
}

Blockade::Blockade(const Blockade &blockade) {
    //copy constructor is currently empty because class has no attributes yet
}

//creates deep copy via the assignment operator (currently no attributes to copy)
Blockade &Blockade::operator=(const Blockade &blockade) {
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

//polymorphycally returns a clone of the calling class
Blockade * Blockade::clone() const {
    return new Blockade(*this);
}

// ====================== Airlift class ======================
Airlift::Airlift() {
    cout << "Created an Airlift order." << endl;
}

Airlift::Airlift(const Airlift &airlift) {
    //copy constructor is currently empty because class has no attributes yet
}

//creates deep copy via the assignment operator (currently no attributes to copy)
Airlift &Airlift::operator=(const Airlift *airlift) {
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
Negotiate::Negotiate() {
    cout << "Created a Negotiate order." << endl;
}

Negotiate::Negotiate(const Negotiate &negotiate) {
    //copy constructor is currently empty because class has no attributes yet
}

//creates deep copy via the assignment operator (currently no attributes to copy)
Negotiate & Negotiate::operator=(const Negotiate *negotiate) {
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
OrdersList::OrdersList() {
    cout << "Created an OrdersList object." << endl;
}

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
