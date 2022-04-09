#include <regex>
#include "PlayerStrategies.h"


using namespace std;
using namespace Graph;

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

void BenevolentPlayerStrategy::issueOrder(const GameEngine &game) {
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

//  ========================== Human Player Strategy class  ==========================
HumanPlayerStrategy::HumanPlayerStrategy(Players::Player *p) : PlayerStrategies(p) {}

HumanPlayerStrategy::HumanPlayerStrategy(const HumanPlayerStrategy &h) : PlayerStrategies(h.player) {}

HumanPlayerStrategy::~HumanPlayerStrategy() = default;

HumanPlayerStrategy &HumanPlayerStrategy::operator=(const HumanPlayerStrategy &h) {
    this->player = h.player;
    return *this;
}

std::ostream &operator<<(std::ostream &out, const HumanPlayerStrategy &h) {
    return out << "Player " << h.player->getName() << " is playing the human player strategy" << endl;
}

string readCommand() {
    string readCommandInput;
    cout << "Input command > ";
    getline(cin, readCommandInput);
    return readCommandInput;
}

// displays all territories from the given map, to be used with toAttack and toDefend lists
void HumanPlayerStrategy::printTerritories(const multimap<int, Territory *> &territories) {
    for (const auto &pair: territories) {
        cout << "[" << pair.first << "] " << pair.second->name << " — owned by " << pair.second->owner->getName()
             << " and has " << pair.second->numberOfArmies << " armies\n";
    }
}

/* displays all territories from the given map with neighbors owned by player,
 * to be used for Advance orders with toAttack and toDefend lists*/
void HumanPlayerStrategy::printTerritoriesWithNeighbors(const vector<Edge *> &edges,
                                                        const multimap<int, Territory *> &territories) const {
    for (const auto &pair: territories) {
        cout << "[" << pair.first << "] " << pair.second->name << " — owned by " << pair.second->owner->getName()
             << " and has " << pair.second->numberOfArmies << " armies\n\tAvailable sources for this territory:\n";
        for (const auto &neighbor: pair.second->adjacentTerritoriesOwnedBy(*player, edges)) {
            cout << "\t[" << neighbor->countryNumber << "] " << neighbor->name << " — owned by "
                 << neighbor->owner->getName() << " and has " << neighbor->numberOfArmies << " armies\n";
        }
    }
}

void printPlayers(const vector<Players::Player *> &players) {
    for (int i = 0; i < players.size(); i++) {
        cout << "[" << i << "] " << players[i]->getName() << endl;
    }
}

bool validateDeploy(string userInput) {
    userInput = " " + userInput;
    if (!regex_match(userInput, regex("(\\s*-[t|a]\\s*[0-9]+)+\\s*"))) {
        cout << "Invalid format. Try again.\n";
        return false;
    }
    size_t tCount = userInput.size() - regex_replace(userInput, regex("t"), "").size();
    size_t aCount = userInput.size() - regex_replace(userInput, regex("a"), "").size();
    if (tCount != aCount) {
        cout << "Every '-t' flag must have its own accompanying '-a' flag. Try again.\n";
        return false;
    }
    return true;
}

bool validateAdv(string userInput) {
    userInput = " " + userInput;
    if (!regex_match(userInput, regex("(\\s*-[t|a|s]\\s*[0-9]+)+\\s*"))) {
        cout << "Invalid format. Try again.\n";
        return false;
    }
    size_t tCount = userInput.size() - regex_replace(userInput, regex("t"), "").size();
    size_t aCount = userInput.size() - regex_replace(userInput, regex("a"), "").size();
    size_t sCount = userInput.size() - regex_replace(userInput, regex("s"), "").size();
    if (tCount != aCount || tCount != sCount) {
        cout << "Every '-t' flag must have its own accompanying '-s' and '-a' flags. Try again.\n";
        return false;
    }
    return true;
}

/**
 * @param command a user command in the valid format
 * @param flag flag whose arguments to capture (should be "-t", "-a", or "-s")
 * @return a vector containing each flag's argument from command
 */
vector<int> flagArguments(const string &command, const string &flag) {
    vector<int> arguments;
    regex delimiter;
    
    // delimiter is set to ignore all text in between flag's argument
    if (flag == "-t") {
        delimiter.assign(R"(\s*((-s|-a)\s*[0-9]+\s*)*-t\s*)");
    } else if (flag == "-s") {
        delimiter.assign(R"(\s*((-t|-a)\s*[0-9]+\s*)*-s\s*)");
    } else if (flag == "-a") {
        delimiter.assign(R"(\s*((-s|-t)\s*[0-9]+\s*)*-a\s*)");
    } else {
        return arguments;
    }

    // get arguments by iterating over string split by delimiter
    sregex_token_iterator end;
    sregex_token_iterator start{command.begin(), command.end(), delimiter, -1};
    for (auto it = ++start; it != end; it++) { // skip empty first element
        arguments.push_back(stoi(*it)); // convert each argument to integer 
    }

    return arguments;
}

bool validateCard(const string &command, const Players::Player *player) {
    if (command == "issueorder") {
        return true;
    }
    if (!command.empty() && std::all_of(command.begin(), command.end(), ::isdigit)) { // command is positive numeric
        if (stoi(command) < player->hand->cards.size()) { // index exists in player's cards
            return true;
        } else {
            cout << "Invalid card id. Try again.\n";
        }
    } else {
        cout << "Invalid format. Try again.\n";
    }
    return false;
}

Territory* promptValidTarget(const multimap<int, Territory *> &territories) {
    while (true) {
        string userInput = readCommand();
        if (regex_match(userInput, regex(R"(\s*-t\s*[0-9]+\s*)"))) {
            regex num("[0-9]+");
            sregex_token_iterator id{userInput.begin(), userInput.end(),num, 0};
            auto countryIt = territories.find(stoi(*id));
            if (countryIt != territories.end()) {
                return countryIt->second;
            } else {
                cout << "Invalid target id. Try again.\n";
            }
        } else {
            cout << "Invalid format. Try again.\n";
        }
    }
}

Players::Player* promptValidPlayer(const vector<Players::Player *> &players) {
    while (true) {
        string userInput = readCommand();
        if (regex_match(userInput, regex(R"(\s*-t\s*[0-9]+\s*)"))) {
            regex num("[0-9]+");
            sregex_token_iterator id{userInput.begin(), userInput.end(),num, 0};
            int index = stoi(*id);
            if (index < players.size()) {
                return players[index];
            } else {
                cout << "Invalid target id. Try again.\n";
            }
        } else {
            cout << "Invalid format. Try again.\n";
        }
    }
}

// change this
void promptValidAirlift(const multimap<int, Territory *> &territories, Players::Player *player, Cards::Card *card, const GameEngine &game) {
    while (true) {
        string userInput = readCommand();
        if (regex_match(userInput, regex(R"((?=.*t.*)(?=.*s.*)(?=.*a.*)(\s*-[t|a|s]\s*[0-9]+){3}\s*)"))) {
            auto sourceIt = territories.find(flagArguments(userInput, "-s")[0]);
            auto targetIt = territories.find(flagArguments(userInput, "-t")[0]);

            if (targetIt == territories.end()) {
                cout << "Invalid target id. Try again.\n";
            } else if (sourceIt == territories.end()) {
                cout << "Invalid source id. Try again.\n";
            } else {
                int armies = flagArguments(userInput, "-a")[0];
                dynamic_cast<Cards::Airlift *>(card)->play(player, game.deck, sourceIt->second, targetIt->second, armies);
                return;
            }
        } else {
            cout << "Invalid format. Try again.\n";
        }
    }
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
 * Issues orders by allowing player to make decisions based on their toAttack and toDefend lists
 * @param game the engine running the current game
 */
void HumanPlayerStrategy::issueOrder(const GameEngine &game) {
    auto edges = game.mapLoader->map->edges;
    auto defendList = toDefend(edges);
    auto attackList = toAttack(edges);
    int armiesDeployed = 0;
    string userInput;
    cout << "------ " << player->getName() << "'s turn to issue orders ------\n";

    while (armiesDeployed < player->reinforcementPool) {
        cout << "You have " << (player->reinforcementPool - armiesDeployed) << " left to deploy.\n";
        if (armiesDeployed == 0 && userInput.empty()) {
            cout << "Territories you can deploy to:\n";
            printTerritories(defendList);
        }
        cout << "Enter flags '-t <targetid>' and '-a <numberofarmies>' to issue a deploy order. "
                "You can create multiple orders by inputting multiple sets of these flags.\n";
        userInput = readCommand();
        while (!validateDeploy(userInput)) {
            userInput = readCommand();
        }
        auto countryNums = flagArguments(userInput, "-t");
        auto armyNums = flagArguments(userInput, "-a");
        auto numOfOrders = countryNums.size();
        for (int i = 0; i < numOfOrders; i++) {
            auto countryIt = defendList.find(countryNums[i]);
            if (countryIt != defendList.end()) {
                player->orders->add(new Orders::Deploy(player, (*countryIt).second, armyNums[i]));
                if (armyNums[i] >= 1) {
                    armiesDeployed += armyNums[i];
                }
            } else {
                cout << "Target id " << countryNums[i] << " does not exist.";
            }
            cout << endl;
        }
    }

    while (true) {
        cout << "Enter one of the following commands:\n"
                "\t'attack' to issue an advance order to attack\n"
                "\t'defend' to issue an advance order to defend\n"
                "\t'card' to play a card\n"
                "\t'issueorders end' to finish issuing orders\n";
        userInput = readCommand();
        if (userInput == "attack") {
            printTerritoriesWithNeighbors(edges, toAttack(edges)); //find way to print only 1st time

            cout << "Enter flags '-t <targetid>', '-s <sourceid>' and '-a <numberofarmies>' to issue an advance order. "
                    "You can create multiple orders by inputting multiple sets of these flags.\n";
            userInput = readCommand();
            while (!validateAdv(userInput)) {
                userInput = readCommand();
            }
            auto srcIds = flagArguments(userInput, "-s");
            auto tgtIds = flagArguments(userInput, "-t");
            auto armyNums = flagArguments(userInput, "-a");
            auto numOfOrders = srcIds.size();
            for (int i = 0; i < numOfOrders; i++) {
                auto targetIt = attackList.find(tgtIds[i]);
                if (targetIt != attackList.end()) {
                    auto neighborsOfTgt = (*targetIt).second->adjacentTerritoriesOwnedBy(*player, edges);
                    auto sourceIt = find_if(neighborsOfTgt.begin(), neighborsOfTgt.end(),
                                            [srcIds, i](Territory *t) { return srcIds[i] == t->countryNumber; });
                    if (sourceIt != neighborsOfTgt.end()) {
                        player->orders->add(new Orders::Advance(player, game.mapLoader->map, *sourceIt,
                                                                (*targetIt).second, armyNums[i]));
                    } else {
                        cout << "Source id " << srcIds[i] << " does not exist.";
                    }
                } else {
                    cout << "Target id " << tgtIds[i] << " does not exist.";
                }
                cout << endl;
            }
        } else if (userInput == "defend") {
            printTerritoriesWithNeighbors(edges, toDefend(edges)); //find way to print only 1st time

            cout << "Enter flags '-t <targetid>', '-s <sourceid>' and '-a <numberofarmies>' to issue an advance order. "
                    "You can create multiple orders by inputting multiple sets of these flags.\n";
            userInput = readCommand();
            while (!validateAdv(userInput)) {
                userInput = readCommand();
            }
            auto srcIds = flagArguments(userInput, "-s");
            auto tgtIds = flagArguments(userInput, "-t");
            auto armyNums = flagArguments(userInput, "-a");
            auto numOfOrders = srcIds.size();
            for (int i = 0; i < numOfOrders; i++) {
                auto targetIt = defendList.find(tgtIds[i]);
                if (targetIt != defendList.end()) {
                    auto neighborsOfTgt = (*targetIt).second->adjacentTerritoriesOwnedBy(*player, edges);
                    auto sourceIt = find_if(neighborsOfTgt.begin(), neighborsOfTgt.end(),
                                            [srcIds, i](Territory *t) { return srcIds[i] == t->countryNumber; });
                    if (sourceIt != neighborsOfTgt.end()) {
                        player->orders->add(new Orders::Advance(player, game.mapLoader->map, *sourceIt,
                                                                (*targetIt).second, armyNums[i]));
                    } else {
                        cout << "Source id " << srcIds[i] << " does not exist for target id " << tgtIds[i] << ".";
                    }
                } else {
                    cout << "Target id " << tgtIds[i] << " does not exist.";
                }
                cout << endl;
            }
        } else if (userInput == "card") {
            if (!player->hand->cards.empty()) {
                player->displayCards();
                cout << "Enter the card id you wish to play or enter 'issueorder' to return to the main menu.\n";
                userInput = readCommand();
                while (!validateCard(userInput, player)) {
                    userInput = readCommand();
                }
                if (userInput != "issueorder") {
                    Cards::Card *card = player->hand->cards[stoi(userInput)];
                    string cardType = card->getType();

                    if (cardType == "reinforcement") {
                        cout << "Territories you can reinforce:\n";
                        printTerritories(defendList);
                        cout << "Enter flag '-t <targetid>' to play your Reinforcement card.\n";
                        auto target = promptValidTarget(defendList);
                        dynamic_cast<Cards::Reinforcement *>(card)->play(player, game.deck, target);
                    } else if (cardType == "bomb") {
                        cout << "Territories you can bomb:\n";
                        printTerritories(attackList);
                        cout << "Enter flag '-t <targetid>' to play your Bomb card.\n";
                        auto target = promptValidTarget(attackList);
                        dynamic_cast<Cards::Bomb *>(card)->play(player, game.deck, target, game.mapLoader->map);
                    } else if (cardType == "airlift") {
                        cout << "Territories you can airlift to and from:\n";
                        printTerritories(defendList);
                        cout << "Enter flags '-t <targetid>', '-s <sourceid>' and '-a <numberofarmies>' to play your "
                                "Airlift card.\n";
                        promptValidAirlift(defendList, player, card, game);
//                        dynamic_cast<Cards::Airlift *>(card)->play(player, game.deck, nullptr, nullptr, 0); //change
                    } else if (cardType == "blockade") {
                        cout << "Territories you can blockade:\n";
                        printTerritories(defendList);
                        cout << "Enter flag '-t <targetid>' to play your Blockade card.\n";
                        auto target = promptValidTarget(defendList);
                        dynamic_cast<Cards::Blockade *>(card)->play(player, game.deck, target);
                    } else if (cardType == "diplomacy") {
                        cout << "Players you can negotiate with:\n";
                        printPlayers(game.playersList);
                        cout << "Enter flag '-t <targetid>' to play your Diplomacy card.\n";
                        auto target = promptValidPlayer(game.playersList);
                        dynamic_cast<Cards::Diplomacy *>(card)->play(player, target, game.deck);
                    }
                }
            } else {
                cout << "You have no cards.\n";
            }
        } else if (userInput == "issueorders end") {
            break;
        } else {
            cout << "Invalid input. Try again.";
        }
    }
}