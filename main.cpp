#include <iostream>
#include "GameEngine/GameEngine.h"
#include "PlayerStrategies/PlayerStrategies.h"

using namespace std;
using namespace Graph;
using namespace Players;

int main() {

    string userInput;

    do {
        cout << "Command list:\n"
                "1. Part 1: Player Strategies\n"
                "2. Part 2: Tournament Mode\n"
                "3. Exit\n"
                "Select the part you would like to run: ";
        cin >> userInput;
        cin.ignore();
        cout << endl;

        if (userInput == "1") {
            cout << "Part 1: Player Strategies" << endl;
            PlayerStrategiesDriver();
        } else if (userInput == "2") {
            cout << "Part 2: Tournament Mode" << endl;
            GameEngine::gameStartupDriver();
        } else if (userInput != "3")
            cout << "ERROR: Invalid input!" << endl;
    } while (userInput != "3");

    return 0;
}