#include "PlayerStrategies.h"

void PlayerStrategiesDriver() {
    auto *engine = new GameEngine();
    engine->startupPhase();
    if (engine->mainGameLoop())
        engine->winStateChange();

    delete engine;
}
