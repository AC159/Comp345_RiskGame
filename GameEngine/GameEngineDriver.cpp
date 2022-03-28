#include "GameEngine.h"

// demonstrates part 2: the startup phase of the game
void GameEngine::gameStartupDriver() {
    auto *gameEngine = new GameEngine();
    gameEngine->startupPhase();
    delete gameEngine;
}

// demonstrates part 3 (reinforcement, issuing orders, order execution and win phases) in addition to part 2
void GameEngine::gamePlayDriver() {
    string userInput;
    do {
        GameEngine game;
        game.startupPhase();
        game.mainGameLoop();
        game.winStateChange();
        cout << "Use the 'replay' command to play again or the 'quit' command to exit" << endl;
        Command command = game.processor->getCommand();
        userInput = command.command;
        while (!game.processor->validate(userInput, game)) {
            cout << "Invalid command! Please try again." << endl;
            command = game.processor->getCommand();
            userInput = command.command;
        }
    } while (userInput == "replay");
    cout << endl;
}