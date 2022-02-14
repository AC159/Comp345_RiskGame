#include <iostream>
#include "Map.h"

using namespace std;
using namespace Graph;

void Graph::Map::mapDriver() {

    MapLoader ml2{};
    //valid
    bool result = ml2.loadMap("../WarzoneMaps/canada/canada.map");
    bool mapIsValid = ml2.map->validate();
    cout << boolalpha << "map is valid = " << mapIsValid << endl;

    MapLoader ml3{};
    // invalid: not connected graph
    bool invalidResult = ml3.loadMap("../WarzoneMaps/solar/invalidsmallsolar.map");
    bool mapIsValid3 = ml3.map->validate();
    cout << boolalpha << "map is valid = " << mapIsValid3 << endl;

    MapLoader ml4{};
    //invalid: duplicate country
    bool invalidResult2 = ml4.loadMap("../WarzoneMaps/solar/smallsolarduplicates.map");
    bool mapIsValid4 = ml4.map->validate();
    cout << boolalpha << "map is valid = " << mapIsValid4 << endl;

}
