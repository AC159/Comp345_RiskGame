#include <regex>
#include "PlayerStrategies.h"
#include <utility>

// initialize static variables of the PlayerStrategies class
const std::string PlayerStrategies::BENEVOLENT_TYPE = "benevolent";
const std::string PlayerStrategies::CHEATER_TYPE = "cheater";
const std::string PlayerStrategies::HUMAN_TYPE = "human";
const std::string PlayerStrategies::AGGRESSIVE_TYPE = "aggressive";
const std::string PlayerStrategies::NEUTRAL_TYPE = "neutral";

using namespace std;
using namespace Graph;

// constructor to be used by subclasses
PlayerStrategies::PlayerStrategies(Players::Player *p, std::string strategyType) {
    this->player = p;
    this->strategyType = std::move(strategyType);
}

// creates shallow copy (avoids cyclic dependency issues)
PlayerStrategies::PlayerStrategies(const PlayerStrategies &ps) {
    this->player = ps.player;
    this->strategyType = ps.strategyType;
}

// dynamically allocated players should be deleted elsewhere
PlayerStrategies::~PlayerStrategies() = default;

// ================== Benevolent Player Strategy Implementation ==================
BenevolentPlayerStrategy::BenevolentPlayerStrategy(Players::Player *p) : PlayerStrategies(p, PlayerStrategies::BENEVOLENT_TYPE) {}

// creates shallow copy (avoids cyclic dependency issues)
BenevolentPlayerStrategy::BenevolentPlayerStrategy(const BenevolentPlayerStrategy &b) : PlayerStrategies(b.player, PlayerStrategies::BENEVOLENT_TYPE) {}

// creates shallow copy via assignment operator (avoids cyclic dependency issues)
BenevolentPlayerStrategy& BenevolentPlayerStrategy::operator=(const BenevolentPlayerStrategy &b) {
    if (this == &b) return *this;
    this->player = b.player;
    return *this;
}

// outputs the player's name and strategy via the stream insertion operator
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

    // Find territories that have adjacent enemy territories and prioritize them based on the number of armies they have
    std::vector<Graph::Territory *> enemyTerritories;
    for (auto pair : this->player->territories) {
        // Check if this territory has any adjacent enemy territories, if yes, then add it to the list of territories to defend.
        // The benevolent player will create advance (and deploy orders on the top 10% of them) orders on these territories in the issueOrder() method
        enemyTerritories = pair.second->adjacentEnemyTerritories(mapEdges);
        if (!enemyTerritories.empty()) {
            territoriesToDefend.insert(std::pair<int, Graph::Territory *> (pair.second->numberOfArmies, pair.second));
        }
    }

    return territoriesToDefend;
}

/**
 * @params edges list of Edge objects present in the map
 * @returns empty list since the Benevolent player doesn't issue attacks
 * */
std::multimap<int, Graph::Territory *> BenevolentPlayerStrategy::toAttack(const std::vector<Graph::Edge *> &edges) {
    // The toAttack method is not relevant for the Benevolent player, we just use the toDefend method since this player does not attack other players
    std::multimap<int, Graph::Territory *> territories;
    return territories;
}

// issues orders and plays cards only to defend the player's weakest territories; never attacks other players
void BenevolentPlayerStrategy::issueOrder(GameEngine &game) {
    auto map = game.mapLoader->map;
    auto deck = game.deck;

    std::multimap<int, Graph::Territory *> toDefend = this->toDefend(map->edges);

    // Create Deploy orders on the territories returned by the toDefend() method
    int armiesDeployed = 0;
    int listSize = toDefend.size();
    int territoriesRemaining = listSize >= 10 ? listSize * 0.1 : listSize; // Get the top 10% of the weakest territories to defend
    if (territoriesRemaining < 1) territoriesRemaining = listSize;
    int armiesToDeploy = this->player->reinforcementPool >= territoriesRemaining ?  this->player->reinforcementPool / territoriesRemaining : 1;
    for (auto &pair : toDefend) {

        if (armiesDeployed >= this->player->reinforcementPool || territoriesRemaining == 0) break;

        if (territoriesRemaining == 1) {
            // Last territory of the top 10 will get the remaining armies left to deploy
            armiesToDeploy = this->player->reinforcementPool - armiesDeployed;
        }
        this->player->orders->add(new Orders::Deploy(this->player, pair.second, armiesToDeploy));
        armiesDeployed = armiesDeployed + armiesToDeploy;
        territoriesRemaining--;
    }

    // create Advance orders from the strongest territories to the weakest
    for (auto &pair : toDefend) {

        // this will return the territories owned by the current player that are adjacent to the territory to be defended
        std::vector<Graph::Territory *> friendlyTerritories = pair.second->adjacentFriendlyTerritories(map->edges);

        if (!friendlyTerritories.empty()) {
            // sort these territories based on the number of armies in each of them
            // this function uses the overloaded operator < in the Territory class
            // sorted in descending order of the number of armies per territory
            std::sort(friendlyTerritories.begin(), friendlyTerritories.end(), std::greater<>());

            // From the friendly territory that is adjacent to the current enemy territory to be defended, take a random amount of armies to transfer
            auto it = friendlyTerritories.begin(); // get the territory owned by the current player that has the most armies
            int armiesToAdvance = floor((*it)->numberOfArmies * 0.25); // take 25% of the armies of the strongest territory and transfer them to the weakest
            if (armiesToAdvance < 1) armiesToAdvance = 1;
            if ((*it)->numberOfArmies <= 2 || (*it)->numberOfArmies <= armiesToAdvance) continue; // do not take armies from territories that have 2 or fewer armies

            this->player->orders->add(new Orders::Advance(this->player, map, *it, pair.second, armiesToAdvance));
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
            dynamic_cast<Cards::Blockade *>(card)->play(this->player, deck, toDefend.begin()->second, game.playersList);
        } else if (type == "airlift" && this->player->territories.size() > 1) { // airlift orders only make sense if the player owns more than 1 territory
            Graph::Territory *src = toDefend.rbegin()->second; // take half the armies from the territory that has the most armies
            dynamic_cast<Cards::Airlift *>(card)->play(this->player, deck, src, toDefend.begin()->second, src->numberOfArmies / 2);
        } else if (type == "diplomacy") {
            // Play the diplomacy card on the largest enemy territory
            std::vector<Graph::Territory *> enemyTerritories = toDefend.begin()->second->adjacentEnemyTerritories(map->edges);

            // sort these territories based on the number of armies in each of them
            // this function uses the overloaded operator < in the Territory class
            if (!enemyTerritories.empty()) {
                std::sort(enemyTerritories.begin(), enemyTerritories.end());
                dynamic_cast<Cards::Diplomacy *>(card)->play(this->player, enemyTerritories.at(enemyTerritories.size()-1)->owner, deck);
            }
        }
    }

}

// dynamically allocated players should be deleted elsewhere
BenevolentPlayerStrategy::~BenevolentPlayerStrategy() = default;

// ================== Cheater Player Strategy Implementation ==================
CheaterPlayerStrategy::CheaterPlayerStrategy(Players::Player *p) : PlayerStrategies(p, PlayerStrategies::CHEATER_TYPE) {}

// creates shallow copy (avoids cyclic dependency issues)
CheaterPlayerStrategy::CheaterPlayerStrategy(const CheaterPlayerStrategy &cheater) : PlayerStrategies(cheater.player, PlayerStrategies::CHEATER_TYPE) {}

// memory de-allocation of player should be handled externally
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

// advance orders issued with this strategy will result in the target territory to be automatically conquered
void CheaterPlayerStrategy::issueOrder(GameEngine &game) {
    auto map = game.mapLoader->map;
    auto deck = game.deck;
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

    // create Advance orders
    for (auto &pair : toAttack) {

        // this will return the territories owned by the current player that are adjacent to the territory to be attacked
        friendlyTerritories = pair.second->adjacentEnemyTerritories(map->edges);

        if (!friendlyTerritories.empty()) {
            // sort these territories based on the number of armies in each of them
            // this function uses the overloaded operator < in the Territory class
            std::sort(friendlyTerritories.begin(), friendlyTerritories.end(), std::greater<>());

            // Find the territory owned by the current player that has the most armies
            Players::Player *currentOwner = this->player;
            auto it = std::find_if(friendlyTerritories.begin(), friendlyTerritories.end(), [&currentOwner](Territory *t) {return t->owner == currentOwner;});

            // generate a value between 0 and 1 exclusive
            randomMultiplier = ((double) rand() / (RAND_MAX));

            if (it != friendlyTerritories.end()) {
                // From the friendly territory that is adjacent to the current enemy territory to be attacked, take a random amount of armies to transfer
                int armiesToAdvance = floor((*it)->numberOfArmies * randomMultiplier);
                if (armiesToAdvance < 1) armiesToAdvance = 1;
                if ((*it)->numberOfArmies <= 2 || (*it)->numberOfArmies <= armiesToAdvance) continue; // do not take armies from territories that have 2 or fewer armies

                this->player->orders->add(new Orders::Advance(this->player, map, *it, pair.second, armiesToAdvance));
            }
        }
    }

    // Create an order from a card
    if (!this->player->hand->cards.empty()) {
        int randomIndex = std::experimental::randint(0, (int) this->player->hand->cards.size()-1);
        Cards::Card *card = this->player->hand->cards.at(randomIndex);

        std::string type = card->getType();
        // Play the card based on its type
        if (type == "reinforcement") {
            // play the reinforcement card on the first territory of the toDefend list
            dynamic_cast<Cards::Reinforcement *>(card)->play(this->player, deck, toDefend.begin()->second);
        } else if (type == "airlift" && this->player->territories.size() > 1) { // airlift orders only make sense if the player owns more than 1 territory
            Graph::Territory *src = toDefend.rbegin()->second; // take half the armies from the territory that has the most armies
            dynamic_cast<Cards::Airlift *>(card)->play(this->player, deck, src, toDefend.begin()->second, src->numberOfArmies / 2);
        }
    }

}

// creates shallow copy via assignment operator (avoids cyclic dependency issues)
CheaterPlayerStrategy &CheaterPlayerStrategy::operator=(const CheaterPlayerStrategy &cheater) {
    if (this == &cheater) return *this;
    this->player = cheater.player;
    return *this;
}

// outputs the player's name and strategy via the stream insertion operator
std::ostream &operator<<(std::ostream &out, const CheaterPlayerStrategy &cheater) {
    out << "Player " << cheater.player->getName() << " is playing the cheater player strategy" << std::endl;
    return out;
}

//computer player that focuses on attack (deploys or advances armies on its strongest
//country, then always advances to enemy territories until it cannot do so anymore).
// ================== Aggressive Player Strategy Implementation ==================

// constructor
AggressivePlayerStrategy::AggressivePlayerStrategy(Players::Player *p) : PlayerStrategies(p, PlayerStrategies::AGGRESSIVE_TYPE) {}

// copy constructor
AggressivePlayerStrategy::AggressivePlayerStrategy(const AggressivePlayerStrategy &aggressive) : PlayerStrategies(aggressive.player,PlayerStrategies::AGGRESSIVE_TYPE){}

// destructor
AggressivePlayerStrategy::~AggressivePlayerStrategy() = default;

// assignment operator
AggressivePlayerStrategy& AggressivePlayerStrategy::operator=(const AggressivePlayerStrategy &aggressive) {
    if (this ==&aggressive) return *this;
    this->player =aggressive.player;
    return *this;
}

// stream insertion operator
std::ostream &operator<<(std::ostream &out, const AggressivePlayerStrategy &aggressive) {
    out << "Player " << aggressive.player->getName() << " is playing the aggressive player strategy" << std::endl;
    return out;
}

//returns territories owned by player
std::multimap<int, Graph::Territory *> AggressivePlayerStrategy::toDefend(const std::vector<Graph::Edge *> &mapEdges) {
    std::multimap<int,Graph::Territory *> territoriesToDefend;
    for(auto pair : this->player->territories){
        territoriesToDefend.insert(std::pair <int,Graph::Territory *> (pair.second->numberOfArmies,pair.second));
    }
    return territoriesToDefend;
}
//returns adjacent enemy territories
std::multimap<int, Graph::Territory *> AggressivePlayerStrategy::toAttack(const std::vector<Graph::Edge *> &mapEdges) {
    // a list of territories to attack in ascending order of number of armies
    std::multimap<int, Graph::Territory *> territoriesToAttack;
    std::vector<Graph::Territory *> enemyTerritories;// adjacent enemy territories
    for(auto pair: this->player->territories){
        enemyTerritories = pair.second->adjacentEnemyTerritories(mapEdges);
        for(auto *t : enemyTerritories){
            territoriesToAttack.insert(std::pair<int,Graph::Territory *> (t->numberOfArmies,t));
        }
    }
    return territoriesToAttack;
}

/**
 * Aggressive player moves all his armies to defend his strongest territory and
 * attacks the weakest adjacent enemy territory with that army
 * @param game the engine running the current game
 */
void AggressivePlayerStrategy::issueOrder(GameEngine &game) {
    auto map = game.mapLoader->map;
    auto deck = game.deck;
    std::cout<<"a wild "<<this->player->getName()<<" appeared, prepare to die"<<std::endl;

    std::multimap<int, Graph::Territory *> toAttack = this->toAttack(map->edges);
    std::multimap<int, Graph::Territory *> toDefend = this->toDefend(map->edges);

    auto weakestEnemy = toAttack.begin(); //attack the enemy territory with the least armies
    auto strongestTerritory = toDefend.crbegin(); //territory with the most armies

    // find adjacent territory to the weakest enemy to deploy armies to and attack
    std::multimap<int, Graph::Territory *> playerAdjacentTerritories;
    int armiesToAdvance=0;
    for(auto *p: weakestEnemy->second->adjacentEnemyTerritories(map->edges)){
       if(p ->owner ==this->player)
           playerAdjacentTerritories.insert(std::pair<int,Graph::Territory *>(p->numberOfArmies,p));
    }

   if(!playerAdjacentTerritories.empty()){//players has adjacent territory
       int armiesToDeploy = this->player->reinforcementPool;
       this->player->orders->add(new Orders::Deploy(this->player,playerAdjacentTerritories.crbegin()->second,armiesToDeploy));
       int armiesToAdvance = armiesToDeploy + strongestTerritory->second->numberOfArmies;
       this->player->orders->add(new Orders::Advance(this->player,map,playerAdjacentTerritories.crbegin()->second,weakestEnemy->second, armiesToAdvance));
   }

     //create an order from a card
    if (!this->player->hand->cards.empty()) {
        //prioritize the bomb card
        int cardSize = (int) this->player->hand->cards.size();
        bool noBombCard = true;
        for(int i=0;i<cardSize;i++){
            if(this->player->hand->cards[i]->getType() == "bomb"){
                Cards::Card *card = this->player->hand->cards.at(i);
                dynamic_cast<Cards::Bomb *>(card)->play(this->player, deck, toAttack.begin()->second, map);
                noBombCard = false;
                break;
            }
        }
        if (noBombCard) {
            //will not play diplomacy card because the player's goal is to attack enemy territories
            //will not play airlift because the attacking territory always has the most armies
            int randomIndex = std::experimental::randint(0, (int) this->player->hand->cards.size() - 1);
            Cards::Card *card = this->player->hand->cards.at(randomIndex);
            std::string type = card->getType();
            // Play the card based on its type
            if (type == "reinforcement") {
                // play the reinforcement card on the first territory of the territory used to attack
                dynamic_cast<Cards::Reinforcement *>(card)->play(this->player, deck, playerAdjacentTerritories.crbegin()->second);
            }
        }
    }
}

//computer player that never issues any order. If a Neutral player is attacked, it becomes an aggressive player
// ================== Neutral Player Strategy Implementation ==================
//constructor
NeutralPlayerStrategy::NeutralPlayerStrategy(Players::Player *p) : PlayerStrategies(p,PlayerStrategies::NEUTRAL_TYPE) {}
// copy constructor
NeutralPlayerStrategy::NeutralPlayerStrategy(const NeutralPlayerStrategy &neutral) : PlayerStrategies(neutral.player, PlayerStrategies::NEUTRAL_TYPE) {}

// destructor
NeutralPlayerStrategy::~NeutralPlayerStrategy() = default;

// assignment operator
NeutralPlayerStrategy& NeutralPlayerStrategy::operator=(const NeutralPlayerStrategy &neutral){
    if(this ==&neutral) return *this;
    this->player = neutral.player;
    return *this;
}
// stream insertion operator
std::ostream &operator<<(std::ostream &out, const NeutralPlayerStrategy &neutral) {
    out << "Player " << neutral.player->getName() << " is playing the neutral player strategy" << std::endl;
    return out;
}
// method returns empty list of territories todefend since the neutral player does not issue orders
std::multimap<int, Graph::Territory *> NeutralPlayerStrategy::toDefend(const std::vector<Graph::Edge *> &mapEdges){
    std::multimap<int,Graph::Territory *> territoriesToDefend;
    //returns an empty map because a neutral player doesnt issue orders
    return territoriesToDefend;
}
// method returns empty list of territories toAttack since the neutral player does not issue orders
std::multimap<int, Graph::Territory *> NeutralPlayerStrategy::toAttack(const std::vector<Graph::Edge *> &edges) {
    std::multimap<int, Graph::Territory *> territoriesToAttack;
    //returns an empty map because a neutral player doesnt issue orders
    return territoriesToAttack;
}
/**
 * Neutral player does not issue orders and does not play cards
 * @param game the engine running the current game
 */
void NeutralPlayerStrategy::issueOrder(GameEngine &game) {
    auto map = game.mapLoader->map;
    auto deck = game.deck;
    // don't issue orders, play cards
    this->player->toDefend(map->edges);
    this->player->toAttack(map->edges);
    std::cout<<this->player->getName()<<" is not issuing orders because this player is neutral"<<std::endl;
}
  
//  ========================== Human Player Strategy class  ==========================
HumanPlayerStrategy::HumanPlayerStrategy(Players::Player *p) : PlayerStrategies(p, PlayerStrategies::HUMAN_TYPE) {}

// creates shallow copy (avoids cyclic dependency issues)
[[maybe_unused]] HumanPlayerStrategy::HumanPlayerStrategy(const HumanPlayerStrategy &h)
        : PlayerStrategies(h.player, PlayerStrategies::HUMAN_TYPE) {}

// dynamically allocated players should be deleted elsewhere
HumanPlayerStrategy::~HumanPlayerStrategy() = default;

// creates shallow copy via assignment operator (avoids cyclic dependency issues)
HumanPlayerStrategy &HumanPlayerStrategy::operator=(const HumanPlayerStrategy &h) {
    this->player = h.player;
    return *this;
}

// outputs the player's name and strategy via the stream insertion operator
std::ostream &operator<<(std::ostream &out, const HumanPlayerStrategy &h) {
    return out << "Player " << h.player->getName() << " is playing the human player strategy" << endl;
}

/**
 * Displays all territories from the given container along with their id
 * @param territories either toAttack or toDefend lists
 */
void HumanPlayerStrategy::printTerritories(const multimap<int, Territory *> &territories) {
    for (const auto &pair: territories) {
        cout << " [" << pair.first << "] " << pair.second->name << " — owned by " << pair.second->owner->getName()
             << " and has " << pair.second->numberOfArmies << " armies\n";
    }
}

/**
 * Displays all territories in the container, and each of their neighbors that are owned by this object's player
 * @param edges the gameplay map's edges
 * @param territories either toAttack or toDefend lists
 */
void HumanPlayerStrategy::printTerritoriesWithNeighbors(const vector<Edge *> &edges,
                                                        const multimap<int, Territory *> &territories) const {
    for (const auto &pair: territories) {
        cout << " [" << pair.first << "] " << pair.second->name << " — owned by " << pair.second->owner->getName()
             << " and has " << pair.second->numberOfArmies << " armies\n\tAvailable sources for this territory:\n";
        for (const auto &neighbor: pair.second->adjacentTerritoriesOwnedBy(*player, edges)) {
            cout << "\t[" << neighbor->countryNumber << "] " << neighbor->name << " — owned by "
                 << neighbor->owner->getName() << " and has " << neighbor->numberOfArmies << " armies\n";
        }
    }
}

// displays all players (with indices) in the list except this object's player
void HumanPlayerStrategy::printPlayers(const vector<Players::Player *> &players) {
    for (int i = 0; i < players.size(); i++) {
        if (&player[i] != player) {
            cout << " [" << i << "] " << players[i]->getName() << endl;
        }
    }
}

// prompts, reads and returns user input
string HumanPlayerStrategy::readCommand() {
    string readCommandInput;
    cout << "Input command > ";
    getline(cin, readCommandInput);
    return readCommandInput;
}

/**
 * @param command a user command in the valid format (combination of '-t', '-a', and/or '-s' each followed by a number)
 * @param flag flag whose arguments to capture (should be "-t", "-a", or "-s")
 * @return the number that was entered in the command after the flag
 */
int HumanPlayerStrategy::flagArgument(const string &command, const string &flag) {
    regex delimiter; // delimiter is set to ignore all text in between flag's argument
    if (flag == "-t") {
        delimiter.assign(R"(\s*((-s|-a)\s*[0-9]+\s*)*-t\s*)");
    } else if (flag == "-s") {
        delimiter.assign(R"(\s*((-t|-a)\s*[0-9]+\s*)*-s\s*)");
    } else if (flag == "-a") {
        delimiter.assign(R"(\s*((-s|-t)\s*[0-9]+\s*)*-a\s*)");
    }
    sregex_token_iterator it{command.begin(), command.end(), delimiter, -1}; //iterator to first element of split string
    return stoi(*++it); //first element is empty, so it's skipped
}

/**
 * Checks validity of deployment command & prints message if invalid
 * @param command input given by the user during deployment phase
 * @param territories toDefend list, to verify if target id is contained
 * @return true if the command is valid in terms of format and target id
 */
bool HumanPlayerStrategy::validateDeployCommand(const string &command, const multimap<int, Territory *> &territories) {
    if (!regex_match(command, regex(R"((?=.*t.*)(?=.*a.*)(\s*-[t|a]\s*[0-9]+){2}\s*)"))) { // flags can be in any order
        cout << "Invalid format. Try again.\n";
        return false;
    }
    if (!territories.contains(flagArgument(command, "-t"))) {
        cout << "Invalid target id. Try again.\n";
        return false;
    }
    return true;
}

/**
 * Checks validity of advance command & prints message if invalid
 * @param command input given by the user during attack or defend phase
 * @param territories toDefend or toAttack list, to verify if target id is contained
 * @param edges the gameplay map's edges, to verify source id
 * @return true if the command is valid in terms of format and target & source ids
 */
bool HumanPlayerStrategy::validateAdvanceCommand(const string &command, const multimap<int, Territory *> &territories,
                                                 const vector<Edge *> &edges) {
    if (command == "issueorder" || command == "issueorders end") {
        return true;
    } else if (!regex_match(command, regex(R"((?=.*t.*)(?=.*s.*)(?=.*a.*)(\s*-[t|a|s]\s*[0-9]+){3}\s*)"))) {
        cout << "Invalid format. Try again.\n";
        return false;
    }
    auto targetIt = territories.find(flagArgument(command, "-t"));
    if (targetIt == territories.end()) {
        cout << "Invalid target id. Try again.\n";
        return false;
    }
    auto targetNeighbors = targetIt->second->adjacentTerritoriesOwnedBy(*player, edges);
    auto sourceCountryNum = flagArgument(command, "-s");
    auto sourceIt = find_if(targetNeighbors.begin(), targetNeighbors.end(),
                            [sourceCountryNum](Territory *t) { return sourceCountryNum == t->countryNumber; });
    if (sourceIt == targetNeighbors.end()) {
        cout << "Invalid source id. Try again.\n";
        return false;
    }
    return true;
}

/**
 * Checks validity of airlift command & prints message if invalid
 * @param command input given by the user while playing airlift card
 * @param territories toDefend list, to verify if target & source ids are contained
 * @return true if the command is valid in terms of format and target & source ids
 */
bool HumanPlayerStrategy::validateAirliftCommand(string &command, const multimap<int, Territory *> &territories) {
    if (command == "issueorder" || command == "issueorders end") {
        return true;
    } else if (!regex_match(command, regex(R"((?=.*t.*)(?=.*s.*)(?=.*a.*)(\s*-[t|a|s]\s*[0-9]+){3}\s*)"))) {
        cout << "Invalid format. Try again.\n";
        return false;
    } else if (!territories.contains(flagArgument(command, "-t"))) {
        cout << "Invalid target id. Try again.\n";
        return false;
    } else if (!territories.contains(flagArgument(command, "-s"))) {
        cout << "Invalid source id. Try again.\n";
        return false;
    }
    return true;
}

/**
 * Checks validity of bomb, blockade, and reinforcement commands & prints message if invalid
 * @param command input given by the user while playing bomb, blockade, or reinforcement card
 * @param territories toDefend or toAttack list, to verify if target id is contained
 * @return true if the command is valid in terms of format and target id
 */
bool HumanPlayerStrategy::validateIdCommand(const string &command, const multimap<int, Territory *> &territories) {
    if (!command.empty() && std::all_of(command.begin(), command.end(), ::isdigit) &&
        territories.contains(stoi(command)) || command == "issueorder" || command == "issueorders end") {
        return true;
    }
    cout << "Invalid id. Try again.\n";
    return false;
}

/**
 * Checks validity of diplomacy and card choosing commands & prints message if invalid
 * @param command input given by the user while playing diplomacy card or choosing card
 * @param vectorSize the size of the players list or cards list
 * @return true if the command is valid in terms of format and chosen index
 */
bool HumanPlayerStrategy::validateIdCommand(const string &command, size_t vectorSize) {
    if (!command.empty() && std::all_of(command.begin(), command.end(), ::isdigit) && stoi(command) < vectorSize ||
        command == "issueorder" || command == "issueorders end") {
        return true;
    }
    cout << "Invalid id. Try again.\n";
    return false;
}

// checks validity of main menu commands & prints message if invalid
bool HumanPlayerStrategy::validateMenuCommand(const string &command) {
    if (command == "attack" || command == "defend" || command == "card" || command == "issueorders end") {
        return true;
    }
    cout << "Invalid command. Try again.\n";
    return false;
}

// prompts the user for input until they enter a valid command during deployment phase; returns the valid command
string HumanPlayerStrategy::promptValidDeploy(const multimap<int, Territory *> &territories) {
    cout << "Enter flags '-t <targetid>' and '-a <numberofarmies>' to issue a deploy order.\n";
    string userInput = readCommand();
    while (!validateDeployCommand(userInput, territories)) {
        userInput = readCommand();
    }
    return userInput;
}

// prompts the user for input until they enter a valid command during attack/defend state; returns the valid command
string HumanPlayerStrategy::promptValidAdvance(const multimap<int, Territory *> &territories,
                                               const vector<Edge *> &edges) {
    cout << "Enter flags '-t <targetid>', '-s <sourceid>' and '-a <numberofarmies>' to issue an advance order. "
            "Otherwise, enter 'issueorder' to return to the main menu or 'issueorders end' to finish.\n";
    string userInput = readCommand();
    while (!validateAdvanceCommand(userInput, territories, edges)) {
        userInput = readCommand();
    }
    return userInput;
}

// prompts the user for input until they enter a valid command during airlift state; returns the valid command
string HumanPlayerStrategy::promptValidAirlift(const multimap<int, Territory *> &territories) {
    cout << "Enter flags '-t <targetid>', '-s <sourceid>' and '-a <numberofarmies>' to play your Airlift card. "
            "Otherwise, enter 'issueorder' to return to the main menu or 'issueorders end' to finish.\n";
    string userInput = readCommand();
    while (!validateAirliftCommand(userInput, territories)) {
        userInput = readCommand();
    }
    return userInput;
}

// prompts the user for input until they enter a valid command during bomb/blockade/reinforcement state
string HumanPlayerStrategy::promptValidId(const multimap<int, Territory *> &territories) {
    cout << "Enter '<territoryid>' to play your card. "
            "Otherwise, enter 'issueorder' to return to the main menu or 'issueorders end' to finish.\n";
    string userInput = readCommand();
    while (!validateIdCommand(userInput, territories)) {
        userInput = readCommand();
    }
    return userInput;
}

// prompts the user for input until they enter a valid command during diplomacy state; returns the valid command
string HumanPlayerStrategy::promptValidId(const vector<Players::Player *> &players) {
    cout << "Enter '<playerid>' to play your card. "
            "Otherwise, enter 'issueorder' to return to the main menu or 'issueorders end' to finish.\n";
    string userInput = readCommand();
    while (!validateIdCommand(userInput, players.size())) {
        userInput = readCommand();
    }
    return userInput;
}

// prompts the user for input until they enter a valid command when choosing card; returns the valid command
string HumanPlayerStrategy::promptValidId(const vector<Cards::Card *> &hand) {
    cout << "Enter '<cardid>' to select a card. "
            "Otherwise, enter 'issueorder' to return to the main menu or 'issueorders end' to finish.\n";
    string userInput = readCommand();
    while (!validateIdCommand(userInput, hand.size())) {
        userInput = readCommand();
    }
    return userInput;
}

// prompts the user for input until they enter a valid command in the main menu; returns the valid command
string HumanPlayerStrategy::promptValidMenuCommand() {
    cout << "Enter one of the following commands:\n"
            "\t'attack' to issue an advance order to attack\n"
            "\t'defend' to issue an advance order to defend\n"
            "\t'card' to play a card\n"
            "\t'issueorders end' to finish issuing orders\n";
    string userInput = readCommand();
    while (!validateMenuCommand(userInput)) {
        userInput = readCommand();
    }
    return userInput;
}

// issues a single advance order using a valid command
void HumanPlayerStrategy::issueAdvanceOrder(const string &command, const multimap<int, Territory *> &territories,
                                            Map *map) {
    int srcId = flagArgument(command, "-s"), tgtId = flagArgument(command, "-t"), armyNum = flagArgument(command, "-a");
    auto target = territories.find(tgtId)->second;
    auto neighborsOfTgt = target->adjacentTerritoriesOwnedBy(*player, map->edges);
    auto source = *find_if(neighborsOfTgt.begin(), neighborsOfTgt.end(),
                           [srcId](Territory *t) { return srcId == t->countryNumber; });
    player->orders->add(new Orders::Advance(player, map, source, target, armyNum));
}

/**
 * @param edges list of Edge objects present in the map
 * @return list of territories owned by player (keys correspond to the country number)
 */
multimap<int, Territory *> HumanPlayerStrategy::toDefend(const vector<Edge *> &edges) {
    multimap<int, Territory *> territoriesToDefend;
    for (const auto &pair: player->territories) {
        territoriesToDefend.insert(pair);
    }
    return territoriesToDefend;
}

/**
 * @param edges list of Edge objects present in the map
 * @return list of enemy territories adjacent to player's territories (keys correspond to the country number)
 */
multimap<int, Territory *> HumanPlayerStrategy::toAttack(const vector<Edge *> &edges) {
    multimap<int, Territory *> territoriesToAttack;
    for (const auto &pair: player->territories) {
        for (const auto adjacentEnemy: pair.second->adjacentEnemyTerritories(edges)) {
            if (!territoriesToAttack.contains(adjacentEnemy->countryNumber)) {
                territoriesToAttack.insert(std::pair<int, Territory *>(adjacentEnemy->countryNumber, adjacentEnemy));
            }
        }
    }
    return territoriesToAttack;
}

/**
 * Issues orders by allowing player to make decisions based on their toAttack and toDefend lists through console input
 * @param game the engine running the current game
 */
void HumanPlayerStrategy::issueOrder(GameEngine &game) {
    auto map = game.mapLoader->map;
    auto edges = map->edges;
    auto &playerHand = player->hand->cards;
    auto deck = game.deck;
    auto defendList = toDefend(edges);
    auto attackList = toAttack(edges);
    int armiesDeployed = 0;
    string userInput;
    bool firstIteration = true;

    // allow the user to issue only deploy orders until all reinforcements have been used
    while (armiesDeployed < player->reinforcementPool) {
        cout << "You have " << (player->reinforcementPool - armiesDeployed) << " left to deploy.\n";

        // print territory options just once to avoid flooding the console
        if (firstIteration) {
            cout << "Territories you can deploy to:\n";
            printTerritories(defendList);
            firstIteration = false;
        }

        userInput = promptValidDeploy(defendList);
        int countryNum = flagArgument(userInput, "-t"), armyNum = flagArgument(userInput, "-a"); // parse input
        player->orders->add(new Orders::Deploy(player, defendList.find(countryNum)->second, armyNum));
        armiesDeployed += armyNum;
    }

    // allow user to issue advance and card orders until they signify that they are finished
    while (userInput != "issueorders end") {
        userInput = promptValidMenuCommand();
        if (userInput == "attack") { // advance using toDefend list
            cout << "Territories you can attack:\n";
            printTerritoriesWithNeighbors(edges, attackList);

            userInput = promptValidAdvance(attackList, edges);
            while (userInput != "issueorder" && userInput != "issueorders end") {
                issueAdvanceOrder(userInput, attackList, map);
                userInput = promptValidAdvance(attackList, edges);
            }
        } else if (userInput == "defend") { // advance using toAttack list
            cout << "Territories you can defend:\n";
            printTerritoriesWithNeighbors(edges, defendList);

            userInput = promptValidAdvance(defendList, edges);
            while (userInput != "issueorder" && userInput != "issueorders end") {
                issueAdvanceOrder(userInput, defendList, map);
                userInput = promptValidAdvance(defendList, edges);
            }
        } else if (userInput == "card") { // choose card from hand
            player->displayCards();
            if (playerHand.empty()) continue; // player has no cards, return to main menu

            userInput = promptValidId(playerHand);
            if (userInput == "issueorder" || userInput == "issueorders end") continue; // skip canceled order

            Cards::Card *card = playerHand[stoi(userInput)];
            string cardType = card->getType();

            if (cardType == "reinforcement") {
                cout << "Territories you can reinforce:\n";
                printTerritories(defendList);
                userInput = promptValidId(defendList);
                if (userInput == "issueorder" || userInput == "issueorders end") continue; // skip canceled order
                dynamic_cast<Cards::Reinforcement *>(card)->play(player, deck,
                                                                 defendList.find(stoi(userInput))->second);
            } else if (cardType == "bomb") {
                cout << "Territories you can bomb:\n";
                printTerritories(attackList);
                userInput = promptValidId(attackList);
                if (userInput == "issueorder" || userInput == "issueorders end") continue; // skip canceled order
                dynamic_cast<Cards::Bomb *>(card)->play(player, deck, attackList.find(stoi(userInput))->second, map);
            } else if (cardType == "airlift") {
                cout << "Territories you can airlift to and from:\n";
                printTerritories(defendList);

                userInput = promptValidAirlift(defendList);
                if (userInput == "issueorder" || userInput == "issueorders end") continue; // skip canceled order
                dynamic_cast<Cards::Airlift *>(card)->play(player, deck,
                                                           defendList.find(flagArgument(userInput, "-s"))->second,
                                                           defendList.find(flagArgument(userInput, "-t"))->second,
                                                           flagArgument(userInput, "-a"));
            } else if (cardType == "blockade") {
                cout << "Territories you can blockade:\n";
                printTerritories(defendList);
                userInput = promptValidId(defendList);
                if (userInput == "issueorder" || userInput == "issueorders end") continue; // skip canceled order
                dynamic_cast<Cards::Blockade *>(card)->play(player, deck, defendList.find(stoi(userInput))->second,
                                                            game.playersList);
            } else if (cardType == "diplomacy") {
                cout << "Players you can negotiate with:\n";
                printPlayers(game.playersList);
                userInput = promptValidId(game.playersList);
                if (userInput == "issueorder" || userInput == "issueorders end") continue; // skip canceled order
                dynamic_cast<Cards::Diplomacy *>(card)->play(player, game.playersList[stoi(userInput)], deck);
            }
        }
    }
}
