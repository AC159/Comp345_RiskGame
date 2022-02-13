
#include "GameEngine.h"

Orders::OrdersList *ordersList = new Orders::OrdersList();

GameEngine::GameEngine() {
  mapLoader= new Graph::MapLoader();
  playerAmount=1;
}

GameEngine::GameEngine(const GameEngine &game){
    this->mapLoader = game.mapLoader;
    this->playerAmount = game.playerAmount;
}
GameEngine &GameEngine::operator=(const GameEngine &gameEngine){
    if(this ==&gameEngine) return *this;
    this->playerAmount = gameEngine.playerAmount;
    this->mapLoader = gameEngine.mapLoader;

    return *this;
}

ostream& operator<<(ostream &out, const GameEngine &gameEngine){
    out<<"Player amount: "<< gameEngine.playerAmount<<endl;

    return out;
}

void GameEngine::changeState(string state) {
    this->state = state;
    cout << "========== state = " << state << " ==========" << endl;
}

//=============Start state ================
void GameEngine::welcomeMessage() {
    changeState("start");
    cout << "Welcome to c++ Risk game!" << endl;
}

bool GameEngine::validateStartStateCommand() {
    cout << "Command list:\n1. loadmap" << endl;
    cout << "Please enter command number: ";
    string userInput;
    cin >> userInput;
    while (userInput != "1") {
        cout << "Invalid selection. Please enter command number: ";
        cin >> userInput;
    }
    return true;
}

//=============Map Loaded state =================
void GameEngine::mapLoadedStateChange() {
    GameEngine::changeState("map loaded");
}

// validates the file and checks if the map is a connected graph
void GameEngine::chooseMapToLoad() {
    bool validateFile = false;
    bool mapIsValid = false;
    while (!validateFile || !mapIsValid) {
        cout << "Choose the map you would like to play.\n"
                "1. Europe\n" //working map
                "2. Solar system\n" // invalid map but proper file format
                "3. Small solar GOOD\n" //working map
                "4. Small Solar system\n" //invalid file map, improper file format
             << endl;
        cout << "Select a number: " << endl;
        int userNumInput;
        cin >> userNumInput;
        while (userNumInput >= 5 || userNumInput <= 0) {
            cout << "Please enter a valid number:";
            cin >> userNumInput;
        }
        if (userNumInput == 1) {
            validateFile = mapLoader->loadMap("../WarzoneMaps/bigeurope/bigeurope.map");
        } else if (userNumInput == 2) {
            validateFile = mapLoader->loadMap("../WarzoneMaps/solar/invalidsmallsolar.map");
        } else if (userNumInput == 3) {
            validateFile = mapLoader->loadMap("../WarzoneMaps/solar/smallsolar.map");
        } else if (userNumInput == 4) {
            validateFile = mapLoader->loadMap("../WarzoneMaps/solar/smallsolarduplicates.map");
        }
        if (validateFile) {
            if (mapLoader->map->validate()) {
                cout << "The map is a connected graph and can be played!" << endl;
                mapIsValid = true;
            } else {
                delete mapLoader->map;
                mapLoader->map = new Graph::Map();
            }
        }
    }
    cout << "The file has been loaded and validated! Moving to the next step" << endl;
}

void GameEngine::validateMapLoadedCommand() {
    cout << "Command list:\n1. validatemap" << endl;
    cout << "Please enter command number: ";
    string userInput;
    cin >> userInput;
    while (userInput != "1") {
        cout << "Invalid selection. Please enter command number: ";
        cin >> userInput;
    }
}


//=============Map validated state =================
void GameEngine::mapValidatedStateChange() {
    GameEngine::changeState("map validated");
}


bool GameEngine::validateMapValidatedCommand() {
    cout << "Command list:\n1. addplayer" << endl;
    cout << "Please enter command number: ";
    string userInput;
    cin >> userInput;
    while (userInput != "1") {
        cout << "Invalid selection. Please enter command number: ";
        cin >> userInput;
    }
    return true;
};

//=============players added state =================

void GameEngine::playersAddedStateChange() {
    GameEngine::changeState("players added");
}

void GameEngine::addPlayer() {
    cout << "Current amount of players: " << playerAmount << endl;
    cout << "Adding 1 more player" << endl;
    playerAmount += 1;
}

// creates a list of players that will be assigned a country in the next phase
bool GameEngine::validatePlayersAddedCommand() {
    if(playerAmount ==1){
        cout << "Currently, " << playerAmount << " players added to the game. A minimum of 2 players are required to play the game." << endl;
        cout << "Command list:\n1. addplayer" << endl;
    }
    else if (playerAmount >1){
        cout << "Currently, " << playerAmount << " players added to the game." << endl;
        cout << "Command list:\n1. addplayer\n2. confirm players" << endl;
    }
        cout << "Please enter command number: ";
        string userInput;
        cin >> userInput;
        while (userInput != "1" && playerAmount <= 1 || (userInput!="1" && playerAmount>=2)) {
            if(userInput =="2" && playerAmount>=2)
                break;
            cout << "Invalid selection. Please enter command number: ";
            cin >> userInput;
        }
    if (userInput == "1") {
        addPlayer();
    }
    // creates player objects
    else if (userInput == "2" && playerAmount>1) {
        cout << "Enter the names of the players: ";
            for (int i = 0; i < playerAmount; i++) {
                string playerName;
                cin >> playerName;
                playersList.emplace_back(new Players::Player(playerName));
            }

        cout<< playerAmount<<" players have been created"<<endl;
        for(int i=0;i<playersList.size();i++){
            cout<<playersList.at(i)->getName()<<endl;
        }

        // validates moving to the play section of the state machine
        cout<<"Command list:\n1. assigncountries" << endl;
        cout << "Please enter command number: ";
        cin >> userInput;
        while (userInput != "1") {
            cout << "Invalid selection. Please enter command number: ";
            cin >> userInput;
        }
        return false;
}
    return true;
}

//=============assign reinforcement state =================

void GameEngine::assignReinforcementStateChange() {
    GameEngine::changeState("assign reinforcement");
}

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

void GameEngine::issueOrdersStateChange() {
    GameEngine::changeState("issue orders");
}

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

void GameEngine::executeOrdersStateChange() {
    GameEngine::changeState("execute orders");
}

void GameEngine::executeOrders() {
    for (size_t i = 0; i < ordersList->length(); i++) {
        ordersList->element(i)->execute();
        ordersList->remove(i);
        i--;
    }
}

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

void GameEngine::winStateChange() {
    GameEngine::changeState("win");
    cout << "Congratulations! You are the winner of this game!" << endl;
}

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

        for(int i=0;i<playersList.size();i++){
            delete (playersList.at(i));

        }
        playersList.clear();

    } else if (userInput == "2") {
        delete ordersList;
        delete mapLoader->map;
    }

    return stoi(userInput);
}
GameEngine::~GameEngine() {
    cout<< "game engine map loader invoked"<<endl;
    delete mapLoader;
    mapLoader = nullptr;
    playersList.erase(playersList.begin(),playersList.end());
}

