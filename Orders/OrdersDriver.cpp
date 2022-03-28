#include "Orders.h"
#include "../Player/Player.h"
#include <iostream>

using namespace std;
using namespace Orders;
using namespace Graph;
using namespace Players;

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCDFAInspection" //ignore irrelevant warning

// prints territory's neighbors
void showAdjacent(const vector<Edge *> &edges, Territory *territory) {
    for (auto e: edges) {
        if (e->source == territory) {
            cout << "\t-" << e->destination->name << "" << endl;
        }
    }
}

#pragma clang diagnostic pop //warning suppression ends here

// assigns territory to given owner with given num of armies & displays territory info
void assignTerritory(const vector<Edge *> &edges, Territory *territory, Player *owner, int armyCount) {
    cout << "• Giving territory " << territory->name << " to " << owner->getName() << endl;
    cout << " > " << territory->name << " has " << armyCount << " armies and its neighbors are:" << endl;
    showAdjacent(edges, territory);
    territory->numberOfArmies = armyCount;
    territory->transferOwnership(owner);
}

// executes the given order and displays its stats before and after its execution
// note: the order should be dynamically allocated as this method deletes it
void testOrder(Order *order) {
    cout << order->issuer->getName() << " issued: " << *order << endl;
    order->displayStats(true);
    cout << "Calling execute()------- ";
    order->execute();
    order->displayStats(false);
    cout << endl;
    delete order;
}

// driver for Part 4: Order execution implementation
void Orders::ordersDriver() {
    cout << "\nCreating map loader and loading smallsolar map..." << endl;
    MapLoader ml;
    ml.loadMap("../WarzoneMaps/solar/solar.map");
    cout << "*******************************************************************************************" << endl;

    cout << "\nCreating players named PlayerOne and PlayerTwo..." << endl;
    Player p1 = Player("PlayerOne");
    Player p2 = Player("PlayerTwo");

    cout << "\nAssigning territories..." << endl;
    Territory *southNeptune = ml.map->territories[0];
    Territory *eastNeptune = ml.map->territories[1];
    Territory *northNeptune = ml.map->territories[3];
    Territory *eastPluto = ml.map->territories[4];
    Territory *westPluto = ml.map->territories[5];
    assignTerritory(ml.map->edges, eastPluto, &p1, 2);
    assignTerritory(ml.map->edges, southNeptune, &p1, 2);
    assignTerritory(ml.map->edges, eastNeptune, &p2, 2);
    assignTerritory(ml.map->edges, westPluto, &p2, 2);
    assignTerritory(ml.map->edges, northNeptune, &p2, 2);

    cout << "\nTesting Deploy execution...\n-----------------------------\n";
    p1.reinforcementPool = 10; //preparing reinforcement pool for deploy order
    testOrder(new Deploy(&p1, eastPluto, 7));//valid
    testOrder(new Deploy(&p1, westPluto, 1));//invalid- target doesn't belong to issuer

    cout << "\nTesting Advance execution...\n-----------------------------\n";
    testOrder(new Advance(&p1, ml.map, eastPluto, westPluto, 4));//valid: attacks & conquers territory
    testOrder(new Advance(&p1, ml.map, eastPluto, westPluto, 3));//valid: transfers armies
    testOrder(new Advance(&p1, ml.map, eastPluto, southNeptune, 2));//invalid: target not adjacent
    cout << "Note: each player's 'receivesCard' boolean is used to award one card per turn accordingly and is cleared "
            "each turn as seen in part 3\n";

    cout << "\nTesting Airlift execution...\n-----------------------------\n";
    testOrder(new Airlift(&p1, eastPluto, southNeptune, 2));//valid
    testOrder(new Airlift(&p1, eastPluto, northNeptune, 2));//invalid: target doesn't belong to issuer

    //code block testing bomb execution
    {
        cout << "\nTesting Bomb execution..." << endl;
        cout << "-----------------------------" << endl;

        //invalid; bombing your own territory
        testOrder(new Bomb(&p1, westPluto, ml.map));

        //invalid; territory not adjacent to any of issuing player's territories
        westPluto->transferOwnership(&p2);
        eastPluto->transferOwnership(&p2);
        testOrder(new Bomb(&p1, westPluto, ml.map));

        //valid; bombing enemy adjacent territory
        eastPluto->transferOwnership(&p1);
        testOrder(new Bomb(&p1, westPluto, ml.map));
    }

    //code block testing blockade execution
    {
        cout << "Testing Blockade execution..." << endl;
        cout << "-----------------------------" << endl;

        //invalid; placing blockade on territory not owned by player
        testOrder(new Blockade(&p2, westPluto));

        //valid; placing blockade on territory owned by player
        testOrder(new Blockade(&p1, westPluto));
    }

    //code block testing negotiate execution
    {
        cout << "Testing Negotiate execution..." << endl;
        cout << "-----------------------------" << endl;
        testOrder(new Negotiate(&p1, &p1));
        testOrder(new Negotiate(&p1, &p2));

        //attempting to bomb a player with negotiation in effect
        cout << "Attempting to bomb a territory whose owner has a negotiation in effect: " << endl;
        testOrder(new Bomb(&p2, southNeptune, ml.map));

        //attempting to advance on a territory owned by a player with negotiation in effect
        cout << "Attempting to advance on a territory whose owner has a negotiation in effect: " << endl;
        eastPluto->transferOwnership(&p1);
        testOrder(new Advance(&p1, ml.map, southNeptune, eastNeptune, 4));//valid: conquers territory
        cout << "Note: all players' negotiation lists are cleared each turn as seen in part 3\n\n";
    }
}