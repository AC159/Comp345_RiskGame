
#include "Orders.h"
#include <iostream>
using namespace std;

// TODO: add more comments and remove dead code
// ====================== Order class ======================
Order::Order() {
    cout << "Created an Order base class. ";
}

Order::Order(const Order &order) {
    //copy constructor is currently empty because class has no attributes yet
}

Order & Order::operator=(const Order &) {
    return *this;
}

ostream & operator<<(ostream &out, const Order &order) {
    out << "Order object";
    return out;
}

istream & operator>>(istream &in, const Order &order) {
    return in;
}

// ====================== Deploy class ======================
Deploy::Deploy() {
    cout << "Created a Deploy order." << endl;
}

Deploy::Deploy(const Deploy &deploy) {
    //copy constructor is currently empty because class has no attributes yet
}

Deploy & Deploy::operator=(const Deploy &deploy) {
    return *this;
}

ostream & operator<<(ostream &out, const Deploy &deploy) {
    out << "Deploy order object";
    return out;
}

istream & operator>>(istream &in, const Deploy &deploy) {
    return in;
}

bool Deploy::validate() {
    cout << "Validated a Deploy order." << endl;
    return true;
}

void Deploy::execute() {
    if (this->validate()) {
        cout << "Executed a Deploy Order." << endl;
    } else {
        cout << "Invalid order could not be executed." << endl;
    }
}

string Deploy::className() {
    return "Deploy";
}

// ====================== Advance class ======================
Advance::Advance() {
    cout << "Created an Advance order." << endl;
}

Advance::Advance(const Advance &advance) {
    //copy constructor is currently empty because class has no attributes yet
}

Advance &Advance::operator=(const Advance &advance) {
    return *this;
}

ostream & operator<<(ostream &out, const Advance &advance) {
    out << "Advance order object";
    return out;
}

istream & operator>>(istream &in, const Advance &advance) {
    return in;
}

bool Advance::validate() {
    cout << "Validated an Advance order." << endl;
    return true;
}

void Advance::execute() {
    if (this->validate()) {
        cout << "Executed an Advance Order." << endl;
    } else {
        cout << "Invalid order could not be executed." << endl;
    }
}

string Advance::className() {
    return "Advance";
}

// ====================== Bomb class ======================
Bomb::Bomb() {
    cout << "Created a Bomb order." << endl;
}

Bomb::Bomb(const Bomb &bomb) {
    //copy constructor is currently empty because class has no attributes yet
}

Bomb &Bomb::operator=(const Bomb &bomb) {
    return *this;
}

ostream & operator<<(ostream &out, const Bomb &bomb) {
    out << "Bomb order object";
    return out;
}

istream & operator>>(istream &in, const Bomb &bomb) {
    return in;
}

bool Bomb::validate() {
    cout << "Validated a Bomb order." << endl;
    return true;
}

void Bomb::execute() {
    if (this->validate()) {
        cout << "Executed a Bomb Order." << endl;
    } else {
        cout << "Invalid order could not be executed." << endl;
    }
}

string Bomb::className() {
    return "Bomb";
}

// ====================== Blockade class ======================
Blockade::Blockade() {
    cout << "Created a Blockade order." << endl;
}

Blockade::Blockade(const Blockade &blockade) {
    //copy constructor is currently empty because class has no attributes yet
}

Blockade &Blockade::operator=(const Blockade &blockade) {
    return *this;
}

ostream & operator<<(ostream &out, const Blockade &blockade) {
    out << "Blockade order object";
    return out;
}

istream & operator>>(istream &in, const Blockade &blockade) {
    return in;
}

bool Blockade::validate() {
    cout << "Validated a Blockade order." << endl;
    return true;
}

void Blockade::execute() {
    if (this->validate()) {
        cout << "Executed a Blockade Order." << endl;
    } else {
        cout << "Invalid order could not be executed." << endl;
    }
}

string Blockade::className() {
    return "Blockade";
}

// ====================== Airlift class ======================
Airlift::Airlift() {
    cout << "Created an Airlift order." << endl;
}

Airlift::Airlift(const Airlift &airlift) {
    //copy constructor is currently empty because class has no attributes yet
}

Airlift &Airlift::operator=(const Airlift *airlift) {
    return *this;
}

ostream & operator<<(ostream &out, const Airlift &airlift) {
    out << "Airlift order object";
    return out;
}

istream & operator>>(istream &in, const Airlift &airlift) {
    return in;
}

bool Airlift::validate() {
    cout << "Validated an Airlift order." << endl;
    return true;
}

void Airlift::execute() {
    if (this->validate()) {
        cout << "Executed an Airlift Order." << endl;
    } else {
        cout << "Invalid order could not be executed." << endl;
    }
}

string Airlift::className() {
    return "Airlift";
}

// ====================== Negotiate class ======================
Negotiate::Negotiate() {
    cout << "Created a Negotiate order." << endl;
}

Negotiate::Negotiate(const Negotiate &negotiate) {
    //copy constructor is currently empty because class has no attributes yet
}

Negotiate & Negotiate::operator=(const Negotiate *negotiate) {
    return *this;
}

ostream &operator<<(ostream &out, const Negotiate &negotiate) {
    out << "Negotiate order object";
    return out;
}

istream &operator>>(istream &in, const Negotiate &negotiate) {
    return in;
}

bool Negotiate::validate() {
    cout << "Validated a Negotiate order." << endl;
    return true;
}

void Negotiate::execute() {
    if (this->validate()) {
        cout << "Executed a Negotiate Order." << endl;
    } else {
        cout << "Invalid order could not be executed." << endl;
    }
}

string Negotiate::className() {
    return "Negotiate";
}

// ====================== OrdersList class ======================
OrdersList::OrdersList() {
    cout << "Created an OrdersList object." << endl;
}

OrdersList::OrdersList(const OrdersList &ordersList) {
    // TODO: figure out why vector elements don't copy and how to fix
    // deep copy:
//    orders.reserve(ordersList.orders.size());
//    for (Order *order: orders) {
//        orders.push_back(new Order(*order))); //this doesn't work either, also Order is abstract
//    }
    // shallow copy
    cout << "copy constructor";
    for (Order *order: orders) {
        this->orders.push_back(order);
    }
}

OrdersList::~OrdersList() {
    for (Order *order : orders) {
        delete order;
    }
}

OrdersList & OrdersList::operator=(const OrdersList &ordersList) {
    // TODO: figure out why vector elements don't copy and how to fix
    // shallow copy
    cout << "assignemtn operatr";
    for (Order *order: orders) {
        this->orders.push_back(order);
    }
    return *this;
}

ostream & operator<<(ostream &out, const OrdersList &ordersList) {
    out << "OrderList object:\torders {";
    if (ordersList.orders.size() > 0) {
        out << "[0]=" << ordersList.orders[0]->className();
        for (int i = 1; i < ordersList.orders.size(); i++) {
            out << ", [" << i << "]=" << ordersList.orders[i]->className();
        }
    }
    out << "}" << endl;
    return out;
}

istream & operator>>(istream &in, const OrdersList &ordersList) {
    return in;
}

void OrdersList::add(Order *const newOrder) {
    this->orders.push_back(newOrder);
}

void OrdersList::remove(int index) {
    delete orders[index];
    orders.erase(orders.begin() + index);
}

void OrdersList::move(int oldIndex, int newIndex) {
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
