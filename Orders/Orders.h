
#ifndef COMP345RISKGAME_ORDERS_H
#define COMP345RISKGAME_ORDERS_H

#include <string>
#include <vector>
using namespace std;

class Order {
public:
    Order();
    Order(const Order&);
    Order & operator=(const Order &order);
    virtual ~Order() = default;
    friend ostream & operator<<(ostream &out, const Order &order);
    friend istream & operator>>(istream &in, const Order &order);
    virtual bool validate() = 0;
    virtual void execute() = 0;
    virtual string className() = 0;
    [[nodiscard]] virtual Order* clone() const = 0;
};

class Deploy : public Order {
public:
    Deploy();
    Deploy(const Deploy &deploy);
    Deploy & operator=(const Deploy &deploy);
    friend ostream & operator<<(ostream &out, const Deploy &deploy);
    friend istream & operator>>(istream &in, const Deploy &deploy);
    bool validate() override;
    void execute() override;
    string className() override;
    [[nodiscard]] Deploy * clone() const override;
};

class Advance : public Order {
public:
    Advance();
    Advance(const Advance &advance);
    Advance & operator=(const Advance &advance);
    friend ostream & operator<<(ostream &out, const Advance &advance);
    friend istream & operator>>(istream &in, const Advance &advance);
    bool validate() override;
    void execute() override;
    string className() override;
    [[nodiscard]] Advance * clone() const override;
};

class Bomb : public Order {
public:
    Bomb();
    Bomb(const Bomb &bomb);
    Bomb & operator=(const Bomb &bomb);
    friend ostream & operator<<(ostream &out, const Bomb &bomb);
    friend istream & operator>>(istream &in, const Bomb &bomb);
    bool validate() override;
    void execute() override;
    string className() override;
    [[nodiscard]] Bomb * clone() const override;
};

class Blockade : public Order {
public:
    Blockade();
    Blockade(const Blockade&);
    Blockade & operator=(const Blockade &blockade);
    friend ostream & operator<<(ostream &out, const Blockade &blockade);
    friend istream & operator>>(istream &in, const Blockade &blockade);
    bool validate() override;
    void execute() override;
    string className() override;
    [[nodiscard]] Blockade * clone() const override;
};

class Airlift : public Order {
public:
    Airlift();
    Airlift(const Airlift &airlift);
    Airlift & operator=(const Airlift *airlift);
    friend ostream & operator<<(ostream &out, const Airlift &airlift);
    friend istream & operator>>(istream &in, const Airlift &airlift);
    bool validate() override;
    void execute() override;
    string className() override;
    [[nodiscard]] Airlift * clone() const override;
};

class Negotiate : public Order {
public:
    Negotiate();
    Negotiate(const Negotiate &negotiate);
    Negotiate & operator=(const Negotiate *negotiate);
    friend ostream & operator<<(ostream &out, const Negotiate &negotiate);
    friend istream & operator>>(istream &in, const Negotiate &negotiate);
    bool validate() override;
    void execute() override;
    string className() override;
    [[nodiscard]] Negotiate * clone() const override;
};

class OrdersList {
public:
    OrdersList();
    OrdersList(const OrdersList &ordersList);
    ~OrdersList();
    OrdersList & operator=(const OrdersList &ordersList);
    friend ostream & operator<<(ostream &out, const OrdersList &ordersList);
    friend istream & operator>>(istream &in, const OrdersList &ordersList);
    void add(Order *newOrder);
    void remove(int index);
    void move(int oldIndex, int newIndex);
private:
    vector<Order*> orders;
};

#endif //COMP345RISKGAME_ORDERS_H
