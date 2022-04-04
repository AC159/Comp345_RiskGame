#include "PlayerStrategies.h"


PlayerStrategies::PlayerStrategies(Players::Player *p) {
    this->player = p;
}

PlayerStrategies::PlayerStrategies(const PlayerStrategies &ps) {
    this->player = ps.player;
}

PlayerStrategies::~PlayerStrategies() = default;

BenevolentPlayerStrategy::BenevolentPlayerStrategy(Players::Player *p) : PlayerStrategies(p) {}

BenevolentPlayerStrategy::BenevolentPlayerStrategy(const BenevolentPlayerStrategy &b) : PlayerStrategies(b.player) {}

BenevolentPlayerStrategy& BenevolentPlayerStrategy::operator=(const BenevolentPlayerStrategy &b) {
    if (this == &b) return *this;
    this->player = b.player;
    return *this;
}

std::ostream &operator<<(std::ostream &out, const BenevolentPlayerStrategy &b) {
    out << "Player " << b.player->getName() << " is playing the benevolent player strategy" << std::endl;
    return out;
}

/**
 * @params mapEdges list of Edge objects present in the map
 * @returns map of territories owned by this player to be defended ordered by priority
 *
 * The keys of the multimap consist of the number of armies in each territory ordered in ascending order
 *
 * */
std::multimap<int, Graph::Territory *> BenevolentPlayerStrategy::toDefend(const std::vector<Graph::Edge *> &mapEdges) {
    std::multimap<int, Graph::Territory *> territoriesToDefend;

    for (auto pair : this->player->territories) {
        territoriesToDefend.insert(std::pair<int, Graph::Territory*> (pair.second->numberOfArmies, pair.second));
    }

    return territoriesToDefend;
}

/**
 * @params edges list of Edge objects present in the map
 * @returns list of territories to be attacked by this player in priority. The benevolent player will issue Advance orders on these territories
 * */
std::multimap<int, Graph::Territory *> BenevolentPlayerStrategy::toAttack(const std::vector<Graph::Edge *> &edges) {
    std::multimap<int, Graph::Territory *> territoriesToAttack;

    // Find territories that have adjacent enemy territories and prioritize them based on the number of armies they have
    std::vector<Graph::Territory *> enemyTerritories;
    for (auto pair : this->player->territories) {
        // Check if this territory has any adjacent enemy territories, if yes, then add it to the list of territories to attack.
        // The benevolent player will create advance orders on these territories in the issueOrder() method
        enemyTerritories = pair.second->adjacentEnemyTerritories(edges);
        if (!enemyTerritories.empty()) {
            territoriesToAttack.insert(std::pair<int, Graph::Territory *> (pair.second->numberOfArmies, pair.second));
        }
    }

    return territoriesToAttack;
}

void BenevolentPlayerStrategy::issueOrder(Cards::Deck *deck, Graph::Map *map) {

    std::multimap<int, Graph::Territory *> toAttack = this->toAttack(map->edges);
    std::multimap<int, Graph::Territory *> toDefend = this->toDefend(map->edges);

    double randomMultiplier;

    // Create Deploy orders on the territories returned by the toDefend() method
    int armiesDeployed = 0;
    for (auto &pair : toDefend) {

        // generate a value between 0 and 1 exclusive
        randomMultiplier = ((double) rand() / (RAND_MAX));

        int armiesToDeploy = floor((this->player->reinforcementPool - armiesDeployed) * randomMultiplier);
        if (armiesToDeploy < 1) armiesToDeploy = 1;

        if (armiesDeployed >= this->player->reinforcementPool) break;

        this->player->orders->add(new Orders::Deploy(this->player, pair.second, armiesToDeploy));
        armiesDeployed = armiesDeployed + armiesToDeploy;
    }

    // Create Advance orders on the territories returned by the toAttack() method

    // Get the list of territories owned by the current player that have no enemy neighbors
    std::vector<Graph::Territory *> friendlyTerritories;

    // Only create Advance orders if the player has territories with no enemy neighbors.
    for (auto &pair : toAttack) {

        friendlyTerritories = pair.second->adjacentFriendlyTerritories(map->edges);
        if (!friendlyTerritories.empty()) {
            // sort these territories based on the number of armies in each of them
            // this function uses the overloaded operator < in the Territory class
            std::sort(friendlyTerritories.begin(), friendlyTerritories.end());

            int index = (int) friendlyTerritories.size() - 1;

            // generate a value between 0 and 1 exclusive
            randomMultiplier = ((double) rand() / (RAND_MAX));

            int armiesToAdvance = floor(friendlyTerritories.at(index)->numberOfArmies * randomMultiplier);
            if (armiesToAdvance < 1) armiesToAdvance = 1;
            if (friendlyTerritories.at(index)->numberOfArmies <= 2) continue; // do not take armies from territories that have 2 or fewer armies

            this->player->orders->add(new Orders::Advance(this->player, map, friendlyTerritories.at(index), pair.second, armiesToAdvance));
        }
    }

    // Create an order from a card
    if (!this->player->hand->cards.empty()) {
        int randomIndex = std::experimental::randint(0, (int) this->player->hand->cards.size()-1);
        Cards::Card *card = this->player->hand->cards.at(randomIndex);

        // The benevolent player will not issue bomb orders since his goal is to protect his weakest territories
        std::string type = card->getType();
        // Play the card based on its type
        if (type == "reinforcement") {
            // play the reinforcement card on the first territory of the toDefend list
            dynamic_cast<Cards::Reinforcement *>(card)->play(this->player, deck, toDefend.begin()->second);
        } else if (type == "blockade") {
            dynamic_cast<Cards::Blockade *>(card)->play(this->player, deck, toDefend.begin()->second);
        } else if (type == "airlift" && this->player->territories.size() <= 1) { // airlift orders only make sense if the player owns more than 1 territory
            Graph::Territory *src = toDefend.rbegin()->second; // take half the armies from the territory that has the most armies
            dynamic_cast<Cards::Airlift *>(card)->play(this->player, deck, src, toDefend.begin()->second, src->numberOfArmies / 2);
        } else if (type == "diplomacy") {
            // Play the diplomacy card on the largest enemy territory
            std::vector<Graph::Territory *> enemyTerritories = toDefend.begin()->second->adjacentEnemyTerritories(map->edges);

            // sort these territories based on the number of armies in each of them
            // this function uses the overloaded operator < in the Territory class
            std::sort(enemyTerritories.begin(), enemyTerritories.end());
            dynamic_cast<Cards::Diplomacy *>(card)->play(this->player, enemyTerritories.at(enemyTerritories.size()-1)->owner, deck);
        }
    }

}

BenevolentPlayerStrategy::~BenevolentPlayerStrategy() = default;

// ================== Cheater Player Strategy Implementation ==================

CheaterPlayerStrategy::CheaterPlayerStrategy(Players::Player *p) : PlayerStrategies(p) {}

CheaterPlayerStrategy::CheaterPlayerStrategy(const CheaterPlayerStrategy &cheater) : PlayerStrategies(cheater.player) {}

CheaterPlayerStrategy::~CheaterPlayerStrategy() = default;

/**
 * @params mapEdges list of Edge objects present in the map
 * @returns map of territories owned by this player to be defended ordered by priority
 *
 * The keys of the multimap consist of the number of armies in each territory ordered in ascending order
 * */
std::multimap<int, Graph::Territory *> CheaterPlayerStrategy::toDefend(const std::vector<Graph::Edge *> &mapEdges) {
    std::multimap<int, Graph::Territory *> territoriesToDefend;
    for (auto pair : this->player->territories) {
        territoriesToDefend.insert(std::pair<int, Graph::Territory*> (pair.second->numberOfArmies, pair.second));
    }
    return territoriesToDefend;
}

/**
 * @params edges list of Edge objects present in the map
 * @returns list of adjacent enemy territories to be conquered by this player. The cheater player will conquer all these territories.
 * */
std::multimap<int, Graph::Territory *> CheaterPlayerStrategy::toAttack(const std::vector<Graph::Edge *> &edges) {
    std::multimap<int, Graph::Territory *> territories;
    std::vector<Graph::Territory *> enemyTerritories;
    // Get all adjacent enemy territories
    for (auto pair : this->player->territories) {
        enemyTerritories = pair.second->adjacentEnemyTerritories(edges);
        for (auto *t : enemyTerritories) {
            territories.insert(std::pair<int, Graph::Territory *>(t->numberOfArmies, t));
        }
    }
    return territories;
}

void CheaterPlayerStrategy::issueOrder(Cards::Deck *deck, Graph::Map *map) {

}

CheaterPlayerStrategy &CheaterPlayerStrategy::operator=(const CheaterPlayerStrategy &cheater) {
    if (this == &cheater) return *this;
    this->player = cheater.player;
    return *this;
}

std::ostream &operator<<(std::ostream &out, const CheaterPlayerStrategy &cheater) {
    out << "Player " << cheater.player->getName() << " is playing the cheater player strategy" << std::endl;
    return out;
}
