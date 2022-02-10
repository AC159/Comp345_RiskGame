#include <iostream>
#include "GameEngine.h"
using namespace std;

int main(int argc, char *argv[]) {

   GameEngine::Start start{};
   start.welcomeMessage();
   if(start.validateCommand()){
       GameEngine::MapLoaded map1{};
       map1.chooseMapToLoad();
       map1.validateCommand();
       //validate map method here
       GameEngine::MapValidated mapValidated;
       mapValidated.validateCommand();
       //add players into map
   };


    return 0;
}
