#include "GameEngine.h"

using namespace std;
using namespace Graph;
using namespace Players;
//prints territory's neighbors
void showAdj(const vector<Edge *> &edges, Territory *territory) {
    for (auto e: edges) {
        if (e->source == territory) {
            cout << "\t-" << e->destination->name << "" << endl;
        }
    }
}

//assigns territory to given owner with given num of armies & displays territory info
void assignT(const vector<Edge *> &edges, Territory *territory, Player *owner, int armyCount) {
    cout << "• Giving territory " << territory->name << " to " << owner->getName() << endl;
    cout << " > " << territory->name << " has " << armyCount << " armies and its neighbors are:" << endl;
    showAdj(edges, territory);
    territory->numberOfArmies = armyCount;
    territory->transferOwnership(owner);
}

void GameEngine::gameStartupDriver() {
    auto* gameEngine = new GameEngine();
    gameEngine->startupPhase();
    delete gameEngine;
}

void GameEngine::gamePlayDriver() {
    auto *ml = new MapLoader;
    ml->loadMap("../WarzoneMaps/solar/solar.map");
    cout << "*******************************************************************************************" << endl;

    cout << "\n\nCreating players named PlayerOne and PlayerTwo..." << endl;
    auto *p1 = new Player("Player1");
    auto *p2 = new Player("Player2");

    cout << "\n\nAssigning territories..." << endl;
    Territory *southNeptune = ml->map->territories[0];
    Territory *eastNeptune = ml->map->territories[1];
    Territory *westNeptune = ml->map->territories[2];
    Territory *northNeptune = ml->map->territories[3];
    Territory *eastPluto = ml->map->territories[4];
    Territory *westPluto = ml->map->territories[5];
    assignT(ml->map->edges, eastNeptune, p1, 2);
    assignT(ml->map->edges, southNeptune, p1, 2);
    assignT(ml->map->edges, westNeptune, p2, 3);
    assignT(ml->map->edges, northNeptune, p2, 2);
    assignT(ml->map->edges, eastPluto, p2, 3);
    assignT(ml->map->edges, westPluto, p2, 2);

    GameEngine game;
    game.mapLoader = ml;
    game.playersList.push_back(p1);
    game.playersList.push_back(p2);
    game.reinforcementPhase();
    p1->reinforcementPool = 50;
    p2->reinforcementPool = 30;
    cout << "\n\nPlayerOne reinforcement pool: " << p1->reinforcementPool << endl;
    cout << "PlayerTwo reinforcement pool: " << p2->reinforcementPool << "\n\n" << endl;

    cout << p1->getName() << endl;
    for (const auto &i : p1->toDefend(ml->map->edges)) {
        cout << "DEFEND " << i.first << " " <<i.second->name << endl;
    }
    cout << endl;
    cout << p2->getName() << endl;
    for (const auto &i : p2->toDefend(ml->map->edges)) {
        cout << "DEFEND " << i.first << " " <<i.second->name << endl;
    }
    cout << endl;

    game.issueOrdersPhase();
    auto *p3 = new Player("Player3");
    game.playersList.push_back(p3);
    p1->orders->add(new Orders::Deploy());
    p1->orders->add(new Orders::Deploy());
    p1->orders->add(new Orders::Advance());
    p1->orders->add(new Orders::Advance());
    p1->orders->add(new Orders::Advance());
    p2->orders->add(new Orders::Deploy());
    p2->orders->add(new Orders::Advance());
    p2->orders->add(new Orders::Advance());
    p2->orders->add(new Orders::Advance());
    p2->orders->add(new Orders::Advance());
    p3->orders->add(new Orders::Deploy());
    p3->orders->add(new Orders::Deploy());
    p3->orders->add(new Orders::Deploy());
    p3->orders->add(new Orders::Advance());
    p3->orders->add(new Orders::Advance());
    game.executeOrdersPhase();
}