
#ifndef COMP345RISKGAME_GAMEENGINE_H
#define COMP345RISKGAME_GAMEENGINE_H

#include "../Map/Map.h"
#include "../Orders/Orders.h"
#include "../Player/Player.h"
#include "../CommandProcessor/CommandProcessing.h"
#include <istream>
#include <string>

using namespace std;

// Warzone game controller
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
    static void tournamentModeDriver();

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

//==== main game loop: reinforcement, order issuing & execution phases ====
    bool mainGameLoop(int maxNumberOfTurns = 500);

//============= assign reinforcement state =================
    void assignReinforcementStateChange();
    void reinforcementPhase();

//============= issue orders state =================
    void issueOrdersStateChange();
    void issueOrdersPhase();

//============= execute orders state =================
    void executeOrdersStateChange();
    bool executeOrdersPhase();

//============= win state =================
    void winStateChange();

    void tournamentMode(Command& command);

private:
    vector<Players::Player *> eliminatedPlayers;
    string state;

    void changeState(string state); //state to change current stated and output current state
    bool ordersRemain(); //helper for the order execution phase
};


#endif //COMP345RISKGAME_GAMEENGINE_H