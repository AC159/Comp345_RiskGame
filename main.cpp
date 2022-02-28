#include <iostream>
#include "Orders/Orders.h"
#include "Map/Map.h"
#include "Player/Player.h"
#include "GameEngine/GameEngine.h"

using namespace std;
using namespace Graph;
using namespace Players;

int main() {

    string userInput;

    do {
        cout << "Command list:\n"
                "1. Part 1: Map Driver\n"
                "2. Part 2: Player Driver\n"
                "3. Part 3: Orders Driver\n"
                "4. Part 4: Cards Driver\n"
                "5. Part 5: GameEngine Driver\n"
                "6. Exit\n"
                "Select the part you would like to run: ";
        cin >> userInput;
        cin.ignore();
        cout << endl;

        if (userInput == "1") {
            cout << "Part 1: Map Driver" << endl;
            Graph::Map::mapDriver();
        } else if (userInput == "2") {
            cout << "Part 2: Player Driver" << endl;
            Player::playerDriver();
        } else if (userInput == "3") {
            cout << "Part 3: Orders Driver" << endl;
            Orders::Order::ordersDriver();
        } else if (userInput == "4") {
            cout << "Part 4: Cards Driver" << endl;
            Cards::Card::cardsDriver();
        } else if (userInput == "5") {
            cout << "Part 5: GameEngine Driver" << endl;
            GameEngine::gameEngineDriver();
        } else if (userInput != "6")
            cout << "ERROR: Invalid input!" << endl;
    } while (userInput != "6");

    return 0;
}
