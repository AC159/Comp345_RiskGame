#include <iostream>
#include <cmath>
#include <utility>
#include "Orders.h"
#include "../Player/Player.h"
#include "../PlayerStrategies/PlayerStrategies.h"

using namespace std;
using namespace Orders;

// ====================== Order class ======================

// sole constructor to be used by subclasses
Order::Order(Players::Player *issuer, std::string type) : issuer(issuer), type(std::move(type)) {}

// copy constructor creates shallow copy due to circular dependency
Order::Order(const Order &order) : type(order.type) {
    this->issuer = order.issuer;
    this->orderEffect = order.orderEffect;
}

// memory de-allocation should be handled externally
Order::~Order() = default;

// creates shallow copy via the assignment operator
Order &Order::operator=(const Order &order) {
    if (this != &order) {
        this->issuer = order.issuer;
        this->orderEffect = order.orderEffect;
    }
    return *this;
}

// overloads the insertion operator with a basic string representation of the object
ostream &Orders::operator<<(ostream &out, const Order &order) {
    return order.write(out);
}

// returns whether the two players are actively negotiating due to diplomacy card
bool Order::hasNegotiation(Players::Player *player1, Players::Player *player2) {
    for (auto playerName: player1->cannotAttack)
        if (playerName == player2->getName())
            return true;

    return false;
}

// ====================== Deploy class ======================
Deploy::Deploy() : Order(nullptr, "deploy"), target(nullptr), armies(0), isReinforcementCard(false) {
    cout << "Created a Deploy order." << endl;
}

/** creates an order with all members initialized through parameters
 * @param issuer the player whose turn it is
 * @param target the territory where armies will be deployed (must belong to issuer for validity)
 * @param armies the number of armies to deploy (must be available in the issuer's reinforcement pool for validity)
 */
Deploy::Deploy(Players::Player *issuer, Graph::Territory *target, int armies)
        : Order(issuer, "deploy"), target(target), armies(armies), isReinforcementCard(false) {}

/** constructor to be used by reinforcement card
 * @param issuer the player whose turn it is
 * @param target the territory where armies will be deployed (must belong to issuer for validity)
 * @param armies the number of armies to deploy (must be available in the issuer's reinforcement pool for validity)
 * @param isReinforcementCard true if a reinforcement card was played to create this order
 */
Deploy::Deploy(Players::Player *issuer, Graph::Territory *target, int armies, bool isReinforcementCard)
        : Order(issuer, "deploy"), target(target), armies(armies), isReinforcementCard(isReinforcementCard) {}


// copy constructor creates shallow copy due to circular dependency
Deploy::Deploy(const Deploy &deploy) : Order(deploy), isReinforcementCard(deploy.isReinforcementCard) {
    this->target = deploy.target;
    this->armies = deploy.armies;
}

// memory de-allocation should be handled externally
Deploy::~Deploy() = default;

// returns a string to be output to the log file when notified
std::string Deploy::stringToLog() const {
    return this->orderEffect;
}

// creates shallow copy via the assignment operator
Deploy &Deploy::operator=(const Deploy &deploy) {
    if (this != &deploy) {
        this->issuer = deploy.issuer;
        this->target = deploy.target;
        this->armies = deploy.armies;
    }
    return *this;
}

// overloads the insertion operator with a basic string representation of the object
ostream &Orders::operator<<(ostream &out, const Deploy &deploy) {
    return deploy.write(out);
}

/* is valid if the target territory belongs to the player that issued the order
 * and if this player has sufficient armies in his reinforcement pool*/
bool Deploy::validate() {
    if (issuer == nullptr || target == nullptr || armies < 1) { // prevent runtime errors
        orderEffect = "at least one of the data members have not been properly initialized";
    } else if (target->owner != issuer) {
        orderEffect = target->name + " is not owned by " + issuer->getName();
    } else if (armies > issuer->reinforcementPool) {
        orderEffect = issuer->getName() + " has insufficient armies";
    } else {
        return true;
    }
    orderEffect = "Deploy canceled — " + orderEffect;
    return false;
}

// if valid, the number of armies are taken from the player's reinforcement pool and added to their target territory
void Deploy::execute() {
    if (validate()) {
        orderEffect = toString();
        issuer->reinforcementPool -= armies;
        target->numberOfArmies += armies;
    }
    orderEffect = (issuer == nullptr ? "" : issuer->getName()) + " executed: " + orderEffect;
    cout << orderEffect << endl;
    notify(*this);
}

// dynamically returns a clone of the calling class
Deploy *Deploy::clone() const {
    return new Deploy(*this);
}

// helper for the stream insertion operator to function dynamically
std::ostream &Deploy::write(ostream &out) const {
    return out << toString();
}

// returns a string representation of the order
std::string Deploy::toString() const {
    return target == nullptr ? "Deploy" : "Deploy " + to_string(armies) + " to " + target->name;
}

// prints the current state of the territory affected by the order
void Deploy::displayStats(bool beforeExecution) const {
    string title;
    if (beforeExecution) {
        title = "Stats before execution > ";
        cout << "Validation info        > " << target->nameAndOwner();
    } else {
        title = "Stats after execution  < ";
    }
    cout << title << target->nameAndArmies();
}

// ====================== Advance class ======================
Advance::Advance() : Order(nullptr, "advance"), map(nullptr), source(nullptr), target(nullptr), armies(0) {
    cout << "Created an Advance order." << endl;
}

/** creates an order with all members initialized through parameters
 * @param issuer the player whose turn it is
 * @param map the map in which the source and target are located in
 * @param source the territory where the armies will be taken from (must belong to issuer for validity)
 * @param target the territory where armies will transfer to or attack (must be adjacent to source for validity)
 * @param armies the number of armies to attack with or to transfer (must be available in source for validity)
 */
Advance::Advance(Players::Player *issuer, Graph::Map *map, Graph::Territory *source, Graph::Territory *target,
                 int armies) : Order(issuer, "advance"), map(map), source(source), target(target), armies(armies) {}

// copy constructor creates shallow copy due to circular dependency
Advance::Advance(const Advance &advance) : Order(advance) {
    this->map = advance.map;
    this->source = advance.source;
    this->target = advance.target;
    this->armies = advance.armies;
}

// memory de-allocation should be handled externally
Advance::~Advance() = default;

// returns a string to be output to the log file when notified
std::string Advance::stringToLog() const {
    return this->orderEffect;
}

// creates shallow copy via the assignment operator
Advance &Advance::operator=(const Advance &advance) {
    if (this != &advance) {
        this->map = advance.map;
        this->issuer = advance.issuer;
        this->source = advance.source;
        this->target = advance.target;
        this->armies = advance.armies;
    }
    return *this;
}

// overloads the insertion operator with a basic string representation of the object
ostream &Orders::operator<<(ostream &out, const Advance &advance) {
    return advance.write(out);
}

/* is valid if the source territory has sufficient armies & belongs to the issuing player, if the target territory
 * neighbors the source, and if the target's owner isn't negotiating with the issuer */
bool Advance::validate() {
    if (issuer == nullptr || map == nullptr || source == nullptr || target == nullptr) { // prevent runtime errors
        orderEffect = "at least one of the data members have not been properly initialized";
    } else if (hasNegotiation(issuer, target->owner)) {
        orderEffect = "peace is enforced between " + issuer->getName() + " and " + target->owner->getName();
    } else if (source->owner != issuer) {
        orderEffect = source->name + " is not owned by " + issuer->getName();
    } else if (!map->edgeExists(source, target)) {
        orderEffect = target->name + " does not neighbor " + source->name;
    } else if (armies <= 0) {
        orderEffect = "cannot issue an attack or transfer of " + to_string(armies) + " armies";
    } else if (source->numberOfArmies == 0) {
        orderEffect = source->name + " has no armies left";
    } else {
        return true;
    }
    orderEffect = "Advance canceled — " + orderEffect;
    return false;
}

/* only performs action if the order is valid: if the target territory belongs to the issuer, the armies are
 * transferred there; otherwise the armies attack the target territory*/
void Advance::execute() {
    bool valid = validate();
    if (valid && this->issuer->ps->strategyType != PlayerStrategies::CHEATER_TYPE) {
        /* as per Warzone rules, use all available armies in the case where armies have decreased to an insufficient
         * amount during the turn */
        if (armies > source->numberOfArmies) {
            armies = source->numberOfArmies;
        }
        if (source->owner == target->owner) { // perform simple army transfer
            orderEffect = to_string(armies) + " transferred to " + target->name + " from " + source->name;
            source->numberOfArmies -= armies;
            target->numberOfArmies += armies;
        } else { // initiate an attack
            int defendersKilled = static_cast<int>(round(armies * 0.6));
            int attackersKilled = static_cast<int>(round(target->numberOfArmies * 0.7));

            //if the target is a neutral strategy player and is attacked, the target switches strategy and
            //becomes an aggressive player
            if (target->owner->ps->strategyType == PlayerStrategies::NEUTRAL_TYPE) {
                delete target->owner->ps; //deleting neutral strategy to prevent cause memory leaks
                target->owner->ps = new AggressivePlayerStrategy(target->owner);
                cout << "** " << target->owner->getName() << " is attacked by " << source->owner->getName()
                     << " on territory " << target->name << " — " << target->owner->getName() << " is now an "
                     << target->owner->ps->strategyType << " player **" << endl;
            }

            if (target->numberOfArmies > defendersKilled || armies <= attackersKilled) { // failed to conquer
                orderEffect = to_string(armies) + " failed to take " + target->name + " from " + source->name;

                // update territories' armies
                target->numberOfArmies -= defendersKilled;
                source->numberOfArmies -= armies < attackersKilled ? armies : attackersKilled;
            } else { // territory successfully conquered
                orderEffect = to_string(armies) + " captured " + target->name + " from " + source->name;
                target->transferOwnership(issuer); // issuer now owns the territory

                // update territories' armies
                source->numberOfArmies -= armies;
                target->numberOfArmies = armies - attackersKilled;

                // the conquering player receives a card as a reward at the end of the turn
                issuer->receivesCard = true;
            }
        }
    } else if (valid && this->issuer->ps->strategyType == PlayerStrategies::CHEATER_TYPE) {
        orderEffect = to_string(armies) + " captured " + target->name + " from " + source->name;

        //if the target is a neutral strategy player and is attacked, the target switches strategy and
        //becomes an aggressive player
        if(target->owner->ps->strategyType == PlayerStrategies::NEUTRAL_TYPE){
            delete target->owner->ps; //deleting neutral strategy to prevent memory leaks
            target->owner->ps = new AggressivePlayerStrategy(target->owner);
            cout << "** " << target->owner->getName() << " is attacked by " << source->owner->getName()
                 << " on territory " << target->name << " — " << target->owner->getName() << " is now an "
                 << target->owner->ps->strategyType << " player **" << endl;
        }

        // The current player is a cheater and will therefore automatically conquer the adjacent enemy territory
        target->transferOwnership(issuer); // cheater player now owns the territory

        // transfer armies of the cheater player
        target->numberOfArmies = armies;
        source->numberOfArmies -= armies;

        // the conquering player receives a card as a reward at the end of the turn
        issuer->receivesCard = true;
    }

    orderEffect = (issuer == nullptr ? "" : issuer->getName()) + " executed: " + orderEffect;
    cout << orderEffect << endl;
    notify(*this);
}

// dynamically returns a clone of the calling class
Advance *Advance::clone() const {
    return new Advance(*this);
}

// helper for the stream insertion operator to function dynamically
std::ostream &Advance::write(ostream &out) const {
    return out << toString();
}

// returns a string representation of the order
std::string Advance::toString() const {
    return target == nullptr || source == nullptr ?
           "Advance" : to_string(armies) + " from " + source->name + " will attack/transfer " + target->name;
}

// prints the current state of the territories affected by the order
void Advance::displayStats(bool beforeExecution) const {
    string title, arrow;
    if (beforeExecution) {
        title = "\nStats before execution > ", arrow = "                       > ";
        cout << "Validation info        > " << source->nameAndOwner() << arrow << target->name << " is "
             << (map->edgeExists(source, target) ? "" : "not ") << "adjacent to " << source->name;
    } else {
        title = "Stats after execution  < ", arrow = "                       < ";
    }
    cout << title << source->nameAndArmies() << arrow << target->nameAndArmies() << arrow << target->nameAndOwner()
         << arrow << issuer->getName() << (issuer->receivesCard ? " receives" : " does not receive")
         << " a card at the end of the turn\n";
}

// ====================== Bomb class ======================
Bomb::Bomb() : Order(nullptr, "bomb"), target(nullptr), map(nullptr) {
    cout << "Created a Bomb order." << endl;
}

/** creates an order with all members initialized through parameters
 * @param issuer the player whose turn it is
 * @param target the territory to bomb (must be adjacent to one of issuer's territories & have diff owner for validity)
 */
Bomb::Bomb(Players::Player *issuer, Graph::Territory *target, Graph::Map *map) : Order(issuer, "bomb"), target(target),
                                                                                 map(map) {}

// copy constructor creates shallow copy due to circular dependency
Bomb::Bomb(const Bomb &bomb) : Order(bomb) {
    this->target = bomb.target;
    this->map = bomb.map;
}

// memory de-allocation should be handled externally
Bomb::~Bomb() = default;

// returns a string to be output to the log file when notified
std::string Bomb::stringToLog() const {
    return this->orderEffect;
}

// creates shallow via the assignment operator
Bomb &Bomb::operator=(const Bomb &bomb) {
    if (this != &bomb) {
        this->issuer = bomb.issuer;
        this->target = bomb.target;
        this->map = bomb.map;
    }
    return *this;
}

// overloads the insertion operator with a basic string representation of the object
ostream &Orders::operator<<(ostream &out, const Bomb &bomb) {
    return bomb.write(out);
}

// is valid if the target territory is not owned by the issuer and is adjacent to at least one of issuer's territories
bool Bomb::validate() {
    if (issuer == nullptr || map == nullptr || target == nullptr) { // prevent runtime errors
        orderEffect = "Bomb canceled — at least one of the data members have not been properly initialized";
        return false;
    }

    if (hasNegotiation(issuer, target->owner)) {
        orderEffect = "Bomb canceled — peace is enforced between " + issuer->getName() + " and " +
                      target->owner->getName();
        return false;
    }

    // checking if the target territory belongs to the issuing player
    if (this->issuer == target->owner) {
        orderEffect = "Bomb canceled — " + issuer->getName() + " owns " + target->name;
        return false;
    }

    // loop to check if the target territory is adjacent to any of the issuing player's territories
    for (auto &territory: issuer->territories) {
        if (this->map->edgeExists(territory.second, target))
            return true;
    }
    orderEffect = "Bomb canceled — " + target->name + " is not adjacent to any of " + issuer->getName() +
                  "'s territories";
    return false;
}

// if valid, half of the armies are removed from the target territory
void Bomb::execute() {
    if (validate()) {
        orderEffect = toString();
        target->numberOfArmies /= 2;
    }
    // bomb is considered an attack, if target is a neutral strategy player, then switch to aggressive strategy player
    if(target->owner->ps->strategyType == PlayerStrategies::NEUTRAL_TYPE){
        delete target->owner->ps; //deleting neutral strat player not to cause memory leaks
        cout<<target->owner->getName()<<" JUST GOT BOMBED"<<endl;
        target->owner->ps = new AggressivePlayerStrategy(target->owner);
        cout<<"** "<<target->owner->getName()<<" is now an "<<target->owner->ps->strategyType<<" player **"<<endl;
    }
    orderEffect = (issuer == nullptr ? "" : issuer->getName()) + " executed: " + orderEffect;
    cout << orderEffect << endl;
    notify(*this);
}

// dynamically returns a clone of the calling class
Bomb *Bomb::clone() const {
    return new Bomb(*this);
}

// helper for the stream insertion operator to function dynamically
std::ostream &Bomb::write(ostream &out) const {
    return out << toString();
}

// returns a string representation of the order
std::string Bomb::toString() const {
    return target == nullptr ? "Bomb" : "Bomb " + target->name;
}

// prints the current state of the territory affected by the order
void Bomb::displayStats(bool beforeExecution) const {
    string title, arrow;
    if (beforeExecution) {
        title = "\nStats before execution > ", arrow = "                       > ";
        auto it = find_if(map->edges.begin(), map->edges.end(),
                          [this](Graph::Edge *e) { return e->source == target && e->destination->owner == issuer; });
        cout << "Validation info        > " << target->nameAndOwner() << arrow << target->name;
        if (it == map->edges.end()) {
            cout << " doesn't neighbor any territories owned by " << target->owner->getName();
        } else {
            cout << " neighbors " << (*it)->destination->name << " which is owned by "
                 << (*it)->destination->owner->getName();
        }
    } else {
        title = "Stats after execution  < ";
    }
    cout << title << target->nameAndArmies();
}

// ====================== Blockade class ======================

// used give unique name to new neutral players created by this order
int Blockade::newNeutralId = 0;

/** creates an order with all members initialized through parameters
 * @param issuer the player whose turn it is
 * @param target the territory to turn into a neutral blockade (must belong to issuer for validity)
 * @param players the current game players
 */
Blockade::Blockade(Players::Player *issuer, Graph::Territory *target, std::vector<Players::Player *> &players)
    : Order(issuer, "blockade"), target(target),  players(players){}

// copy constructor creates shallow copy due to circular dependency
Blockade::Blockade(const Blockade &blockade) : Order(blockade), players(blockade.players) {
    this->target = blockade.target;
}

// memory de-allocation should be handled externally
Blockade::~Blockade() = default;

// returns a string to be output to the log file when notified
std::string Blockade::stringToLog() const {
    return this->orderEffect;
}

// creates shallow copy via the assignment operator
Blockade &Blockade::operator=(const Blockade &blockade) {
    if (this != &blockade) {
        this->issuer = blockade.issuer;
        this->target = blockade.target;
    }
    return *this;
}

// overloads the insertion operator with a basic string representation of the object
ostream &Orders::operator<<(ostream &out, const Blockade &blockade) {
    return blockade.write(out);
}

// is valid if the target territory is owned by the issuing player
bool Blockade::validate() {
    if (issuer == nullptr || target == nullptr) { // prevent runtime errors
        orderEffect = "at least one of the data members have not been properly initialized";
    } else if (target->owner != this->issuer) {
        orderEffect = target->name + " is not owned by " + issuer->getName();
    } else {
        return true;
    }
    orderEffect = "Blockade canceled — " + orderEffect;
    return false;
}

// if valid, the number of armies on the target territory are doubled and transferred to a neutral player
void Blockade::execute() {
    if (validate()) {
        orderEffect = toString();
        target->numberOfArmies *= 2;

        // get a neutral player in the player list, if it exists
        auto neutralPlayerIt = find_if(players.begin(), players.end(), [](Players::Player *p) {
            return p->ps->strategyType == PlayerStrategies::NEUTRAL_TYPE;
        });

        if (neutralPlayerIt == players.end()) { // neutral player must be created
            newNeutralId += 1;
            auto neutralPlayer = new Players::Player(PlayerStrategies::NEUTRAL_TYPE + to_string(newNeutralId));
            PlayerStrategies *ps = new NeutralPlayerStrategy(neutralPlayer);
            neutralPlayer->ps = ps;
            players.push_back(neutralPlayer);
            target->transferOwnership(neutralPlayer);
        } else { // neutral player exists
            target->transferOwnership(*neutralPlayerIt);
        }
    }
    orderEffect = (issuer == nullptr ? "" : issuer->getName()) + " executed: " + orderEffect;
    cout << orderEffect << endl;
    notify(*this);
}

// dynamically returns a clone of the calling class
Blockade *Blockade::clone() const {
    return new Blockade(*this);
}

// helper for the stream insertion operator to function dynamically
std::ostream &Blockade::write(ostream &out) const {
    return out << toString();
}

// returns a string representation of the order
std::string Blockade::toString() const {
    return target == nullptr ? "Blockade" : "Blockade " + target->name;
}

// prints the current state of the territory affected by the order
void Blockade::displayStats(bool beforeExecution) const {
    string title, arrow;
    if (beforeExecution) {
        title = "Stats before execution > ", arrow = "                       > ";
    } else {
        title = "Stats after execution  < ", arrow = "                       < ";
    }
    cout << title << target->nameAndArmies() << arrow << target->nameAndOwner();
}

// ====================== Airlift class ======================
Airlift::Airlift() : Order(nullptr, "airlift"), source(nullptr), target(nullptr), armies(0) {
    cout << "Created an Airlift order." << endl;
}

/** creates an order with all members initialized through parameters
 * @param issuer the player whose turn it is
 * @param source the territory where the armies will be taken from (must belong to issuer for validity)
 * @param target the territory where the armies will be transferred to (must belong to issuer for validity)
 * @param armies the number of armies to transfer
 */
Airlift::Airlift(Players::Player *issuer, Graph::Territory *source, Graph::Territory *target, int armies)
        : Order(issuer, "airlift"), source(source), target(target), armies(armies) {}

// copy constructor creates shallow copy due to circular dependency
Airlift::Airlift(const Airlift &airlift) : Order(airlift) {
    this->source = airlift.source;
    this->target = airlift.target;
    this->armies = airlift.armies;
}

// memory de-allocation should be handled externally
Airlift::~Airlift() = default;

// returns a string to be output to the log file when notified
std::string Airlift::stringToLog() const {
    return this->orderEffect;
}

// creates shallow copy via the assignment operator
Airlift &Airlift::operator=(const Airlift &airlift) {
    if (this != &airlift) {
        this->issuer = airlift.issuer;
        this->source = airlift.source;
        this->target = airlift.target;
    }
    return *this;
}

// overloads the insertion operator with a basic string representation of the object
ostream &Orders::operator<<(ostream &out, const Airlift &airlift) {
    return airlift.write(out);
}

// is valid if both the source and target territories belong to the issuer and the armies are not 0
bool Airlift::validate() {
    if (issuer == nullptr || source == nullptr || target == nullptr) { // prevent runtime errors
        orderEffect = "at least one of the data members have not been properly initialized";
    } else if (source->owner != issuer) {
        orderEffect = source->name + " is not owned by " + issuer->getName();
    } else if (target->owner != issuer) {
        orderEffect = target->name + " is not owned by " + issuer->getName();
    } else if (armies <= 0) {
        orderEffect = "cannot issue an airlift of " + to_string(armies) + " armies";
    } else if (source->numberOfArmies == 0) {
        orderEffect = source->name + " has no armies left";
    } else {
        return true;
    }
    orderEffect = "Airlift canceled — " + orderEffect;
    return false;
}

// if the order is valid, the selected number of armies are moved from the source to the target territories
void Airlift::execute() {
    /* as per Warzone rules, use all available armies in the case where armies have decreased to an insufficient
     * amount during the turn */
    if (armies > source->numberOfArmies) {
        armies = source->numberOfArmies;
    }
    if (validate()) {
        orderEffect = toString();
        source->numberOfArmies -= armies;
        target->numberOfArmies += armies;
    }
    orderEffect = (issuer == nullptr ? "" : issuer->getName()) + " executed: " + orderEffect;
    cout << orderEffect << endl;
    notify(*this);
}

// dynamically returns a clone of the calling class
Airlift *Airlift::clone() const {
    return new Airlift(*this);
}

// helper for the stream insertion operator to function dynamically
std::ostream &Airlift::write(ostream &out) const {
    return out << toString();
}

// returns a string representation of the order
std::string Airlift::toString() const {
    return target == nullptr || source == nullptr ?
           "Airlift" : "Airlift " + to_string(armies) + " armies from " + source->name + " to " + target->name;
}

// prints the current state of the territories affected by the order
void Airlift::displayStats(bool beforeExecution) const {
    string title, arrow;
    if (beforeExecution) {
        title = "Stats before execution > ", arrow = "                       > ";
        cout << "Validation info        > " << source->nameAndOwner() << arrow << target->nameAndOwner();
    } else {
        title = "Stats after execution  < ", arrow = "                       < ";
    }
    cout << title << source->nameAndArmies() << arrow << target->nameAndArmies();
}

// ====================== Negotiate class ======================
Negotiate::Negotiate() : Order(nullptr, "negotiate"), target(nullptr) {
    cout << "Created a Negotiate order." << endl;
}

/** creates an order with all members initialized through parameters
 * @param issuer the player whose turn it is
 * @param target the player to negotiate with (must not be the issuer for validity)
 */
Negotiate::Negotiate(Players::Player *issuer, Players::Player *target) : Order(issuer, "negotiate"), target(target) {}

// copy constructor creates shallow copy due to circular dependency
Negotiate::Negotiate(const Negotiate &negotiate) : Order(negotiate) {
    this->target = negotiate.target;
}

// memory de-allocation should be handled externally
Negotiate::~Negotiate() = default;

// returns a string to be output to the log file when notified
std::string Negotiate::stringToLog() const {
    return this->orderEffect;
}

// creates shallow copy via the assignment operator
Negotiate &Negotiate::operator=(const Negotiate &negotiate) {
    if (this != &negotiate) {
        this->issuer = negotiate.issuer;
        this->target = negotiate.target;
    }
    return *this;
}

// overloads the insertion operator with a basic string representation of the object
ostream &Orders::operator<<(ostream &out, const Negotiate &negotiate) {
    return negotiate.write(out);
}

// is valid if the target player is not eliminated & is not the issuing player
bool Negotiate::validate() {
    if (issuer == nullptr || target == nullptr) { // prevent runtime errors
        orderEffect = "at least one of the data members have not been properly initialized";
    } else if (target->isEliminated) {
        orderEffect = "the target player has been eliminated";
    } else if (issuer == target) {
        orderEffect = "the target player is not an enemy";
    } else {
        return true;
    }
    orderEffect = "Negotiate canceled — " + orderEffect;
    return false;
}

// if valid, prevents all attacks between the issuer and the target player for the remainder of the turn
void Negotiate::execute() {
    if (validate()) {
        orderEffect = toString();
        issuer->cannotAttack.push_back(target->getName());
        target->cannotAttack.push_back(issuer->getName());
    }
    orderEffect = (issuer == nullptr ? "" : issuer->getName()) + " executed: " + orderEffect;
    cout << orderEffect << endl;
    notify(*this);

}

// dynamically returns a clone of the calling class
Negotiate *Negotiate::clone() const {
    return new Negotiate(*this);
}

// helper for the stream insertion operator to function dynamically
std::ostream &Negotiate::write(ostream &out) const {
    return out << toString();
}

// returns a string representation of the order
std::string Negotiate::toString() const {
    return target == nullptr ? "Negotiate" : "Negotiate with " + target->getName();
}

// prints the current contents of the issuer's and target's 'cannotAttack' lists
void Negotiate::displayStats(bool beforeExecution) const {
    string title, arrow;
    if (beforeExecution) {
        title = "Stats before execution > ", arrow = "                       > ";
    } else {
        title = "Stats after execution  < ", arrow = "                       < ";
    }
    cout << title << issuer->cannotAttackString() << arrow << target->cannotAttackString();
}

// ====================== OrdersList class ======================
OrdersList::OrdersList() = default;

// creates a deep copy of the given OrdersList and of all the Order objects it contains
OrdersList::OrdersList(const OrdersList &ordersList) {
    orders.reserve(ordersList.orders.size());
    for (Order *order: ordersList.orders) {
        orders.push_back(order->clone());
    }
}

// destructor deallocates memory to prevent memory leaks
OrdersList::~OrdersList() {
    for (Order *order: orders) {
        delete order;
    }
}

// returns a string to be output to the log file when notified
std::string OrdersList::stringToLog() const {
    Order *order = this->orders.back();
    return order->issuer->getName() + " has added \"" + order->toString() + "\" to their list of orders";
}

// creates true deep copy via the assignment operator
OrdersList &OrdersList::operator=(const OrdersList &ordersList) {
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

// overloads the insertion operator with a basic string representation of the object
ostream &Orders::operator<<(ostream &out, const OrdersList &ordersList) {
    out << "{" << endl;
    int i = 0;
    for (auto order: ordersList.orders) {
        out << "\t[" << i << "] = " << *order << endl;
        i++;
    }
    out << "}" << endl;
    return out;
}

// removes and deletes the element in the list located at the given index
void OrdersList::remove(int index) {
    // validate index
    if (index < 0 || index >= orders.size()) {
        return;
    }

    delete orders[index];
    orders.erase(orders.begin() + index);
}

// moves the element from the given oldIndex to the given newIndex while preserving the order of other elements
[[maybe_unused]] void OrdersList::move(int oldIndex, int newIndex) {
    // validate indices
    if (oldIndex < 0 || newIndex < 0 || oldIndex == newIndex || oldIndex >= orders.size() ||
        newIndex >= orders.size()) {
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

// adds an element to the back of the list
void OrdersList::add(Order *const newOrder) {
    if (newOrder != nullptr && newOrder->issuer != nullptr) {
        cout << newOrder->issuer->getName() << " issued: " << *newOrder;

        // only add line break if order is not played by issuing a card (for cards, line is completed in play methods)
        if (newOrder->type == "advance" ||
            newOrder->type == "deploy" && !dynamic_cast<Deploy *>(newOrder)->isReinforcementCard) {
            cout << endl;
        }
    }
    orders.push_back(newOrder);
    notify(*this);
}

// returns the current number of elements in the list
size_t OrdersList::length() const {
    return orders.size();
}

// returns the element at the given index
Order *OrdersList::element(size_t index) const {
    // validate index
    if (index >= orders.size()) {
        return nullptr;
    }

    return orders[index];
}
