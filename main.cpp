#include <iostream>
#include "Orders/Orders.h"
#include "Map/Map.h"
#include "Player/Player.h"
#include "GameEngine/GameEngine.h"

using namespace std;
using namespace Graph;
using namespace Players;

int main() {

//    string userInput;
//
//    do {
//        cout
//                << "\nCommand list:\n1. Part 1: Command processor and command adapter\n2. Part 2: Game startup phase\n3. Part 3: Game play - main game loop\n4. Part 4: Order execution implementation\n5. Part 5: Game log observer - commands and orders\n6. Exit\nSelect the part you would like to run:"
//                << endl;
//        cin >> userInput;
//
//        if (userInput == "1") {
//            cout << "Part 1: Command processor and command adapter" << endl;
//            //call driver for Command processor and command adapter here
//        } else if (userInput == "2") {
//            cout << "Part 2: Game startup phase" << endl;
//            //call driver for Game startup phase here
//        } else if (userInput == "3") {
//            cout << "Part 3: Game play - main game loop" << endl;
//            //call driver for Game play: main game loop here
//        } else if (userInput == "4") {
//            cout << "Part 4: Order execution implementation" << endl;
//            Orders::ordersDriver();
//        } else if (userInput == "5") {
//            cout << "Part 5: Game log observer - commands and orders" << endl;
//            //call driver for Game log observer: commands and orders here
//        } else if (userInput != "6")
//            cout << "ERROR: Invalid input!" << endl;
//    } while (userInput != "6");

    Orders::ordersDriver();

    return 0;
}
