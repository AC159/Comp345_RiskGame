
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

void GameEngine::PlayersAdded::addPlayer() {
    cout<<"Current amount of players: "<<playerAmount<<endl;
        cout << "Adding 1 more player" << endl;
            playerAmount += 1;
}

bool GameEngine::PlayersAdded::validateCommand() {
    cout<<"Currently, "<<playerAmount<<" players added to the game."<<endl;
    cout<<"Command list:\n1. addplayer\n2. confirmplayers\n3. assigncountries"<<endl;
    cout<<"Please enter command number: ";
    string userInput;
    cin >> userInput;
    while(userInput != "1" && userInput != "2" && userInput !="3") {
        cout<< "Invalid selection. Please enter command number: ";
        cin >> userInput;
    }
    if(userInput == "1"){
        addPlayer();
    }
    else if((userInput =="2" || userInput=="3") && playerAmount<=1){
        if(playerAmount<=1){
            cout<<"The minimum amount of players to play the game is 2. Add another player."<<endl;
            return true;
        }
    }
    else if (userInput =="3"){
        cout<<playerAmount<<" players will be created."<<endl;
        cout<<"assigning territories to the players."<<endl;
        return false ;
    }
    else if(userInput =="2"){
        cout<<"Enter the names of the players: ";
        for(int i=0;i<playerAmount;i++){
            string name;
            cin >> name;
        }

    }
    return true ;
}

//=============assign reinforcement state =================


