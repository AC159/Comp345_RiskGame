
#ifndef COMP345RISKGAME_GAMEENGINE_H
#define COMP345RISKGAME_GAMEENGINE_H

#include "../Map/Map.h"
#include "../Orders/Orders.h"
#include "../Player/Player.h"
#include <istream>
#include <string>

using namespace std;

class CommandProcessor;
class Command;

class GameEngine {
public:
    CommandProcessor* processor;
    Graph::MapLoader* mapLoader;
    Cards::Deck* deck;
    vector<Players::Player*> playersList;
    GameEngine();
    GameEngine(const GameEngine &gameEngine); //copy constructor
    ~GameEngine();

    GameEngine& operator=(const GameEngine &gameEngine);
    friend ostream& operator<<(ostream &out, const GameEngine &gameEngine);
    static void gameStartupDriver();
    static void gamePlayDriver();

    //added for implementation of As2 part1
    string getState() const;

//============= start state ================
    void startupPhase();
    void gameStart();

//============= map loaded state =================
    void mapLoadedStateChange();
    void chooseMapToLoad() const;

//============= map validated state =================
    void mapValidatedStateChange();

//============= players added state =================
    void playersAddedStateChange();
    void addPlayer();

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
    string state;

    void changeState(string state); //state to change current stated and output current state
    void createAndAddOrder(int commandNumber); //issue orders state
    void executeOrders(); //execute orders state
};


#endif //COMP345RISKGAME_GAMEENGINE_H
