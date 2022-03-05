#include "GameEngine.h"

using namespace std;

void GameEngine::gameEngineDriver() {
    GameEngine* gameEngine = new GameEngine();
    gameEngine->startupPhase();
    delete gameEngine;
}
