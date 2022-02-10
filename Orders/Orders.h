
#ifndef COMP345RISKGAME_ORDERS_H
#define COMP345RISKGAME_ORDERS_H

#include <list>
#include <string>
using namespace std;

class Order {
public:
    Order();
    Order(const Order&);
    Order& operator=(const Order&);
    friend ostream & operator << (ostream&, const Order&);
    friend istream & operator >> (istream&, const Order&);
    virtual bool validate() = 0;
    virtual void execute() = 0;
};

class Deploy : public Order {
public:
    Deploy();
    Deploy(const Deploy&);
    bool validate() override;
    void execute() override;
};

class Advance : public Order {
public:
    Advance();
    Advance(const Advance&);
    bool validate() override;
    void execute() override;
};

class Bomb : public Order {
public:
    Bomb();
    Bomb(const Bomb&);
    bool validate() override;
    void execute() override;
};

class Blockade : public Order {
public:
    Blockade();
    Blockade(const Blockade&);
    bool validate() override;
    void execute() override;
};

class Airlift : public Order {
public:
    Airlift();
    Airlift(const Airlift&);
    bool validate() override;
    void execute() override;
};

class Negotiate : public Order {
public:
    Negotiate();
    Negotiate(const Negotiate&);
    bool validate() override;
    void execute() override;
};

class OrdersList {
private:

public:
    OrdersList();
    OrdersList(const OrdersList&);
    OrdersList& operator=(const OrdersList&);
    friend istream & operator >> (istream&, const OrdersList&);
    friend ostream & operator << (ostream&, const OrdersList&);
    void remove(Order);
    void move(Order, int); //move up, down or at specific location?
};

#endif //COMP345RISKGAME_ORDERS_H
