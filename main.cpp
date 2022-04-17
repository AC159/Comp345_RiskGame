#include <iostream>
#include "Orders/Orders.h"
#include "Player/Player.h"
#include "GameEngine/GameEngine.h"
#include "CommandProcessor/CommandProcessing.h"
#include "PlayerStrategies/PlayerStrategies.h"


using namespace std;
using namespace Graph;
using namespace Players;

int main() {

    auto *logObserver = new LogObserver();
    Subject::attach(logObserver);

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

            // clear game log file in preparation for new logs
            ofstream ofs;
            ofs.open("gamelog.txt", ofstream::out | ofstream::trunc);
            ofs.close();

            PlayerStrategiesDriver();
        } else if (userInput == "2") {
            cout << "Part 2: Tournament mode" << endl;

            // clear game log file in preparation for new logs
            ofstream ofs;
            ofs.open("gamelog.txt", ofstream::out | ofstream::trunc);
            ofs.close();

            GameEngine::tournamentModeDriver();
        } else if (userInput != "3")
            cout << "ERROR: Invalid input!" << endl;
    } while (userInput != "3");

    delete logObserver;
    return 0;
}