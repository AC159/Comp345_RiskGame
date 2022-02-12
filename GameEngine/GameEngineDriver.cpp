#include "GameEngine.h"
using namespace std;

int main(int argc, char *argv[]) {

//   GameEngine::Start start{};
//   start.welcomeMessage();
//   if(start.validateCommand()) {
//       GameEngine::MapLoaded map1{};
//       map1.chooseMapToLoad();
//       map1.validateCommand();
//       GameEngine::MapValidated mapValidated;
//       mapValidated.validateCommand();
       GameEngine::PlayersAdded playersAdded;
       bool playerAddedValidateCommand = true;
       while (playerAddedValidateCommand) {
           playerAddedValidateCommand =  playersAdded.validateCommand();

       }
       // assign reinforcment state
       //issue orders
       //execute order
       //win
//   }


    return 0;
}
