#include "GameEngine.h"

using namespace std;

int main(int argc, char *argv[]) {

    GameEngine *gameEngine = new GameEngine();
    start:
    gameEngine->welcomeMessage();
    if (gameEngine->validateStartStateCommand()) {
        gameEngine->mapLoadedStateChange();
        gameEngine->chooseMapToLoad();
        gameEngine->validateMapLoadedCommand();
        gameEngine->mapValidatedStateChange();
        gameEngine->validateMapValidatedCommand();
        gameEngine->playersAddedStateChange();
        while (gameEngine->validatePlayersAddedCommand()){}};
        //---> assign countries here: to be implemented next phase
        assignreinforcement:
        gameEngine->assignReinforcementStateChange();
        gameEngine->validateAssignReinforcementCommand();
        gameEngine->issueOrdersStateChange();
        gameEngine->validateIssueOrdersCommand();
        gameEngine->executeOrdersStateChange();
        if (gameEngine->validateExecuteOrdersCommand() == 1)
            goto assignreinforcement;
        gameEngine->winStateChange();
        if (gameEngine->validateWinCommand() == 1)
            goto start;
 //   }

   // delete gameEngine;

    return 0;
}
