#include <iostream>
#include "Orders/Orders.h"
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
                "2. Part 2: Tournament mode\n"
                "3. Exit\n"
                "Select the part you would like to run: ";
        cin >> userInput;
        cin.ignore();
        cout << endl;

        if (userInput == "1") {
            cout << "Part 1: Command processor and command adapter" << endl;
            CommandProcessor::commandProcessorDriver();
        } else if (userInput == "2") {
            cout << "Part 2: Tournamend mode" << endl;
            GameEngine::tournamentModeDriver();
        } else if (userInput != "3")
            cout << "ERROR: Invalid input!" << endl;
    } while (userInput != "3");

    delete Players::Player::neutralPlayer;
    return 0;
}