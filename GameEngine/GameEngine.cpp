
#include "GameEngine.h"
#include "GameEngine.h"
vector<string> GameEngine::ordersList = vector<string>();

//=============Start state ================
GameEngine::Start::Start() = default;

void GameEngine::Start::welcomeMessage() {
    cout << "========== state = start ==========" << endl;
    cout << "Welcome to c++ Risk game!" << endl;
}

bool GameEngine::Start::validateCommand() {
    cout << "Command list:\n1. loadmap" << endl;
    cout << "Please enter command number: ";
    cin >> userInput;
    while (userInput != "1") {
        cout << "Invalid selection. Please enter command number: ";
        cin >> userInput;
    }
    return true;
}

//=============Map Loaded state =================
//for demo purposes we have predefined choices to demonstrate what happens for each scenario:
// the map file is valid and its a connected graph
// the map file is not valid
// the map file is valid but its not a connected graph
void GameEngine::MapLoaded::chooseMapToLoad() {
    bool validateFile = false;
    bool mapIsValid = false;
    cout << "========== state = map loaded ==========" << endl;
    while (!validateFile || !mapIsValid) {
        //TODO implement a function that dynamically extracts the names of all maps
        cout << "Choose the map you would like to play.\n"
                "1. Europe\n" //working map
                "2. Solar system\n" // invalid map but proper file format
                "3. Small solar GOOD\n" //working map
                "4. Small Solar system\n" //invalid file map, improper file format
             << endl;
        cout << "Select a number: " << endl;
        cin >> userNumInput;
        while (userNumInput >= 5 || userNumInput <= 0) {
            cout << "Please enter a valid number:";
            cin >> userNumInput;
        }
        if (userNumInput == 1) {
            validateFile = ml.loadMap("../WarzoneMaps/bigeurope/bigeurope.map");
        } else if (userNumInput == 2) {
            validateFile = ml.loadMap("../WarzoneMaps/solar/invalidsmallsolar.map");
        } else if (userNumInput == 3) {
            validateFile = ml.loadMap("../WarzoneMaps/solar/smallsolar.map");
        } else if (userNumInput == 4) {
            validateFile = ml.loadMap("../WarzoneMaps/solar/smallsolarduplicates.map");
        }
        if (validateFile) {
            if (ml.map->validate()) {
                cout << "The map is a connected graph and can be played!" << endl;
                mapIsValid = true;
            } else {
                delete ml.map;
                ml.map = new Map;
            }
        }

    }
    cout << "The file has been loaded and validates! Moving to the next step" << endl;
}

int GameEngine::MapLoaded::validateCommand() {
    cout << "Command list:\n1. validatemap" << endl;
    cout << "Please enter command number: ";
    string userInput;
    cin >> userInput;
    while (userInput != "1") {
        cout << "Invalid selection. Please enter command number: ";
        cin >> userInput;
    }
    return stoi(userInput);
}


//=============Map validated state =================
// checks that the map is a connected graph, if it isnt, reload a map file
bool GameEngine::MapValidated::validateCommand() {
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

int GameEngine::PlayersAdded::chooseAmountOfPlayersToAdd() {
    bool notenoughPlayers = false;
    while (!notenoughPlayers) {
        cout << "Select the amount of players for the game:" << endl;
        cin >> playerAmount;
        if (playerAmount > 1)
            notenoughPlayers = true;
    }
    return playerAmount;
}

//=============assign reinforcement state =================
void GameEngine::AssignReinforcement::assignReinforcement() {
    cout << "========== state = assign reinforcement ==========" << endl;
}

void GameEngine::AssignReinforcement::validateCommand() {
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
void GameEngine::IssueOrders::issueOrdersStateChange() {
    cout << "========== state = issue orders ==========" << endl;
}

void GameEngine::IssueOrders::createAndAddOrder(int commandNumber) {
    //create and add orders in the cases below
    switch (commandNumber) {
        case 1:
            cout << "Adding 'deploy' order to order list..." << endl;
            ordersList.push_back("deploy");
            break;
        case 2:
            cout << "Adding 'advance' order to order list..." << endl;
            ordersList.push_back("advance");
            break;
        case 3:
            cout << "Adding 'bomb' order to order list..." << endl;
            ordersList.push_back("bomb");
            break;
        case 4:
            cout << "Adding 'blockade' order to order list..." << endl;
            ordersList.push_back("blockade");
            break;
        case 5:
            cout << "Adding 'airlift' order to order list..." << endl;
            ordersList.push_back("airlift");
            break;
        case 6:
            cout << "Adding 'negotiate' order to order list..." << endl;
            ordersList.push_back("negotiate");
            break;
    }
//    switch (commandNumber) {
//        case 1:
//            cout << "Deploying forces..." << endl;
//            break;
//        case 2:
//            cout << "Advancing forces..." << endl;
//            break;
//        case 3:
//            cout << "Bombing territory..." << endl;
//            break;
//        case 4:
//            cout << "Adding blockade..." << endl;
//            break;
//        case 5:
//            cout << "Airlifting armies..." << endl;
//            break;
//        case 6:
//            cout << "Negotiating..." << endl;
//            break;
//    }
}

void GameEngine::IssueOrders::validateCommand() {
    cout << "Command list:\n1. deploy\n2. advance\n3. bomb\n4. blockade\n5. airlift\n6. negotiate\n7. endissueorders" << endl;
    cout << "Please enter command number:";
    int userInput;
    cin >> userInput;

    while (userInput != 7) {
        if (userInput < 1 || userInput > 7) {
            cout << "Invalid selection. Please enter command number:" << endl;
            cin >> userInput;
        } else {
            GameEngine::IssueOrders::createAndAddOrder(userInput);
            cout << "Command list:\n1. deploy\n2. advance\n3. bomb\n4. blockade\n5. airlift\n6. negotiate\n7. endissueorders" << endl;
            cout << "Please enter command number:";
            cin >> userInput;
        }
    }
}

//=============execute orders state =================
void GameEngine::ExecuteOrders::executeOrdersStateChange() {
    cout << "========== state = execute orders ==========" << endl;
}

void GameEngine::ExecuteOrders::executeOrders() {
    for(string order:ordersList)
        cout << "Executing \'" << order << "\' order..." << endl;
    ordersList.clear();
}

int GameEngine::ExecuteOrders::validateCommand() {
    cout << "Command list:\n1. execorder\n2. win" << endl;
    cout << "Please enter command number:";
    int userInput;
    cin >> userInput;

    while (userInput != 1 && userInput != 2){
        cout << "Invalid selection. Please enter command number:" << endl;
        cin >> userInput;
    }

    if (userInput == 1){
        GameEngine::ExecuteOrders::executeOrders();
        cout << "Command list:\n1. endexecorder\n2. win" << endl;
        cout << "Please enter command number:";
        cin >> userInput;

        while (userInput != 1 && userInput != 2){
            cout << "Invalid selection. Please enter command number:" << endl;
            cin >> userInput;
        }
    }

    return userInput;
}

//=============win state =================
void GameEngine::Win::winStateChange() {
    cout << "========== state = win ==========" << endl;
    cout << "Congratulations! You are the winner of this game!" << endl;
}