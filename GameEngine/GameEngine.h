
#ifndef COMP345RISKGAME_GAMEENGINE_H
#define COMP345RISKGAME_GAMEENGINE_H
#include "../Map/Map.h"
#include <istream>
#include <string>

using namespace std;

class GameEngine {

//Start state
public:
    class Start{
    public:
        Start();
        string userInput;

        void welcomeMessage();
        //user command loadMap
        bool validateCommand();

    private:
        //Start *currentState;

    };
    class MapLoaded{

    public:
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
        int playerAmount=1;
        void addPlayer();
        bool validateCommand();

    };

};








#endif //COMP345RISKGAME_GAMEENGINE_H
