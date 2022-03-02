#include "GameEngine.h"
#include <filesystem>
#include <utility>
#include <experimental/random>

// todo: list of orders should be specific to a single player
Orders::OrdersList *ordersList = new Orders::OrdersList();

string GameEngine::getState() const {
    return this->state;
}

// GameEngine class constructor
GameEngine::GameEngine() {
    mapLoader = new Graph::MapLoader();
    processor = new CommandProcessor();
    deck = new Cards::Deck();
    deck->fillDeckWithCards();
}

// GameEngine class copy constructor
GameEngine::GameEngine(const GameEngine &game) {
    this->mapLoader = game.mapLoader;
    this->processor = new CommandProcessor(); // generate a new command processor
    this->playersList = game.playersList;
    this->deck = new Cards::Deck(*game.deck);
}

// GameEngine assignment operator
GameEngine &GameEngine::operator=(const GameEngine &gameEngine) {
    if (this == &gameEngine) return *this;
    delete this->mapLoader;
    delete this->deck;
    for (auto *player : this->playersList) {
        delete player;
    }
    delete this->processor;
    this->mapLoader = gameEngine.mapLoader;
    this->processor = new CommandProcessor();
    this->playersList = gameEngine.playersList;
    this->deck = new Cards::Deck(*gameEngine.deck);
    return *this;
}

// GameEngine stream operator
ostream &operator<<(ostream &out, const GameEngine &gameEngine) {
    out << "Nbr of players: " << gameEngine.playersList.size() << endl;
    return out;
}

// method to set and output current game state
void GameEngine::changeState(string changedState) {
    this->state = std::move(changedState);
    cout << "========== state = " << this->state << " ==========" << endl;
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
        cout << "Use the 'gamestart' command to distribute territories, determine play order, assign initial armies and draw cards from the deck" << endl;
        string command = processor->getCommand();
        if (!processor->validate(command, *this)) {
            cout << "Invalid command! Please try again." << endl;
            //save Effect
            string effect="Invalid command! Please try again.";
            commands->saveEffect(effect,*processor);
            continue;
        } else break;
    }

    //save Effect
    string effect="Assign Reinforcement";
    commands->saveEffect(effect,*processor);
    cout << "Assigning territories to players..." << endl;
    // copy all territory pointers into another vector in order to randomly distribute territories to players
    vector<Graph::Territory*> territories = this->mapLoader->map->territories;
    int nbrOfTerritories = territories.size();
    while (territories.size() > 0) {
        for (auto *player : this->playersList) {
            if (nbrOfTerritories == 0) break;
            int random;
            if (nbrOfTerritories == 1) random = 0;
            else random = std::experimental::randint(0, nbrOfTerritories-1);
            player->addTerritory(*territories.at(random));
            if (random != nbrOfTerritories) territories.erase(territories.begin() + random);
            else territories.erase(territories.end() - 1);
            nbrOfTerritories = territories.size();
        }
    }

    // Randomize the order of play of players
    cout << "Assigning play order..." << endl;
    auto rd = std::random_device {};
    auto rng = std::default_random_engine {rd()};
    std::shuffle(this->playersList.begin(), this->playersList.end(), rng);

    cout << "Assigning 50 initial armies & 2 cards to each player..." << endl;
    for (auto *player : this->playersList) {
        player->armyReinforcementPool = 50;
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
        string mapPaths {"../WarzoneMaps"};
        int count {1};
        for (const auto &dir : filesystem::directory_iterator(mapPaths)) {
            string token = dir.path();
            string mapName = token.substr(token.find_last_of('/') + 1, token.length());
            cout << count << ". " << mapName << endl;
            count++;
        }
        string input;
        string command = processor->getCommand();
        if (!processor->validate(command, *this)) {
            cout << "Invalid command! Please try again." << endl;
            //save Effect
            string effect="Invalid command! Please try again.";
            commands->saveEffect(effect,*processor);
            continue;
        }
        // extract name of the chosen map
        string mapName = command.substr(command.find_last_of(' ')+1, command.length());
        validateFile = this->mapLoader->loadMap("../WarzoneMaps/" + mapName + "/" + mapName + ".map");

        //save Effect
        string effect="Maploaded";
        commands->saveEffect(effect,*processor);

        if (validateFile) {
            do {
                cout << "Use the 'validatemap' command to validate that the chosen map is a connected graph" << endl;
                string validateMapCommand = processor->getCommand();
                bool validCommand = processor->validate(validateMapCommand, *this);
                if (!validCommand) {
                    cout << "Invalid command! Please try again." << endl;
                    //save Effect
                    string effect="Invalid command! Please try again.";
                    commands->saveEffect(effect,*processor);
                    continue;
                } else break;
            } while (true);
            if (mapLoader->map->validate()) {
                cout << "The map is a connected graph and can be played!" << endl;
                mapIsValid = true;
                //save Effect
                string effect="mapValidated";
                commands->saveEffect(effect,*processor);
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

    int count {0};
    while (count < playerCount) {
        cout << "Use the 'addplayer <player name>' command to enter players in the game" << endl;
        string command = processor->getCommand();
        if (!processor->validate(command, *this)) {
            cout << "Invalid command! Please try again." << endl;
            //save Effect
            string effect="Invalid command! Please try again.";
            commands->saveEffect(effect,*processor);
            continue;
        } else {
            Players::Player* p = new Players::Player(command.substr(command.find_last_of(' ')+1, command.length()));
            this->playersList.push_back((p));
            count++;
            //save Effect
            string effect="playersadded";
            commands->saveEffect(effect,*processor);
        }
    }
}

//=============assign reinforcement state =================
//method to modify current game state to 'assign reinforcement'
void GameEngine::assignReinforcementStateChange() {
    GameEngine::changeState("assign reinforcement");
}

//method to check user input in the assign reinforcement state and perform related logic
void GameEngine::validateAssignReinforcementCommand() {
    cout << "Command list:\n1. issueorder" << endl;
    cout << "Please enter command number:";
    string userInput;
    cin >> userInput;
    while (userInput != "1") {
        cout << "Invalid selection. Please enter command number:";
        cin >> userInput;
    }
}

//=============issue orders state =================
//method to modify current game state to 'issue orders'
void GameEngine::issueOrdersStateChange() {
    GameEngine::changeState("issue orders");
}

//method to create orders and add them to an order list
void GameEngine::createAndAddOrder(int commandNumber) {
    switch (commandNumber) {
        case 1:
            cout << "Adding 'deploy' order to order list..." << endl;
            ordersList->add(new Orders::Deploy);
            break;
        case 2:
            cout << "Adding 'advance' order to order list..." << endl;
            ordersList->add(new Orders::Advance);
            break;
        case 3:
            cout << "Adding 'bomb' order to order list..." << endl;
            ordersList->add(new Orders::Bomb);
            break;
        case 4:
            cout << "Adding 'blockade' order to order list..." << endl;
            ordersList->add(new Orders::Blockade);
            break;
        case 5:
            cout << "Adding 'airlift' order to order list..." << endl;
            ordersList->add(new Orders::Airlift);
            break;
        case 6:
            cout << "Adding 'negotiate' order to order list..." << endl;
            ordersList->add(new Orders::Negotiate);
            break;
    }
}

//method to check user input in the issue orders state and perform related logic
void GameEngine::validateIssueOrdersCommand() {
    cout << "Command list:\n1. deploy\n2. advance\n3. bomb\n4. blockade\n5. airlift\n6. negotiate\n7. endissueorders"
         << endl;
    cout << "Please enter command number:";
    int userInput;
    cin >> userInput;

    while (userInput != 7) {
        if (userInput < 1 || userInput > 7) {
            cout << "Invalid selection. Please enter command number:" << endl;
            cin >> userInput;
        } else {
            GameEngine::createAndAddOrder(userInput);
            cout
                    << "Command list:\n1. deploy\n2. advance\n3. bomb\n4. blockade\n5. airlift\n6. negotiate\n7. endissueorders"
                    << endl;
            cout << "Please enter command number:";
            cin >> userInput;
        }
    }
}

//=============execute orders state =================
//method to modify current game state to 'execute orders'
void GameEngine::executeOrdersStateChange() {
    GameEngine::changeState("execute orders");
}

//method to execute orders in the order list
void GameEngine::executeOrders() {
    for (size_t i = 0; i < ordersList->length(); i++) {
        ordersList->element(i)->execute();
        ordersList->remove(i);
        i--;
    }
}

//method to check user input in the execute orders state and perform related logic
int GameEngine::validateExecuteOrdersCommand() {
    cout << "Command list:\n1. execorder\n2. win" << endl;
    cout << "Please enter command number:";
    int userInput;
    cin >> userInput;

    while (userInput != 1 && userInput != 2) {
        cout << "Invalid selection. Please enter command number:" << endl;
        cin >> userInput;
    }

    if (userInput == 1) {
        GameEngine::executeOrders();
        cout << "Command list:\n1. endexecorder\n2. win" << endl;
        cout << "Please enter command number:";
        cin >> userInput;

        while (userInput != 1 && userInput != 2) {
            cout << "Invalid selection. Please enter command number:" << endl;
            cin >> userInput;
        }
    }

    return userInput;
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

        for (auto & i : playersList) {
            delete i;
        }
        playersList.clear();
    }

    return stoi(userInput);
}

//GameEngine class destructor
GameEngine::~GameEngine() {
    delete ordersList;
    delete mapLoader;
    mapLoader = nullptr;
    delete processor;
    delete deck;
    for (Players::Player* p : playersList) {
        delete p;
    }
    playersList.clear();
}

