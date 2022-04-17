#include "GameEngine.h"
#include <filesystem>
#include <utility>
#include <experimental/random>
#include "../PlayerStrategies/PlayerStrategies.h"


#ifdef _WIN32 // any windows system
#define PLATFORM "windows"
#else // any linux distribution
#define PLATFORM "unix"
#endif

// returns the current game phase
string GameEngine::getState() const {
    return this->state;
}

// dynamically instantiates a filled deck, and empty mapLoader and processor
GameEngine::GameEngine() {
    mapLoader = new Graph::MapLoader();
    processor = new CommandProcessor();
    deck = new Cards::Deck();
    deck->fillDeckWithCards();
}

// creates deep copy of processor & deck, and shallow copy of mapLoader & playersList
GameEngine::GameEngine(const GameEngine &game) {
    this->mapLoader = game.mapLoader;
    this->processor = new CommandProcessor(); // generate a new command processor
    this->playersList = game.playersList;
    this->deck = new Cards::Deck(*game.deck);
}

// returns a string of the current state to be output to the log file when notified
string GameEngine::stringToLog() const {
    return "Game engine state: " + this->state;
}

// creates copy via assignment operator
GameEngine &GameEngine::operator=(const GameEngine &gameEngine) {
    if (this == &gameEngine) return *this;
    delete this->mapLoader;
    delete this->deck;
    for (auto *player: this->playersList) {
        delete player;
    }
    delete this->processor;
    this->mapLoader = gameEngine.mapLoader;
    this->processor = new CommandProcessor();
    this->playersList = gameEngine.playersList;
    this->deck = new Cards::Deck(*gameEngine.deck);
    return *this;
}

// outputs the num of players via the stream insertion operator
ostream &operator<<(ostream &out, const GameEngine &gameEngine) {
    out << "Nbr of players: " << gameEngine.playersList.size() << endl;
    return out;
}

// method to set and output current game state
void GameEngine::changeState(string changedState) {
    this->state = std::move(changedState);
    cout << "\n========== state = " << this->state << " ==========" << endl;
    notify(*this);
}

//=============Start state ================

// runs the whole startup phase: map selection & validation, player addition, and distribution of territories
void GameEngine::startupPhase() {
    changeState("start");
    string welcomeBanner =
            "WWWWWWWW                           WWWWWWWW                                                                                                          \n"
            "W::::::W                           W::::::W                                                                                                          \n"
            "W::::::W                           W::::::W                                                                                                          \n"
            "W::::::W                           W::::::W                                                                                                          \n"
            " W:::::W           WWWWW           W:::::Waaaaaaaaaaaaa  rrrrr   rrrrrrrrr   zzzzzzzzzzzzzzzzz   ooooooooooo   nnnn  nnnnnnnn        eeeeeeeeeeee    \n"
            "  W:::::W         W:::::W         W:::::W a::::::::::::a r::::rrr:::::::::r  z:::::::::::::::z oo:::::::::::oo n:::nn::::::::nn    ee::::::::::::ee  \n"
            "   W:::::W       W:::::::W       W:::::W  aaaaaaaaa:::::ar:::::::::::::::::r z::::::::::::::z o:::::::::::::::on::::::::::::::nn  e::::::eeeee:::::ee\n"
            "    W:::::W     W:::::::::W     W:::::W            a::::arr::::::rrrrr::::::rzzzzzzzz::::::z  o:::::ooooo:::::onn:::::::::::::::ne::::::e     e:::::e\n"
            "     W:::::W   W:::::W:::::W   W:::::W      aaaaaaa:::::a r:::::r     r:::::r      z::::::z   o::::o     o::::o  n:::::nnnn:::::ne:::::::eeeee::::::e\n"
            "      W:::::W W:::::W W:::::W W:::::W     aa::::::::::::a r:::::r     rrrrrrr     z::::::z    o::::o     o::::o  n::::n    n::::ne:::::::::::::::::e \n"
            "       W:::::W:::::W   W:::::W:::::W     a::::aaaa::::::a r:::::r                z::::::z     o::::o     o::::o  n::::n    n::::ne::::::eeeeeeeeeee  \n"
            "        W:::::::::W     W:::::::::W     a::::a    a:::::a r:::::r               z::::::z      o::::o     o::::o  n::::n    n::::ne:::::::e           \n"
            "         W:::::::W       W:::::::W      a::::a    a:::::a r:::::r              z::::::zzzzzzzzo:::::ooooo:::::o  n::::n    n::::ne::::::::e          \n"
            "          W:::::W         W:::::W       a:::::aaaa::::::a r:::::r             z::::::::::::::zo:::::::::::::::o  n::::n    n::::n e::::::::eeeeeeee  \n"
            "           W:::W           W:::W         a::::::::::aa:::ar:::::r            z:::::::::::::::z oo:::::::::::oo   n::::n    n::::n  ee:::::::::::::e  \n"
            "            WWW             WWW           aaaaaaaaaa  aaaarrrrrrr            zzzzzzzzzzzzzzzzz   ooooooooooo     nnnnnn    nnnnnn    eeeeeeeeeeeeee  ";

    cout << welcomeBanner << endl << endl;
    cout << "Let's begin!" << endl;
    mapLoadedStateChange();
    chooseMapToLoad();
    mapValidatedStateChange();
    addPlayer();
    gameStart();
    assignReinforcementStateChange();
}

// fairly assigns territories, gives 50 armies & 2 cards to each player, and randomizes play order
void GameEngine::gameStart() {
    while (true) {
        cout
                << "Use the 'gamestart' command to distribute territories, determine play order, assign initial armies and draw cards from the deck"
                << endl;
        Command command = processor->getCommand();
        if (!processor->validate(command.command, *this)) {
            cout << "Invalid command! Please try again." << endl;
            command.saveEffect("Invalid command! Please try again.");
            continue;
        } else {
            command.saveEffect("Assign Reinforcement");
            break;
        }
    }

    cout << endl;
    cout << "Assigning territories to players..." << endl;
    // copy all territory pointers into another vector in order to randomly distribute territories to players
    vector<Graph::Territory *> territories = this->mapLoader->map->territories;
    int nbrOfTerritories = static_cast<int>(territories.size());
    while (!territories.empty()) {
        for (auto *player: this->playersList) {
            if (nbrOfTerritories == 0) break;
            int random;
            if (nbrOfTerritories == 1) random = 0;
            else random = std::experimental::randint(0, nbrOfTerritories - 1);
            territories.at(random)->transferOwnership(player);
            if (random != nbrOfTerritories) territories.erase(territories.begin() + random);
            else territories.erase(territories.end() - 1);
            nbrOfTerritories = static_cast<int>(territories.size());
        }
    }

    // Randomize the order of play of players
    cout << "Assigning play order..." << endl;
    auto rd = std::random_device{};
    auto rng = std::default_random_engine{rd()};
    std::shuffle(this->playersList.begin(), this->playersList.end(), rng);

    cout << "Assigning 50 initial armies & 2 cards to each player..." << endl;
    for (auto *player: this->playersList) {
        player->reinforcementPool = 50;
        player->hand->cards.push_back(this->deck->draw());
        player->hand->cards.push_back(this->deck->draw());
    }
}

//=============Map Loaded state =================
// method to modify current game state to 'map loaded'
void GameEngine::mapLoadedStateChange() {
    GameEngine::changeState("maploaded");
}

// validates the file and checks if the map is a connected graph
void GameEngine::chooseMapToLoad() const {
    bool validateFile = false;
    bool mapIsValid = false;
    while (!validateFile || !mapIsValid) {
        cout << "Use the 'loadmap <filename>' command to choose among the available maps: " << endl;
        string mapPaths{"../WarzoneMaps"};
        int count{1};
        for (const auto &dir: filesystem::directory_iterator(mapPaths)) {
            string token = dir.path().string();
            string mapName;
            if (PLATFORM == "windows")
                mapName = token.substr(token.find_last_of('\\') + 1, token.length());
            else
                mapName = token.substr(token.find_last_of('/') + 1, token.length());
            cout << count << ". " << mapName << endl;
            count++;
        }
        Command command = processor->getCommand();
        if (!processor->validate(command.command, *this)) {
            string effect = "Invalid command! Please try again.";
            cout << effect << endl;
            command.saveEffect(effect);
            continue;
        }
        // extract name of the chosen map
        string mapName = command.command.substr(command.command.find_last_of(' ') + 1, command.command.length());
        validateFile = this->mapLoader->loadMap("../WarzoneMaps/" + mapName + "/" + mapName + ".map");

        command.saveEffect("Maploaded");

        if (validateFile) {
            do {
                cout << "Use the 'validatemap' command to validate that the chosen map is a connected graph" << endl;
                Command validateMapCommand = processor->getCommand();
                bool validCommand = processor->validate(validateMapCommand.command, *this);
                if (!validCommand) {
                    string effect = "Invalid command! Please try again.";
                    cout << effect << endl;
                    validateMapCommand.saveEffect(effect);
                    continue;
                } else break;
            } while (true);
            if (mapLoader->map->validate()) {
                cout << "The map is a connected graph and can be played!" << endl;
                mapIsValid = true;
                continue;
            } else {
                delete mapLoader->map;
                mapLoader->map = new Graph::Map();
            }
        }
    }
    cout << "The file has been loaded and validated! Moving to the next step" << endl;
}

//============= Map validated state =================
// method to modify current game state to 'map validated'
void GameEngine::mapValidatedStateChange() {
    GameEngine::changeState("mapvalidated");
}

//=============players added state =================
// method to modify current game state to 'players added'
void GameEngine::playersAddedStateChange() {
    GameEngine::changeState("playersadded");
}

// method to handle player addition
void GameEngine::addPlayer() {
    int playerCount;
    while (true) {
        cout << "2-6 players are allowed to play." << endl;
        cout << "How many players would you like to add? ";
        cin >> playerCount;
        if (cin.fail()) {
            cout << "Please enter an integer" << endl;
            cin.clear();
            // discard 'bad' character(s)
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        if (playerCount < 2 || playerCount > 6) {
            cout << "Invalid number of players. Please try again." << endl;
        } else {
            cin.ignore();
            break;
        }
    }
    playersAddedStateChange();

    int count{0};
    while (count < playerCount) {
        cout << "Use the 'addplayer <player name>' command to enter players in the game" << endl;
        Command command = processor->getCommand();
        if (!processor->validate(command.command, *this)) {
            string effect = "Invalid command! Please try again.";
            cout << effect << endl;
            command.saveEffect(effect);
            continue;
        } else {
            string choice;
            while (true) {
                cout << "What kind of player would you like to be? " << endl;
                cout << "1. Benevolent player" << endl;
                cout << "2. Cheater player" << endl;
                cout << "3. Neutral player" << endl;
                cout << "4. Aggressive player" << endl;
                cout << "5. Human player" << endl;

                cout << "Enter choice: ";
                cin >> choice;
                if (choice == "1" || choice == "2" || choice == "3" || choice == "4" || choice == "5") break;
                else cout << "Invalid input! Please enter a valid choice." << endl;
            }
            cin.ignore();
            auto *p = new Players::Player(
                    command.command.substr(command.command.find_last_of(' ') + 1, command.command.length()));
            PlayerStrategies *ps;

            if (choice == "1") ps = new BenevolentPlayerStrategy(p);
            else if (choice == "2") ps = new CheaterPlayerStrategy(p);
            else if (choice == "3") ps = new NeutralPlayerStrategy(p);
            else if (choice == "4") ps = new AggressivePlayerStrategy(p);
            else if (choice == "5") ps = new HumanPlayerStrategy(p);

            p->ps = ps; // assign player strategy to player
            this->playersList.push_back((p));
            count++;
            command.saveEffect("playersadded");
        }
    }
}

//==== main game loop: reinforcement, order issuing & execution phases ====
bool GameEngine::mainGameLoop(int maxNumberOfTurns) {
    bool playerWon = false;
    int turnCount = 0;
    while (!playerWon) {
        turnCount++;
        if (turnCount == maxNumberOfTurns + 1) {
            if (maxNumberOfTurns == 500)
                cout << "* ERROR: Too many turns. Main game loop terminated to avoid infinite loop. *\n" << endl;
            else
                cout << "* " << maxNumberOfTurns << " turns have been played. Game concludes as a draw. *\n" << endl;
            break;
        }
        cout << " —▶ TURN " << turnCount << endl;
        reinforcementPhase();
        issueOrdersPhase();
        playerWon = executeOrdersPhase(); //player win and player eliminations are determined in the execution phase
    }
    return playerWon;
}

//=============assign reinforcement state =================
//method to modify current game state to 'assign reinforcement'
void GameEngine::assignReinforcementStateChange() {
    GameEngine::changeState("assign reinforcement");
}

// fill all players' reinforcement pools for the turn according to the territories they own
void GameEngine::reinforcementPhase() {
    bool firstTurn = std::all_of(playersList.begin(), playersList.end(),
                                 [](Players::Player *p) { return p->reinforcementPool == 50; });
    if (!firstTurn) {
        assignReinforcementStateChange();

        // reset each player's reinforcement pool to 0
        for_each(playersList.begin(), playersList.end(), [](Players::Player *p) { p->reinforcementPool = 0; });
    }

    for (auto player: playersList) {
        int territoriesRate = static_cast<int>(player->territories.size() / 3);
        int continentsBonus = 0;
        vector<string> continentsOwned;
        for (const auto &continent: mapLoader->map->continents) {
            // find if player owns any whole continent and honor bonus reinforcements accordingly
            bool ownsContinent = true;
            for (auto continentTerritory: continent->territories) {
                if (!player->territories.contains(continentTerritory->countryNumber)) {
                    ownsContinent = false;
                    break;
                }
            }
            if (ownsContinent) {
                continentsBonus += continent->bonusValue;
                continentsOwned.push_back(continent->name);
            }
        }

        // add total armies for the turn
        player->reinforcementPool += territoriesRate + continentsBonus;
        int minimumBonus = 3 - player->reinforcementPool;
        if (minimumBonus > 0) {
            player->reinforcementPool += minimumBonus; // the minimum reinforcements a player receives is 3
        }

        // display summary
        cout << player->getName() << " has a total of " << player->reinforcementPool << " armies this turn:\n";
        if (firstTurn) {
            cout << "\t• 50 armies from the initial bonus\n";
        }
        cout << "\t• " << territoriesRate << " armies from owning " << player->territories.size() << " territories\n"
             << "\t• " << continentsBonus << " armies from owning " << continentsOwned.size() << " continents";
        string delimiter = ": ";
        for (const auto &continentName: continentsOwned) {
            cout << delimiter << continentName;
            delimiter = ", ";
        }
        cout << endl;
        if (minimumBonus > 0) {
            cout << "\t• " << minimumBonus << " bonus armies to meet the minimum of 3 armies per turn\n";
        }
    }
}

//=============issue orders state =================
//method to modify current game state to 'issue orders'
void GameEngine::issueOrdersStateChange() {
    GameEngine::changeState("issue orders");
}

// issue all players' orders for the turn
void GameEngine::issueOrdersPhase() {
    issueOrdersStateChange();

    for (auto player: playersList) {
        cout << "\n———→ " << player->getName() << "'s turn to issue orders using " << player->ps->strategyType
             << " strategy\n";
        player->issueOrder(*this);
    }
}

//=============execute orders state =================
//method to modify current game state to 'execute orders'
void GameEngine::executeOrdersStateChange() {
    GameEngine::changeState("execute orders");
}

/* executes all issued orders in round-robin fashion across the players, prioritizing deploy orders
 * returns true if a player won during the turn*/
bool GameEngine::executeOrdersPhase() {
    executeOrdersStateChange();

    int skipStrike = 0; //the number of non-deploy orders skipped in a row
    int numOfPlayers = static_cast<int>(playersList.size());
    bool doneDeploying = false;

    while (ordersRemain()) {
        for (int i = 0; i < playersList.size();) {
            Players::Player *player = playersList[i];
            Orders::Order *topOrder = player->orders->element(0);

            //deployment is confirmed to be completed once every player's top order was found to not be of deploy type
            if (skipStrike == numOfPlayers) {
                doneDeploying = true;
            }

            if (player->orders->length() != 0 && (doneDeploying || topOrder->type == "deploy")) {
                skipStrike = 0;

                //save the targeted territory's owner to check if they should be eliminated afterwards
                long targetPlayerIndex = -1;
                if (topOrder->type == "advance") {
                    Orders::Advance &advanceOrder = (dynamic_cast<Orders::Advance &>(*topOrder));
                    if (advanceOrder.target != nullptr) { // avoid runtime error
                        targetPlayerIndex =
                                std::find(playersList.begin(), playersList.end(), advanceOrder.target->owner) -
                                playersList.begin();
                    }
                }

                topOrder->execute();
                player->orders->remove(0);

                //player won if they own all territories
                if (player->territories.size() == mapLoader->map->territories.size()) {
                    cout << player->getName() << " won!" << endl;
                    //loop to remove the last losing player from the playerList, ensuring playerList ends with a size of 1
                    for (int j = 0; j < playersList.size(); j++)
                        if (player->getName() != playersList.at(j)->getName()) {
                            playersList.at(j)->isEliminated = true;
                            eliminatedPlayers.push_back(playersList.at(j));
                            playersList.erase(playersList.begin() + j);
                            j--;
                        }
                    return true;
                }

                //target player is eliminated if their last territory was taken after an advance order
                if (targetPlayerIndex != -1 && playersList[targetPlayerIndex]->territories.empty()
                    && playersList[targetPlayerIndex] != player) {
                    cout << " → " << playersList[targetPlayerIndex]->getName() << " is eliminated." << endl;
                    playersList[targetPlayerIndex]->isEliminated = true;
                    eliminatedPlayers.push_back(playersList[targetPlayerIndex]);
                    playersList.erase(playersList.begin() + targetPlayerIndex);
                }

                //player eliminates self if they played a blockade on their last territory
                if (player->territories.empty()) {
                    cout << " → " << player->getName() << " is eliminated." << endl;
                    player->isEliminated = true;
                    eliminatedPlayers.push_back(player);
                    playersList.erase(playersList.begin() + i);
                } else {
                    i++;
                }
            } else {
                skipStrike++;
                i++;
            }
        }
    }

    for (const auto &player: playersList) {
        player->cannotAttack.clear(); //reset each player's negotiation list at the end of the turn

        if (player->receivesCard) {
            //give card to all players who conquered a territory this turn
            Cards::Card *awardedCard = deck->draw();
            player->hand->cards.push_back(awardedCard);
            cout << player->getName() << " received a " << awardedCard->getType() << " card" << endl;
            player->receivesCard = false;
        }
    }
    cout << endl;
    return false;
}

// returns whether any of the players have orders left in their order list
bool GameEngine::ordersRemain() {
    if (std::all_of(playersList.begin(), playersList.end(),
                    [](Players::Player *p) { return p->orders->length() == 0; })) {
        return false;
    }
    return true;
}

//=============win state =================
//method to modify current game state to 'win'
void GameEngine::winStateChange() {
    GameEngine::changeState("win");
    cout << "Congratulations! You are the winner of this game!" << endl;
}

// runs all games based on a tournament command and records a summary of the results
void GameEngine::tournamentMode(Command &command) {
    string tournamentCommand = command.command;
    vector<string> maps;
    vector<string> playerStrategies;
    vector<pair<string, string>> winners;
    int noOfGames;
    int maxNoOfTurns;

    // get maps from command
    string lineBetween = tournamentCommand.substr(tournamentCommand.find("-M") + 2,
                                                  tournamentCommand.find("-P") - tournamentCommand.find("-M") - 2);
    istringstream parse2(lineBetween);
    while (parse2 >> lineBetween) {
        maps.push_back(lineBetween);
    }

    // get player strats from command
    lineBetween = tournamentCommand.substr(tournamentCommand.find("-P") + 2,
                                           tournamentCommand.find("-G") - tournamentCommand.find("-P") - 2);
    istringstream parse3(lineBetween);
    while (parse3 >> lineBetween) {
        playerStrategies.push_back(lineBetween);
    }

    // get no of games from command
    lineBetween = tournamentCommand.substr(tournamentCommand.find("-G") + 2,
                                           tournamentCommand.find("-D") - tournamentCommand.find("-G") - 2);
    noOfGames = std::stoi(lineBetween);

    // get no of turns from command
    lineBetween = tournamentCommand.substr(tournamentCommand.find("-D") + 2);
    maxNoOfTurns = std::stoi(lineBetween);

    // play each maps by noOfGames times
    for (const string &map: maps) {
        for (int i = 0; i < noOfGames; i++) {
            cout << "<<<<<<<<<<<<<<<" << " Game number: " << i + 1 << " >>>>>>>>>>>>>>>" << endl;

            changeState("start");
            if (!this->mapLoader->loadMap("../WarzoneMaps/" + map + "/" + map +
                                          ".map")) { //checking that map has loaded before proceeding with the game
                cout << map + ".map is an invalid map file." << endl;
                return;
            }

            mapLoadedStateChange();
            if (!mapLoader->map->validate()) { //validating map before proceeding with the game
                cout << map + " is an invalid map." << endl;
                return;
            }

            mapValidatedStateChange();

            playersAddedStateChange();
            int playerNumber = 1; //parameter to store a player number used in player naming
            for (const string &player: playerStrategies) {
                auto *p = new Players::Player("Player " + to_string(playerNumber++));
                if (player == "Benevolent")
                    p->ps = new BenevolentPlayerStrategy(p);
                else if (player == "Aggressive")
                    p->ps = new AggressivePlayerStrategy(p);
                else if (player == "Neutral")
                    p->ps = new NeutralPlayerStrategy(p);
                else if (player == "Cheater")
                    p->ps = new CheaterPlayerStrategy(p);
                this->playersList.push_back((p));
            }

            streambuf *orig = std::cin.rdbuf(); //saving original cin state
            cin.rdbuf(istringstream(
                    "gamestart").rdbuf()); //injecting "gamestart" string into cin to be read by gameStart() method
            gameStart(); //calling gameStart() method to assign territories and cards to players
            cin.rdbuf(orig); //setting cin to its original state; runtime error occurs otherwise

            assignReinforcementStateChange();
            if (mainGameLoop(maxNoOfTurns)) //if a winner is declared, the win state change is invoked
                winStateChange();

            // name of the winner is put in the winners vector if there's only one player left in the list, otherwise put Draw in the vector
            if (playersList.size() == 1)
                winners.emplace_back(playersList.at(0)->getName(), playersList.at(0)->ps->strategyType);
            else
                winners.emplace_back("Draw", "Draw");

            // fully reset game in preparation for the next round
            delete mapLoader;
            delete processor;
            delete deck;
            for (auto *p: playersList) {
                delete p;
            }
            for (auto *p: eliminatedPlayers) {
                delete p;
            }
            playersList.clear();
            eliminatedPlayers.clear();
            mapLoader = new Graph::MapLoader();
            processor = new CommandProcessor();
            deck = new Cards::Deck();
            deck->fillDeckWithCards();
        }
    }

    fstream output;
    output.open("gamelog.txt", std::ios_base::app | std::ios_base::in);

    // tournament settings
    output << "Tournament mode: " << endl;
    output << "M: ";
    for (const string &map: maps)
        output << map << ", ";
    output << endl;
    output << "P: ";
    for (const string &player: playerStrategies)
        output << player << ", ";
    output << endl;
    output << "G: " << noOfGames << endl;
    output << "D: " << maxNoOfTurns << endl << endl << "|";

    int noOfColumns = noOfGames + 1;
    int width = 14;
    string indent = " ";
    char filler1 = ' ';
    char filler2 = '-';

    // for resetting output format
    ios init(NULL);
    init.copyfmt(output);

    // header row of the table
    for (int i = 0; i < noOfColumns; i++)
        output << left << setw(width) << setfill(filler2) << "" << "|";
    output << endl << "|";

    for (int i = 0; i < noOfColumns; i++) {
        if (i == 0) {
            output << left << setw(width) << setfill(filler1) << "" << "|";
        } else
            output << left << setw(width) << setfill(filler1) << (indent + "Game " + to_string(i)) << "|";
    }
    output << endl << "|";
    for (int i = 0; i < noOfColumns; i++)
        output << left << setw(width) << setfill(filler2) << ("") << "|";
    output << endl;

    output.copyfmt(init);

    // consequent rows of the table
    auto winnerName = winners.begin();
    auto mapName = maps.begin();
    for (int i = 0; i < maps.size(); i++) {
        output << "| " << maps.at(i) << setfill(filler1) << setw(width - maps.at(i).length() + 1);
        for (int j = i * noOfGames; j < (i + 1) * noOfGames; j++)
            if (j != (i + 1) * noOfGames - 1)
                output << "| " << winners.at(j).second << setfill(filler1)
                       << setw(width - winners.at(j).second.length() + 1);
            else
                output << "| " << winners.at(j).second << setfill(filler1)
                       << setw(width - winners.at(j).second.length()) << "|" << endl;
        for (int j = 0; j <= noOfGames; j++)
            if (j != noOfGames)
                output << "|" << setfill(filler2) << setw(width) << "-";
            else
                output << "|" << setfill(filler2) << setw(width) << "-" << "|" << endl;
    }

    output << endl << endl;

    // tournament settings
    cout << "Tournament mode: " << endl;
    cout << "M: ";
    for (const string &map: maps)
        cout << map << ", ";
    cout << endl;
    cout << "P: ";
    for (const string &player: playerStrategies)
        cout << player << ", ";
    cout << endl;
    cout << "G: " << noOfGames << endl;
    cout << "D: " << maxNoOfTurns << endl << endl << "|";

    init.copyfmt(cout);

    // header row of the table
    for (int i = 0; i < noOfColumns; i++)
        cout << left << setw(width) << setfill(filler2) << "" << "|";
    cout << endl << "|";

    for (int i = 0; i < noOfColumns; i++) {
        if (i == 0) {
            cout << left << setw(width) << setfill(filler1) << "" << "|";
        } else
            cout << left << setw(width) << setfill(filler1) << (indent + "Game " + to_string(i)) << "|";
    }
    cout << endl << "|";
    for (int i = 0; i < noOfColumns; i++)
        cout << left << setw(width) << setfill(filler2) << ("") << "|";
    cout << endl;

    cout.copyfmt(init);

    // consequent rows of the table
    for (int i = 0; i < maps.size(); i++) {
        cout << "| " << maps.at(i) << setfill(filler1) << setw(width - maps.at(i).length() + 1);
        for (int j = i * noOfGames; j < (i + 1) * noOfGames; j++)
            if (j != (i + 1) * noOfGames - 1)
                cout << "| " << winners.at(j).second << setfill(filler1)
                     << setw(width - winners.at(j).second.length() + 1);
            else
                cout << "| " << winners.at(j).second << setfill(filler1)
                     << setw(width - winners.at(j).second.length()) << "|" << endl;
        for (int j = 0; j <= noOfGames; j++)
            if (j != noOfGames)
                cout << "|" << setfill(filler2) << setw(width) << "-";
            else
                cout << "|" << setfill(filler2) << setw(width) << "-" << "|" << endl;
    }

    cout << endl << endl;
}

//GameEngine class destructor
GameEngine::~GameEngine() {
    delete mapLoader;
    mapLoader = nullptr;
    delete processor;
    delete deck;
    for (auto *p: playersList) {
        delete p;
    }
    for (auto *p: eliminatedPlayers) {
        delete p;
    }
    playersList.clear();
}
