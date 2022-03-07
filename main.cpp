#include <iostream>
#include "Orders/Orders.h"
#include "Map/Map.h"
#include "Player/Player.h"
#include "GameEngine/GameEngine.h"
#include "CommandProcessor/CommandProcessing.h"

using namespace std;
using namespace Graph;
using namespace Players;

int main() {

    string userInput;

    do {
        cout << "Command list:\n"
                "1. Part 1: Command processor and command adapter\n"
                "2. Part 2: Game startup phase\n"
                "3. Part 3: Game play - main game loop\n"
                "4. Part 4: Order execution implementation\n"
                "5. Part 5: Game log observer - commands and orders\n"
                "6. Exit\n"
                "Select the part you would like to run: ";
        cin >> userInput;
        cin.ignore();
        cout << endl;

        if (userInput == "1") {
            cout << "Part 1: Command processor and command adapter" << endl;
            CommandProcessor::commandProcessorDriver();
        } else if (userInput == "2") {
            cout << "Part 2: Game startup phase" << endl;
            GameEngine::gameStartupDriver();
        } else if (userInput == "3") {
            cout << "Part 3: Game play - main game loop" << endl;
            GameEngine::gamePlayDriver();
        } else if (userInput == "4") {
            cout << "Part 4: Order execution implementation" << endl;
            Orders::ordersDriver();
        } else if (userInput == "5") {
            cout << "Part 5: Game log observer - commands and orders" << endl;
            //call driver for Game log observer: commands and orders here
        } else if (userInput != "6")
            cout << "ERROR: Invalid input!" << endl;
    } while (userInput != "6");

    return 0;
}