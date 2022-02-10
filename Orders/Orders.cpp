
#include "Orders.h"
#include <iostream>
using namespace std;

// ====================== Order class ======================
Order::Order() {
    cout << "Created an Order base class.";
}

Order::Order(const Order &order) {
    //copy constructor is currently empty because class has no attributes yet
}

Order &Order::operator=(const Order &) {
    return *this;
}

// ====================== Deploy class ======================
Deploy::Deploy() {
    cout << "Created a Deploy order." << endl;
}

Deploy::Deploy(const Deploy &deploy) {
    //copy constructor is currently empty because class has no attributes yet
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

// ====================== Advance class ======================
Advance::Advance() {
    cout << "Created an Advance order." << endl;
}

Advance::Advance(const Advance &advance) {
    //copy constructor is currently empty because class has no attributes yet
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

// ====================== Bomb class ======================
Bomb::Bomb() {
    cout << "Created a Bomb order." << endl;
}

Bomb::Bomb(const Bomb &bomb) {
    //copy constructor is currently empty because class has no attributes yet
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

// ====================== Blockade class ======================
Blockade::Blockade() {
    cout << "Created a Blockade order." << endl;
}

Blockade::Blockade(const Blockade &blockade) {
    //copy constructor is currently empty because class has no attributes yet
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

// ====================== Airlift class ======================
Airlift::Airlift() {
    cout << "Created an Airlift order." << endl;
}

Airlift::Airlift(const Airlift &airlift) {
    //copy constructor is currently empty because class has no attributes yet
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

// ====================== Negotiate class ======================
Negotiate::Negotiate() {
    cout << "Created a Negotiate order." << endl;
}

Negotiate::Negotiate(const Negotiate &negotiate) {
    //copy constructor is currently empty because class has no attributes yet
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

// ====================== OrdersList class ======================
OrdersList::OrdersList() {

}

OrdersList::OrdersList(const OrdersList &ordersList) {

}

OrdersList &OrdersList::operator=(const OrdersList &ordersList) {
    return *this;
}

void OrdersList::remove(Order) {

}

void OrdersList::move(Order, int) {

}
