
#ifndef COMP345RISKGAME_GAMEENGINE_H
#define COMP345RISKGAME_GAMEENGINE_H

#include "../Map/Map.h"
#include "../Orders/Orders.h"
#include "../Player/Player.h"
#include <istream>
#include <string>

using namespace std;

class GameEngine {

public:
    Graph::MapLoader* mapLoader;
    int playerAmount;
    vector<Players::Player*> playersList;
    GameEngine();
    GameEngine(const GameEngine &gameEngine); //copy constructor
    ~GameEngine();

    GameEngine& operator=(const GameEngine &gameEngine);
    friend ostream& operator<<(ostream &out, const GameEngine &gameEngine);

//============= start state ================
    void welcomeMessage();

    //user command loadMap
    bool validateStartStateCommand();


//============= map loaded state =================
    void mapLoadedStateChange();

    void chooseMapToLoad();

    void validateMapLoadedCommand();

//============= map validated state =================
    void mapValidatedStateChange();

    bool validateMapValidatedCommand();

//============= players added state =================
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
