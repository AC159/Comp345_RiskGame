
#ifndef COMP345RISKGAME_GAMEENGINE_H
#define COMP345RISKGAME_GAMEENGINE_H

#include "../Map/Map.h"
#include "../Orders/Orders.h"
#include "../Player/Player.h"
#include "../CommandProcessor/CommandProcessing.h"
#include <istream>
#include <string>

using namespace std;

class GameEngine : public ILoggable, public Subject {
public:
    CommandProcessor* processor;
    Graph::MapLoader* mapLoader;
    Cards::Deck* deck;
    vector<Players::Player*> playersList;
    GameEngine();
    GameEngine(const GameEngine &gameEngine); //copy constructor
    ~GameEngine();

    string stringToLog() const override;
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
    void reinforcementPhase();

//============= issue orders state =================
    void issueOrdersStateChange();
    void issueOrdersPhase();

//============= execute orders state =================
    void executeOrdersStateChange();
    bool executeOrdersPhase();
    bool ordersRemain();

//============= win state =================
    void winStateChange();
    int validateWinCommand();

private:
    string state;

    void changeState(string state); //state to change current stated and output current state
};


#endif //COMP345RISKGAME_GAMEENGINE_H