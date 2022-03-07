#include "GameEngine.h"

using namespace std;

void GameEngine::gameStartupDriver() {
    auto* gameEngine = new GameEngine();
    gameEngine->startupPhase();
    delete gameEngine;
}

void GameEngine::gamePlayDriver() {

}