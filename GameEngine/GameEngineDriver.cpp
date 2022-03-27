#include "GameEngine.h"

// demonstrates part 2: the startup phase of the game
void GameEngine::gameStartupDriver() {
    auto *gameEngine = new GameEngine();
    gameEngine->startupPhase();
    delete gameEngine;
}

// demonstrates part 3 (reinforcement, issuing orders, and order execution phases) in addition to part 2
void GameEngine::gamePlayDriver() {
    GameEngine game;
    game.startupPhase();
    game.mainGameLoop();
}