
#include "GameEngine.h"
#include "GameEngine.h"



//=============Start state ================
GameEngine::Start::Start()=default;

void GameEngine::Start::welcomeMessage() {
    cout<<"========== state = start =========="<<endl;
    cout<<"Welcome to c++ Risk game!"<<endl;
}

bool GameEngine::Start::validateCommand(){
    cout<<"Command list:\n1. loadmap"<<endl;
    cout<<"Please enter command number: ";
    cin >> userInput;
    while(userInput != "1"){
        cout<< "Invalid selection. Please enter command number: ";
        cin >> userInput;}
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

    int GameEngine::MapLoaded::validateCommand(){
        cout<<"Command list:\n1. validatemap"<<endl;
        cout<<"Please enter command number: ";
        string userInput;
        cin >> userInput;
        while(userInput != "1"){
            cout<< "Invalid selection. Please enter command number: ";
            cin >> userInput;
        }
        return stoi(userInput);
    }


//=============Map validated state =================
// checks that the map is a connected graph, if it isnt, reload a map file
bool GameEngine::MapValidated::validateCommand() {
    cout<<"Command list:\n1. addplayer"<<endl;
    cout<<"Please enter command number: ";
    string userInput;
    cin >> userInput;
    while(userInput != "1"){
        cout<< "Invalid selection. Please enter command number: ";
        cin >> userInput;
    }
    return true;
};

//=============players added state =================

int GameEngine::PlayersAdded::chooseAmountOfPlayersToAdd() {
    bool notenoughPlayers= false;
    while(!notenoughPlayers) {
        cout << "Select the amount of players for the game:" << endl;
        cin >> playerAmount;
        if (playerAmount > 1)
            notenoughPlayers=true;
    }
        return playerAmount;
}