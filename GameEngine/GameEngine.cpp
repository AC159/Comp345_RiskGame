#include "GameEngine.h"
#include <filesystem>
#include <utility>
#include <experimental/random>


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
    cout << "========== state = " << this->state << " ==========" << endl;
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
        cout << "Use the 'gamestart' command to distribute territories, determine play order, assign initial armies and draw cards from the deck"
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
            auto *p = new Players::Player(command.command.substr(command.command.find_last_of(' ') + 1, command.command.length()));
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
    while(!playerWon) {
        turnCount++;
        if (turnCount == 100) {
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
    assignReinforcementStateChange();

    for (auto player: playersList) {
        int baseRate = static_cast<int>(player->territories.size() / 3);
        int totalBonus = 0;
        for (auto continent : mapLoader->map->continents) {
            //find if player owns any whole continent and honor bonus reinforcements accordingly
            int bonus = continent->bonusValue;
            for (auto continentTerritory: continent->territories) {
                if (!player->territories.contains(continentTerritory->countryNumber)) {
                    bonus = 0;
                    break;
                }
            }
            totalBonus += bonus;
        }

        //the minimum reinforcements a player receives is 3
        player->reinforcementPool = baseRate + totalBonus > 3 ? baseRate + totalBonus : 3;
    }
}

//=============issue orders state =================
//method to modify current game state to 'issue orders'
void GameEngine::issueOrdersStateChange() {
    GameEngine::changeState("issue orders");
}

void GameEngine::issueOrdersPhase() {
    issueOrdersStateChange();

    for (auto player: playersList) {
        player->issueOrder(mapLoader->map->edges);
    }
}

//=============execute orders state =================
//method to modify current game state to 'execute orders'
void GameEngine::executeOrdersStateChange() {
    GameEngine::changeState("execute orders");
}

//returns true if a player won during the turn
bool GameEngine::executeOrdersPhase() {
    executeOrdersStateChange();

    int skipStrike = 0; //the number of non-deploy orders skipped in a row
    int numOfPlayers = static_cast<int>(playersList.size());
    bool doneDeploying = false;

    while (ordersRemain()) {
        for (auto it = playersList.begin(); it < playersList.end(); it++) {
            Players::Player *player = *it;
            Orders::Order *topOrder = player->orders->element(0);

            //deployment is confirmed to be completed once every player's top order was found to not be of deploy type
            if (skipStrike == numOfPlayers) {
                doneDeploying = true;
            }

            if (player->orders->length() != 0 && (doneDeploying || topOrder->type == "deploy")) {
                skipStrike = 0;

                //save the targeted territory's owner to check if they should be eliminated afterwards
                auto targetPlayerIt = playersList.end();
                if (topOrder->type == "advance") {
                    Orders::Advance &advanceOrder = (dynamic_cast<Orders::Advance &>(*topOrder));
                    if (advanceOrder.target != nullptr) { // avoid runtime error
                        targetPlayerIt = std::find(playersList.begin(), playersList.end(),
                                                   advanceOrder.target->owner);
                    }
                }

                cout << player->getName() << " executed: ";
                topOrder->execute();

                //player won if they own all territories
                if (player->territories.size() == mapLoader->map->territories.size()) {
                    cout << player->getName() << " won!" << endl;
                    return true;
                }

                //player is eliminated if their last territory was taken
                if (targetPlayerIt != playersList.end() && (*targetPlayerIt)->territories.empty()
                    && targetPlayerIt != it) {
                    cout << " → " << (*targetPlayerIt)->getName() << " is eliminated." << endl;
                    delete *targetPlayerIt;
                    playersList.erase(targetPlayerIt);
                }

                player->orders->remove(0);
            } else {
                skipStrike++;
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

//method to check user input in the win state and perform related logic
int GameEngine::validateWinCommand() {
    cout << "Command list:\n1. play\n2. end" << endl;
    cout << "Please enter command number:";
    string userInput;
    cin >> userInput;
    while (userInput != "1" && userInput != "2") {
        cout << "Invalid selection. Please enter command number:";
        cin >> userInput;
    }
    //deleting objects in heap
    if (userInput == "1") {
        delete mapLoader->map;
        mapLoader->map = new Graph::Map;

        for (auto &i: playersList) {
            delete i;
        }
        playersList.clear();
    }

    return stoi(userInput);
}

//GameEngine class destructor
GameEngine::~GameEngine() {
    delete mapLoader;
    mapLoader = nullptr;
    delete processor;
    delete deck;
    for (Players::Player *p: playersList) {
        delete p;
    }
    playersList.clear();
}
