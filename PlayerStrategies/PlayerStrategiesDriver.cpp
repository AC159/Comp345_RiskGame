#include "PlayerStrategies.h"
#include "../GameEngine/GameEngine.h"

void PlayerStrategiesDriver() {

    // Create several players with different strategies
    auto *benevolentPlayer = new Players::Player("p1");
    auto *benevolentStrategy = new BenevolentPlayerStrategy(benevolentPlayer);

    // Assign strategy to player
    benevolentPlayer->ps = benevolentStrategy;

    auto *engine = new GameEngine();
    auto *logObserver = new LogObserver();
    Subject::attach(logObserver);
    engine->startupPhase();

    engine->mainGameLoop();
    engine->winStateChange();

    delete benevolentPlayer;
    delete engine;
}
