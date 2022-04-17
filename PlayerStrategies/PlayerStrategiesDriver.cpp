#include "PlayerStrategies.h"

// demonstrates part 1: the player strategy pattern
void PlayerStrategiesDriver() {
    auto *engine = new GameEngine();
    engine->startupPhase();
    if (engine->mainGameLoop())
        engine->winStateChange();

    delete engine;
}
