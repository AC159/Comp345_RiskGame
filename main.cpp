#include <iostream>
#include "Orders/Orders.h"
#include "Map/Map.h"
#include "Player/Player.h"
#include "GameEngine/GameEngine.h"

using namespace std;
using namespace Graph;
using namespace Players;

int main() {
    cout << "Part 1: Map Driver" << endl;
    Graph::Map::mapDriver();

    cout << "Part 2: Player Driver" << endl;
    Player::playerDriver();

    cout << "Part 3: Orders Driver" << endl;
    Orders::Order::ordersDriver();

    cout << "Part 4: Cards Driver" << endl;
    Cards::Card::cardsDriver();

    cout << "Part 5: Map Driver" << endl;
    GameEngine::gameEngineDriver();

    return 0;
}
