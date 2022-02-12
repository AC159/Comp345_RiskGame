
#ifndef COMP345RISKGAME_GAMEENGINE_H
#define COMP345RISKGAME_GAMEENGINE_H

#include "../Map/Map.h"
#include "../Orders/Orders.h"
#include <istream>
#include <string>

using namespace std;

class GameEngine {


public:
    //Start state
    class Start {
    public:
        Start();
        string userInput;

        void welcomeMessage();

        //user command loadMap
        bool validateCommand();

    private:
        //Start *currentState;

    };

    //map loaded state
    class MapLoaded {

    public:
        int userNumInput;

        void chooseMapToLoad();

        int validateCommand();

        MapLoader ml{};
    };

    //map validated state
    class MapValidated {
    public:

        bool validateCommand();
    };

    //players added state
    class PlayersAdded {
    public:
        int playerAmount=1;
        void addPlayer();
        bool validateCommand();

    };

    //assign reinforcement state
    class AssignReinforcement {
    public:
        void assignReinforcement();

        void validateCommand();
    };

    //issue orders state
    class IssueOrders {
    public:
        void issueOrdersStateChange();

        void validateCommand();

    private:
        void createAndAddOrder(int commandNumber);
    };

    //execute orders state
    class ExecuteOrders {
    public:
        void executeOrdersStateChange();

        int validateCommand();

    private:
        void executeOrders();
    };

    //win state
    class Win {
    public:
        void winStateChange();
    };

};


#endif //COMP345RISKGAME_GAMEENGINE_H
