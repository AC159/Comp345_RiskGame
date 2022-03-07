#include "LoggingObserver.h"
#include "../GameEngine/GameEngine.h"

void LogObserver::LoggingDriver() {
    LogObserver *logObserver = new LogObserver();
    GameEngine* engine = new GameEngine();
    engine->attach(logObserver);
    engine->startupPhase();
    delete engine;
    delete logObserver;
}
