#include "LoggingObserver.h"
#include "../GameEngine/GameEngine.h"

void LogObserver::LoggingDriver() {
    LogObserver *logObserver = new LogObserver();
    GameEngine* engine = new GameEngine();
    cout << "States logging:" << endl;
    engine->attach(logObserver);
    engine->startupPhase();
    cout << endl;

    cout << "Orders logging:" << endl;
    Orders::OrdersList *ordersList = new Orders::OrdersList();
    Players::Player *player = new Players::Player("player");
    player->reinforcementPool = 10;
    string name("Territory");
    Graph::Territory *territory = new Graph::Territory(1, 1, name);
    territory->transferOwnership(player);
    Orders::Deploy *deploy = new Orders::Deploy(player, territory, 10);
    ordersList->add(deploy);
    Orders::Blockade *blockade = new Orders::Blockade(player, territory);
    ordersList->add(blockade);
    ordersList->element(0)->execute();
    ordersList->element(1)->execute();
    delete ordersList;
    delete player;
    delete territory;
    cout << endl;

    cout << "Commands logging:" << endl;
    CommandProcessor::commandProcessorDriver();

    delete engine;
    delete logObserver;
}