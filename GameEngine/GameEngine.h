
#ifndef COMP345RISKGAME_GAMEENGINE_H
#define COMP345RISKGAME_GAMEENGINE_H

#include "../Map/Map.h"
#include "../Orders/Orders.h"
#include <istream>
#include <string>

using namespace std;

class GameEngine {


public:
//============= start state ================
    void welcomeMessage();

    //user command loadMap
    bool validateStartStateCommand();


//============= map loaded state =================
    void mapLoadedStateChange();

    void chooseMapToLoad();

    void validateMapLoadedCommand();

    Graph::MapLoader mapLoader{};


//============= map validated state =================
    void mapValidatedStateChange();

    bool validateMapValidatedCommand();

//============= players added state =================
    int playerAmount = 1;

    void playersAddedStateChange();

    void addPlayer();

    bool validatePlayersAddedCommand();

//============= assign reinforcement state =================
    void assignReinforcementStateChange();

    void validateAssignReinforcementCommand();

//============= issue orders state =================
    void issueOrdersStateChange();

    void validateIssueOrdersCommand();

//============= execute orders state =================
    void executeOrdersStateChange();

    int validateExecuteOrdersCommand();

//============= win state =================
    void winStateChange();

    int validateWinCommand();

private:
    string state; //state variable

    void changeState(string state); //state to change current stated and output current state
    void createAndAddOrder(int commandNumber); //issue orders state
    void executeOrders(); //execute orders state
};


#endif //COMP345RISKGAME_GAMEENGINE_H
