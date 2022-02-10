
#ifndef COMP345RISKGAME_GAMEENGINE_H
#define COMP345RISKGAME_GAMEENGINE_H
#include "../Map/Map.h"
#include <istream>

using namespace std;

class GameEngine {
//    static string state;

//Start state
public:
    class Start{
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

    class MapLoaded{

        //MapLoaded(const MapLoaded)

    public:
        //MapLoaded();
        int userNumInput;
        void chooseMapToLoad();
        int validateCommand();
        MapLoader ml{};
    };

class MapValidated{
public:

    bool validateCommand();
};


    class PlayersAdded{
    public:
        int playerAmount;
        int chooseAmountOfPlayersToAdd();

    };

};








#endif //COMP345RISKGAME_GAMEENGINE_H
