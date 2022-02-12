
#include "GameEngine.h"

Orders::OrdersList *ordersList = new Orders::OrdersList();

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

void GameEngine::chooseMapToLoad() {
    bool validateFile = false;
    bool mapIsValid = false;
    while (!validateFile || !mapIsValid) {
        //TODO implement a function that dynamically extracts the names of all maps
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
            validateFile = mapLoader.loadMap("../WarzoneMaps/bigeurope/bigeurope.map");
        } else if (userNumInput == 2) {
            validateFile = mapLoader.loadMap("../WarzoneMaps/solar/invalidsmallsolar.map");
        } else if (userNumInput == 3) {
            validateFile = mapLoader.loadMap("../WarzoneMaps/solar/smallsolar.map");
        } else if (userNumInput == 4) {
            validateFile = mapLoader.loadMap("../WarzoneMaps/solar/smallsolarduplicates.map");
        }
        if (validateFile) {
            if (mapLoader.map->validate()) {
                cout << "The map is a connected graph and can be played!" << endl;
                mapIsValid = true;
            } else {
                delete mapLoader.map;
                mapLoader.map = new Graph::Map();
            }
        }
    }
    cout << "The file has been loaded and validates! Moving to the next step" << endl;
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

// checks that the map is a connected graph, if it isnt, reload a map file
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

bool GameEngine::validatePlayersAddedCommand() {
    cout << "Currently, " << playerAmount << " players added to the game." << endl;
    cout << "Command list:\n1. addplayer\n2. confirmplayers\n3. assigncountries" << endl;
    cout << "Please enter command number: ";
    string userInput;
    cin >> userInput;
    while (userInput != "1" && userInput != "2" && userInput != "3") {
        cout << "Invalid selection. Please enter command number: ";
        cin >> userInput;
    }
    if (userInput == "1") {
        addPlayer();
    } else if ((userInput == "2" || userInput == "3") && playerAmount <= 1) {
        if (playerAmount <= 1) {
            cout << "The minimum amount of players to play the game is 2. Add another player." << endl;
            return true;
        }
    } else if (userInput == "3") {
        cout << playerAmount << " players will be created." << endl;
        cout << "assigning territories to the players." << endl;
        return false;
    } else if (userInput == "2") {
        cout << "Enter the names of the players: ";
        for (int i = 0; i < playerAmount; i++) {
            string name;
            cin >> name;
        }

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

    if (userInput == "1") {
        delete mapLoader.map;
        mapLoader.map = new Graph::Map;
        //---> empty player list here
    } else if (userInput == "2") {
        delete ordersList;
        delete mapLoader.map;
    }

    return stoi(userInput);
}

