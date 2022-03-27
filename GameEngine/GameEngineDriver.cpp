#include "GameEngine.h"

void GameEngine::gameStartupDriver() {
    auto *gameEngine = new GameEngine();
    gameEngine->startupPhase();
    delete gameEngine;
}

void GameEngine::gamePlayDriver() {
    GameEngine game;
    game.startupPhase();
    game.mainGameLoop();
}