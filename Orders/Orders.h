#ifndef COMP345RISKGAME_ORDERS_H
#define COMP345RISKGAME_ORDERS_H

#include <string>
#include <vector>
#include "../Map/Map.h"

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

// Abstract base class for Deploy, Advance, Bomb, Blockade, Airlift and Negotiate orders
class Orders::Order {
protected:
    Players::Player *issuer;

public:
    Order();
    explicit Order(Players::Player *issuer);
    Order(const Order &order);
    virtual ~Order();

    Order & operator=(const Order &order);
    friend std::ostream & operator<<(std::ostream &out, const Order &order);

    virtual bool validate() = 0;
    virtual void execute() = 0;

    [[nodiscard]] virtual std::string className() const = 0;
    [[nodiscard]] virtual Order* clone() const = 0;
    static void ordersDriver();
};

/* A deployment order tells a number of armies taken from the reinforcement pool to deploy to a target territory owned
 * by the player issuing this order.*/
class Orders::Deploy : public Order {
private:
    Graph::Territory *target;
    int armies;

public:
    Deploy();
    Deploy(Players::Player *issuer, Graph::Territory *target, int armies);
    Deploy(const Deploy &deploy);
    ~Deploy() override;

    Deploy & operator=(const Deploy &deploy);
    friend std::ostream & operator<<(std::ostream &out, const Deploy &deploy);

    bool validate() override;
    void execute() override;
    [[nodiscard]] std::string className() const override;
    [[nodiscard]] Deploy * clone() const override;
};

/* An advance order tells a number of army units from a source territory to transfer to or to attack a target adjacent
 * territory.*/
class Orders::Advance : public Order {
private:
    Graph::Territory *source;
    Graph::Territory *target;
    int armies;

public:
    Advance();
    Advance(Players::Player *issuer, Graph::Territory *source, Graph::Territory *target, int armies);
    Advance(const Advance &advance);
    ~Advance() override;

    Advance & operator=(const Advance &advance);
    friend std::ostream & operator<<(std::ostream &out, const Advance &advance);

    bool validate() override;
    void execute() override;
    [[nodiscard]] std::string className() const override;
    [[nodiscard]] Advance * clone() const override;
};

/* A bomb order removes half of the armies from a territory belonging to an enemy.
 * This order can only be created by playing the bomb card.*/
class Orders::Bomb : public Order {
private:
    Graph::Territory *target;

public:
    Bomb();
    Bomb(Players::Player *issuer, Graph::Territory *target);
    Bomb(const Bomb &bomb);
    ~Bomb() override;

    Bomb & operator=(const Bomb &bomb);
    friend std::ostream & operator<<(std::ostream &out, const Bomb &bomb);

    bool validate() override;
    void execute() override;
    [[nodiscard]] std::string className() const override;
    [[nodiscard]] Bomb * clone() const override;
};

/* A blockade order doubles the number of armies on a territory belonging to the issuer and transfers the ownership of
 * the territory to the Neutral player. This order can only be created by playing the blockade card.*/
class Orders::Blockade : public Order {
private:
    Graph::Territory *target;

public:
    Blockade();
    Blockade(Players::Player *issuer, Graph::Territory *target);
    Blockade(const Blockade&);
    ~Blockade() override;

    Blockade & operator=(const Blockade &blockade);
    friend std::ostream & operator<<(std::ostream &out, const Blockade &blockade);

    bool validate() override;
    void execute() override;
    [[nodiscard]] std::string className() const override;
    [[nodiscard]] Blockade * clone() const override;
};

/* An airlift order tells a number of armies taken from a source territory to be moved to a target territory.
 * This order can only be created by playing the airlift card.*/
class Orders::Airlift : public Order {
private:
    Graph::Territory *source;
    Graph::Territory *target;
    int armies;

public:
    Airlift();
    Airlift(Players::Player *issuer, Graph::Territory *source, Graph::Territory *target, int armies);
    Airlift(const Airlift &airlift);
    ~Airlift() override;

    Airlift & operator=(const Airlift &airlift);
    friend std::ostream & operator<<(std::ostream &out, const Airlift &airlift);

    bool validate() override;
    void execute() override;
    [[nodiscard]] std::string className() const override;
    [[nodiscard]] Airlift * clone() const override;
};

/* A negotiating order results in an enemy player and the player issuing the order to be unable to attack each other's
 * territories for the remainder of the turn. This order can only be created by playing the diplomacy card.*/
class Orders::Negotiate : public Order {
private:
    Players::Player *target;

public:
    Negotiate();
    Negotiate(Players::Player *issuer, Players::Player *target);
    Negotiate(const Negotiate &negotiate);
    ~Negotiate() override;

    Negotiate & operator=(const Negotiate &negotiate);
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
