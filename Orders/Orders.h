
#ifndef COMP345RISKGAME_ORDERS_H
#define COMP345RISKGAME_ORDERS_H

#include <string>
#include <vector>

namespace Orders {
    class Order;
    class Deploy;
    class Advance;
    class Bomb;
    class Blockade;
    class Airlift;
    class Negotiate;
    class OrdersList;
}

// Abstract base class for Deploy, Advance, Bomb, Blockade, Airlift and Negotiate player order actions
class Orders::Order {
public:
    Order();
    Order(const Order&);
    virtual ~Order() = default;
    static void ordersDriver();

    Order & operator=(const Order &order);
    friend std::ostream & operator<<(std::ostream &out, const Order &order);

    virtual bool validate() = 0;
    virtual void execute() = 0;

    [[nodiscard]] virtual std::string className() const = 0;
    [[nodiscard]] virtual Order* clone() const = 0;
};

// Deploy action order (arbitrary behaviour for now)
class Orders::Deploy : public Order {
public:
    Deploy();
    Deploy(const Deploy &deploy);

    Deploy & operator=(const Deploy &deploy);
    friend std::ostream & operator<<(std::ostream &out, const Deploy &deploy);

    bool validate() override;
    void execute() override;

    [[nodiscard]] std::string className() const override;
    [[nodiscard]] Deploy * clone() const override;
};

// Advance action order (arbitrary behaviour for now)
class Orders::Advance : public Order {
public:
    Advance();
    Advance(const Advance &advance);

    Advance & operator=(const Advance &advance);
    friend std::ostream & operator<<(std::ostream &out, const Advance &advance);

    bool validate() override;
    void execute() override;

    [[nodiscard]] std::string className() const override;
    [[nodiscard]] Advance * clone() const override;
};

// Bomb action order (arbitrary behaviour for now)
class Orders::Bomb : public Order {
public:
    Bomb();
    Bomb(const Bomb &bomb);

    Bomb & operator=(const Bomb &bomb);
    friend std::ostream & operator<<(std::ostream &out, const Bomb &bomb);

    bool validate() override;
    void execute() override;

    [[nodiscard]] std::string className() const override;
    [[nodiscard]] Bomb * clone() const override;
};

// Blockade action order (arbitrary behaviour for now)
class Orders::Blockade : public Order {
public:
    Blockade();
    Blockade(const Blockade&);

    Blockade & operator=(const Blockade &blockade);
    friend std::ostream & operator<<(std::ostream &out, const Blockade &blockade);

    bool validate() override;
    void execute() override;

    [[nodiscard]] std::string className() const override;
    [[nodiscard]] Blockade * clone() const override;
};

// Airlift action order (arbitrary behaviour for now)
class Orders::Airlift : public Order {
public:
    Airlift();
    Airlift(const Airlift &airlift);

    Airlift & operator=(const Airlift *airlift);
    friend std::ostream & operator<<(std::ostream &out, const Airlift &airlift);

    bool validate() override;
    void execute() override;

    [[nodiscard]] std::string className() const override;
    [[nodiscard]] Airlift * clone() const override;
};

// Negotiate action order (arbitrary behaviour for now)
class Orders::Negotiate : public Order {
public:
    Negotiate();
    Negotiate(const Negotiate &negotiate);

    Negotiate & operator=(const Negotiate *negotiate);
    friend std::ostream & operator<<(std::ostream &out, const Negotiate &negotiate);

    bool validate() override;
    void execute() override;

    [[nodiscard]] std::string className() const override;
    [[nodiscard]] Negotiate * clone() const override;
};

// Represents a sequential list of orders with basic functionalities
class Orders::OrdersList {
private:
    std::vector<Order*> orders;

public:
    OrdersList();
    OrdersList(const OrdersList &ordersList);
    ~OrdersList();

    OrdersList & operator=(const OrdersList &ordersList);
    friend std::ostream & operator<<(std::ostream &out, const OrdersList &ordersList);

    void remove(int index);
    void move(int oldIndex, int newIndex);

    void add(Order *newOrder);
    [[nodiscard]] size_t length() const;
    [[nodiscard]] Order * element(size_t index) const;
};

#endif //COMP345RISKGAME_ORDERS_H
