
#ifndef COMP345RISKGAME_GAMEENGINE_H
#define COMP345RISKGAME_GAMEENGINE_H

#include "../Map/Map.h"
#include "../Orders/Orders.h"
#include <istream>

using namespace std;

class GameEngine {
//    static string state;


public:
    //Start state
    class Start {
    public:
        Start();


        string userInput;

//    Start(const Start &start); //copy constructor
//    Start& operator=(const Start &start); //overloaded assignment operator
//    friend ostream& operator<<(ostream &out, const Start &start);

        void welcomeMessage();

        //user command loadMap
        bool validateCommand();



        // ~Start();
    private:
        //Start *currentState;

    };

    //map loaded state
    class MapLoaded {

        //MapLoaded(const MapLoaded)

    public:
        //MapLoaded();
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
        int playerAmount;

        int chooseAmountOfPlayersToAdd();

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
