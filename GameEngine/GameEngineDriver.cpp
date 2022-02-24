#include "GameEngine.h"

using namespace std;

    void GameEngine::gameEngineDriver()
    {

        GameEngine *gameEngine = new GameEngine();
        start:
        gameEngine->welcomeMessage();
        gameEngine->startupPhase();
        if (gameEngine->validateStartStateCommand()) {
            gameEngine->mapLoadedStateChange();
            gameEngine->chooseMapToLoad();
            gameEngine->validateMapLoadedCommand();
            gameEngine->mapValidatedStateChange();
            gameEngine->validateMapValidatedCommand();
            gameEngine->playersAddedStateChange();
            while (gameEngine->validatePlayersAddedCommand()) {}
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
        }
        delete gameEngine;
    }

