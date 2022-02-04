#include <iostream>
#include "Map.h"

using namespace std;

int main(int argc, char *argv[]) {

    MapLoader ml{};

    cout << ml << endl;
    //valid map
   // bool result = ml.loadMap("../WarzoneMaps/solar/smallsolar.map");
    cout << ml << endl;
    //invalid: not connected graph
    bool invalidResult = ml.loadMap("../WarzoneMaps/solar/invalidsmallsolar.map");
    //invalid: duplicate country
    //bool invalidResult = ml.loadMap("../WarzoneMaps/solar/smallsolarduplicates.map");

    bool mapIsValid = ml.map->validate();
    cout << boolalpha << "map is valid = " << mapIsValid << endl;



//    MapLoader ml2;
//    ml2 = ml;

    return 0;
}
