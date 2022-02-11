
#ifndef COMP345RISKGAME_ORDERS_H
#define COMP345RISKGAME_ORDERS_H

#include <string>
#include <vector>
using namespace std;

// Abstract base class for Deploy, Advance, Bomb, Blockade, Airlift and Negotiate player order actions
class Order {
public:
    Order();
    Order(const Order&);
    virtual ~Order() = default;

    Order & operator=(const Order &order);
    friend ostream & operator<<(ostream &out, const Order &order);

    virtual bool validate() = 0;
    virtual void execute() = 0;

    [[nodiscard]] virtual string className() const = 0;
    [[nodiscard]] virtual Order* clone() const = 0;
};

// Deploy action order (arbitrary behaviour for now)
class Deploy : public Order {
public:
    Deploy();
    Deploy(const Deploy &deploy);

    Deploy & operator=(const Deploy &deploy);
    friend ostream & operator<<(ostream &out, const Deploy &deploy);

    bool validate() override;
    void execute() override;

    [[nodiscard]] string className() const override;
    [[nodiscard]] Deploy * clone() const override;
};

// Advance action order (arbitrary behaviour for now)
class Advance : public Order {
public:
    Advance();
    Advance(const Advance &advance);

    Advance & operator=(const Advance &advance);
    friend ostream & operator<<(ostream &out, const Advance &advance);

    bool validate() override;
    void execute() override;

    [[nodiscard]] string className() const override;
    [[nodiscard]] Advance * clone() const override;
};

// Bomb action order (arbitrary behaviour for now)
class Bomb : public Order {
public:
    Bomb();
    Bomb(const Bomb &bomb);

    Bomb & operator=(const Bomb &bomb);
    friend ostream & operator<<(ostream &out, const Bomb &bomb);

    bool validate() override;
    void execute() override;

    [[nodiscard]] string className() const override;
    [[nodiscard]] Bomb * clone() const override;
};

// Blockade action order (arbitrary behaviour for now)
class Blockade : public Order {
public:
    Blockade();
    Blockade(const Blockade&);

    Blockade & operator=(const Blockade &blockade);
    friend ostream & operator<<(ostream &out, const Blockade &blockade);

    bool validate() override;
    void execute() override;

    [[nodiscard]] string className() const override;
    [[nodiscard]] Blockade * clone() const override;
};

// Airlift action order (arbitrary behaviour for now)
class Airlift : public Order {
public:
    Airlift();
    Airlift(const Airlift &airlift);

    Airlift & operator=(const Airlift *airlift);
    friend ostream & operator<<(ostream &out, const Airlift &airlift);

    bool validate() override;
    void execute() override;

    [[nodiscard]] string className() const override;
    [[nodiscard]] Airlift * clone() const override;
};

// Negotiate action order (arbitrary behaviour for now)
class Negotiate : public Order {
public:
    Negotiate();
    Negotiate(const Negotiate &negotiate);

    Negotiate & operator=(const Negotiate *negotiate);
    friend ostream & operator<<(ostream &out, const Negotiate &negotiate);

    bool validate() override;
    void execute() override;

    [[nodiscard]] string className() const override;
    [[nodiscard]] Negotiate * clone() const override;
};

// Represents a sequential list of orders with basic functionalities
class OrdersList {
public:
    OrdersList();
    OrdersList(const OrdersList &ordersList);
    ~OrdersList();

    OrdersList & operator=(const OrdersList &ordersList);
    friend ostream & operator<<(ostream &out, const OrdersList &ordersList);

    void remove(int index);
    void move(int oldIndex, int newIndex);

    void add(Order *newOrder);
    [[nodiscard]] size_t length() const;
    [[nodiscard]] Order * element(int index) const;


private:
    vector<Order*> orders;
};

#endif //COMP345RISKGAME_ORDERS_H
