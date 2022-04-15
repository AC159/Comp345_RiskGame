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

string GameEngine::getState() const {
    return this->state;
}

GameEngine::GameEngine() {
    mapLoader = new Graph::MapLoader();
    processor = new CommandProcessor();
    deck = new Cards::Deck();
    deck->fillDeckWithCards();
    state = "NULL"; //TODO: keep an eye out for this added line in case it is interfering with main branch functionality
}

GameEngine::GameEngine(const GameEngine &game) {
    this->mapLoader = game.mapLoader;
    this->processor = new CommandProcessor(); // generate a new command processor
    this->playersList = game.playersList;
    this->deck = new Cards::Deck(*game.deck);
}

string GameEngine::stringToLog() const {
    return "Game engine state: " + this->state;
}

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
// method to show game start welcome message

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

    cout << "Assigning territories to players..." << endl;
    // copy all territory pointers into another vector in order to randomly distribute territories to players
    vector<Graph::Territory *> territories = this->mapLoader->map->territories;
    int nbrOfTerritories = territories.size();
    while (territories.size() > 0) {
        for (auto *player: this->playersList) {
            if (nbrOfTerritories == 0) break;
            int random;
            if (nbrOfTerritories == 1) random = 0;
            else random = std::experimental::randint(0, nbrOfTerritories - 1);
            territories.at(random)->transferOwnership(player);
            if (random != nbrOfTerritories) territories.erase(territories.begin() + random);
            else territories.erase(territories.end() - 1);
            nbrOfTerritories = territories.size();
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
            auto *p = new Players::Player(command.command.substr(command.command.find_last_of(' ') + 1, command.command.length()));
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
void GameEngine::mainGameLoop() {
    bool playerWon = false;
    int turnCount = 0;
    while (!playerWon) {
        turnCount++;
        if (turnCount == 500) {
            cout << "* ERROR:  Too many turns. Main game loop terminated to avoid infinite loop. *\n" << endl;
            break;
        }
        cout << " —▶ TURN " << turnCount << endl;
        reinforcementPhase();
        issueOrdersPhase();
        playerWon = executeOrdersPhase(); //player win and player eliminations are determined in the execution phase
    }
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

    for (const auto &player : playersList) {
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
                    [](Players::Player *p){ return p->orders->length() == 0; })) {
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

void GameEngine::tournamentMode(Command &command) {
    string tournamentCommand = command.command;
    vector<string> maps;
    vector<string> playerStrategies;
    vector<string> winners;
    int noOfGames;
    int maxNoOfTurns;

    // get maps from command
    string lineBetween = tournamentCommand.substr(tournamentCommand.find("-M") + 2,
                                                  tournamentCommand.find("-P") - tournamentCommand.find("-M") - 2);
    istringstream parse2(lineBetween);
    while (parse2 >> lineBetween) {
        maps.push_back(lineBetween);
    }
    int numberOfMaps = maps.size();

    // get player strats from command
    lineBetween = tournamentCommand.substr(tournamentCommand.find("-P") + 2,
                                           tournamentCommand.find("-G") - tournamentCommand.find("-P") - 2);
    istringstream parse3(lineBetween);
    while (parse3 >> lineBetween) {
        playerStrategies.push_back(lineBetween);
    }
    int numberOfStrategies = playerStrategies.size();

    // get no of games from command
    lineBetween = tournamentCommand.substr(tournamentCommand.find("-G") + 2,
                                           tournamentCommand.find("-D") - tournamentCommand.find("-G") - 2);
    noOfGames = std::stoi(lineBetween);

    // get no of turns from command
    lineBetween = tournamentCommand.substr(tournamentCommand.find("-D") + 2);
    maxNoOfTurns = std::stoi(lineBetween);

//    cout << "*" << endl;
//    for(string m: maps)
//        cout << m << endl;
//    for(string ps: playerStrategies)
//        cout << ps << endl;
//    cout << noOfGames << endl;
//    cout << maxNoOfTurns << endl;
//    cout << "*" << endl;

//    for(string m: maps){
//        bool validateFile = this->mapLoader->loadMap("../WarzoneMaps/" + m + "/" + m + ".map");
//        if(validateFile){
//            if(!mapLoader->map->validate())
//                cout << "Invalid map." << endl;
//        }
//        else {
//            cout << "Invalid map file." << endl;
//        }
//        delete mapLoader->map;
//        mapLoader->map = new Graph::Map();
//    }

//    vector<string> maps;
//    vector<string> playerStrategies;
//    vector<string> winners;
//    int noOfGames;
//    int maxNoOfTurns;

    for (auto map: maps)
        cout << map << " ";
    cout << endl;

    for (auto strategy: playerStrategies)
        cout << strategy << " ";
    cout << endl;

    cout << noOfGames << endl;
    cout << maxNoOfTurns << endl;

//    return;

    //TODO: implement max number of turns
    //TODO: implement player strategies in tournament
    //TODO: implement winner player names storage and display

    // play each maps by noOfGames times
    for (string map: maps) {
        for (int i = 0; i < noOfGames; i++) {
            cout<< "<<<<<<<<<<<<<<<" << " Game number: " << i << " >>>>>>>>>>>>>>>" <<endl;

            changeState("start");
            if (!this->mapLoader->loadMap("../WarzoneMaps/" + map + "/" + map + ".map")) {
                cout << map + ".map is an invalid map file." << endl;
                return;
            }

            mapLoadedStateChange();
            if (!mapLoader->map->validate()) {
                cout << map + " is an invalid map." << endl;
                return; //TODO: test that this is appropriate/working properly in this case; should prevent the game from continuing if the map is not validated
            }

            mapValidatedStateChange();

            playersAddedStateChange();
            int playerNumber = 1;
            for (string player: playerStrategies) {
                Players::Player *p = new Players::Player("Player " + to_string(playerNumber++));
                this->playersList.push_back((p));
            }

            //TODO: remove the below code block
            {
                for (auto player: playersList)
                    cout << player->getName() << endl;
                delete mapLoader->map;
                mapLoader->map = new Graph::Map();
                for (Players::Player *p: playersList) {
                    delete p;
                }
                playersList.clear();
                continue;
            }

            assignReinforcementStateChange();
            mainGameLoop();
            winStateChange();

            // name of the winner is put in the winners vector if there's only one player left in the list, otherwise put Draw in the vector
            if (playersList.size() == 1)
                winners.push_back(playersList.at(0)->getName());
            else
                winners.push_back("Draw");

            // reset map and players list
            delete mapLoader->map;
            mapLoader->map = new Graph::Map();
            for (Players::Player *p: playersList) {
                delete p;
            }
            playersList.clear();
        }
    }
    return; //TODO: remove this line once the tournament is running properly; this line is included to avoid flooding gamelog.txt while testing

    fstream output("gamelog.txt", std::ios::out);

    output << "Tournament mode: " << endl;
    output << "M: ";
    for (string map: maps)
        output << map << ", ";
    output << "\b\b" << " " << endl;
    output << "P: ";
    for (string player: playerStrategies)
        output << player << ", ";
    output << "\b\b" << " " << endl;
    output << ("G: " + noOfGames) << endl;
    output << ("D: " + maxNoOfTurns) << endl << endl;

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
    output << "|";
    for (int i = 0; i < noOfColumns; i++) {
        if (i == 0) {
            output << left << setw(width) << setfill(filler1) << "" << "|";
        } else
            output << left << setw(width) << setfill(filler1) << (indent + "Game " + to_string(i)) << "|";
    }
    output << "|";
    for (int i = 0; i < noOfColumns; i++)
        output << left << setw(width) << setfill(filler2) << ("") << "|";

    output.copyfmt(init);

    // consequent rows of the table
    auto winnerName = winners.begin();
    auto mapName = maps.begin();
    for (int i = 0; i < maps.size() * 2; i++) {
        if (i % 2 == 0) {
            for (int column = 0; column < noOfColumns; column++) {
                if (column == 0) {
//                    output << left << setw(width) << setfill(filler1) << (indent + mapName) << "|";
                    mapName++;
                } else {
//                    output << left << setw(width) << setfill(filler1) << (indent + winnerName) << "|";
                    winnerName++;
                }
            }
        } else {
            output << "|";
            for (int column = 0; column < noOfColumns; column++) {
                output << left << setw(width) << setfill(filler2) << ("") << "|";
            }
        }
    }
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
