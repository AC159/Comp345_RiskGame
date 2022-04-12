#ifndef COMP345RISKGAME_ORDERS_H
#define COMP345RISKGAME_ORDERS_H

#include <string>
#include <vector>
#include "../Map/Map.h"
#include "../Logging/LoggingObserver.h"

namespace Orders {
    class Order;
    class Deploy;
    class Advance;
    class Bomb;
    class Blockade;
    class Airlift;
    class Negotiate;
    class OrdersList;
    std::ostream & operator<<(std::ostream &out, const Order &order);
    std::ostream & operator<<(std::ostream &out, const Deploy &deploy);
    std::ostream & operator<<(std::ostream &out, const Advance &advance);
    std::ostream & operator<<(std::ostream &out, const Bomb &bomb);
    std::ostream & operator<<(std::ostream &out, const Blockade &blockade);
    std::ostream & operator<<(std::ostream &out, const Airlift &airlift);
    std::ostream & operator<<(std::ostream &out, const Negotiate &negotiate);
    std::ostream & operator<<(std::ostream &out, const OrdersList &ordersList);
}

// Abstract base class for Deploy, Advance, Bomb, Blockade, Airlift and Negotiate orders
class Orders::Order : public ILoggable, public Subject {
public:
    Players::Player *issuer;    //the player whose turn it is to issue orders
    std::string orderEffect;
    const std::string type;

    virtual ~Order();

    Order & operator=(const Order &order);
    friend std::ostream & operator<<(std::ostream &out, const Order &order);

    virtual bool validate() = 0;
    virtual void execute() = 0;
    [[nodiscard]] std::string stringToLog() const override = 0;

    [[nodiscard]] virtual Order* clone() const = 0;
    virtual std::ostream & write(std::ostream &out) const = 0;
    [[nodiscard]] virtual std::string toString() const = 0;
    virtual void displayStats(bool beforeExecution) const = 0;
    bool hasNegotiation(Players::Player* player1, Players::Player* player2);

protected:
    Order(Players::Player *issuer, std::string type);
    Order(const Order &order);
};

/* A deployment order tells a number of armies taken from the reinforcement pool to deploy to a target territory owned
 * by the player issuing this order.*/
class Orders::Deploy : public Order {
public:
    Graph::Territory *target;       //the territory where armies will be deployed
    int armies;                     //the number of armies to deploy
    const bool isReinforcementCard; //true if this order was issued using a reinforcement card

    Deploy();
    Deploy(Players::Player *issuer, Graph::Territory *target, int armies);
    Deploy(Players::Player *issuer, Graph::Territory *target, int armies, bool reinforcement);
    Deploy(const Deploy &deploy);
    ~Deploy() override;

    Deploy & operator=(const Deploy &deploy);
    friend std::ostream & operator<<(std::ostream &out, const Deploy &deploy);

    [[nodiscard]] std::string stringToLog() const override;
    bool validate() override;
    void execute() override;

    [[nodiscard]] Deploy * clone() const override;
    std::ostream & write(std::ostream &out) const override;
    [[nodiscard]] std::string toString() const override;
    void displayStats(bool beforeExecution) const override;
};

/* An advance order tells a number of army units from a source territory to transfer to or to attack a target adjacent
 * territory.*/
class Orders::Advance : public Order {
public:
    Graph::Map *map;            //the map in which the source and target are located in
    Graph::Territory *source;   //the territory where the armies will be taken from
    Graph::Territory *target;   //the territory where armies will transfer to or attack
    int armies;                 //the number of armies to attack with or to transfer

    Advance();
    Advance(Players::Player *issuer, Graph::Map *map, Graph::Territory *source, Graph::Territory *target, int armies);
    Advance(const Advance &advance);
    ~Advance() override;

    Advance & operator=(const Advance &advance);
    friend std::ostream & operator<<(std::ostream &out, const Advance &advance);

    [[nodiscard]] std::string stringToLog() const override;
    bool validate() override;
    void execute() override;

    [[nodiscard]] Advance * clone() const override;
    std::ostream & write(std::ostream &out) const override;
    [[nodiscard]] std::string toString() const override;
    void displayStats(bool beforeExecution) const override;
};

/* A bomb order removes half of the armies from a territory belonging to an enemy.
 * This order can only be created by playing the bomb card.*/
class Orders::Bomb : public Order {
public:
    Graph::Territory *target;   //the territory to bomb
    Graph::Map *map;

    Bomb();
    Bomb(Players::Player *issuer, Graph::Territory *target, Graph::Map *map);
    Bomb(const Bomb &bomb);
    ~Bomb() override;

    Bomb & operator=(const Bomb &bomb);
    friend std::ostream & operator<<(std::ostream &out, const Bomb &bomb);

    [[nodiscard]] std::string stringToLog() const override;
    bool validate() override;
    void execute() override;

    [[nodiscard]] Bomb * clone() const override;
    std::ostream & write(std::ostream &out) const override;
    [[nodiscard]] std::string toString() const override;
    void displayStats(bool beforeExecution) const override;
};

/* A blockade order doubles the number of armies on a territory belonging to the issuer and transfers the ownership of
 * the territory to the Neutral player. This order can only be created by playing the blockade card.*/
class Orders::Blockade : public Order {
public:
    Graph::Territory *target;                //the territory to turn into a neutral blockade
    std::vector<Players::Player *> &players; //the list of players currently playing the game

    Blockade(Players::Player *issuer, Graph::Territory *target, std::vector<Players::Player *> &players);
    Blockade(const Blockade&);
    ~Blockade() override;

    Blockade & operator=(const Blockade &blockade);
    friend std::ostream & operator<<(std::ostream &out, const Blockade &blockade);

    [[nodiscard]] std::string stringToLog() const override;
    bool validate() override;
    void execute() override;

    [[nodiscard]] Blockade * clone() const override;
    std::ostream & write(std::ostream &out) const override;
    [[nodiscard]] std::string toString() const override;
    void displayStats(bool beforeExecution) const override;

private:
    static int newNeutralId;
};

/* An airlift order tells a number of armies taken from a source territory to be moved to a target territory.
 * This order can only be created by playing the airlift card.*/
class Orders::Airlift : public Order {
public:
    Graph::Territory *source;   //the territory where the armies will be taken from
    Graph::Territory *target;   //the territory where the armies will be airlifted to
    int armies;                 //the number of armies to airlift

    Airlift();
    Airlift(Players::Player *issuer, Graph::Territory *source, Graph::Territory *target, int armies);
    Airlift(const Airlift &airlift);
    ~Airlift() override;

    Airlift & operator=(const Airlift &airlift);
    friend std::ostream & operator<<(std::ostream &out, const Airlift &airlift);

    [[nodiscard]] std::string stringToLog() const override;
    bool validate() override;
    void execute() override;

    [[nodiscard]] Airlift * clone() const override;
    std::ostream & write(std::ostream &out) const override;
    [[nodiscard]] std::string toString() const override;
    void displayStats(bool beforeExecution) const override;
};

/* A negotiating order results in an enemy player and the player issuing the order to be unable to attack each other's
 * territories for the remainder of the turn. This order can only be created by playing the diplomacy card.*/
class Orders::Negotiate : public Order {
public:
    Players::Player *target;    //the negotiating counterparty

    Negotiate();
    Negotiate(Players::Player *issuer, Players::Player *target);
    Negotiate(const Negotiate &negotiate);
    ~Negotiate() override;

    Negotiate & operator=(const Negotiate &negotiate);
    friend std::ostream & operator<<(std::ostream &out, const Negotiate &negotiate);

    [[nodiscard]] std::string stringToLog() const override;
    bool validate() override;
    void execute() override;

    [[nodiscard]] Negotiate * clone() const override;
    std::ostream & write(std::ostream &out) const override;
    [[nodiscard]] std::string toString() const override;
    void displayStats(bool beforeExecution) const override;
};

// Represents a sequential list of orders with basic functionalities
class Orders::OrdersList : public ILoggable, public Subject {
private:
    std::vector<Order*> orders;

public:
    OrdersList();
    OrdersList(const OrdersList &ordersList);
    ~OrdersList();

    OrdersList & operator=(const OrdersList &ordersList);
    friend std::ostream & operator<<(std::ostream &out, const OrdersList &ordersList);

    [[nodiscard]] std::string stringToLog() const override;
    void remove(int index);

    [[maybe_unused]] void move(int oldIndex, int newIndex);
    void add(Order *newOrder);
    [[nodiscard]] size_t length() const;
    [[nodiscard]] Order * element(size_t index) const;
};

#endif //COMP345RISKGAME_ORDERS_H
