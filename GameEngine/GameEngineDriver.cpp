#include "GameEngine.h"

using namespace std;

void GameEngine::gameEngineDriver() {

    GameEngine* gameEngine = new GameEngine();
    gameEngine->startupPhase();

//    start:
//    gameEngine->startupPhase();
//    if (gameEngine->validateStartStateCommand()) {
//        gameEngine->mapLoadedStateChange();
//        gameEngine->chooseMapToLoad();
//        gameEngine->validateMapLoadedCommand();
//        gameEngine->mapValidatedStateChange();
//        gameEngine->validateMapValidatedCommand();
//        gameEngine->playersAddedStateChange();
//        while (gameEngine->validatePlayersAddedCommand()) {}
//        //---> assign countries here: to be implemented next phase
//        assignReinforcement:
//        gameEngine->assignReinforcementStateChange();
//        gameEngine->validateAssignReinforcementCommand();
//        gameEngine->issueOrdersStateChange();
//        gameEngine->validateIssueOrdersCommand();
//        gameEngine->executeOrdersStateChange();
//        if (gameEngine->validateExecuteOrdersCommand() == 1)
//            goto assignReinforcement;
//        gameEngine->winStateChange();
//        if (gameEngine->validateWinCommand() == 1)
//            goto start;
//    }
    delete gameEngine;
}
