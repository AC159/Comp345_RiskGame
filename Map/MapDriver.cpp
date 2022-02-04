#include <iostream>
#include "Map.h"

using namespace std;

int main(int argc, char *argv[]) {

    MapLoader ml{};
    cout << ml << endl;
    bool result = ml.loadMap("../WarzoneMaps/solar/smallsolar.map");

    bool mapIsValid = ml.map->validate();
    cout << boolalpha << "map is valid = " << mapIsValid << endl;

//    MapLoader ml2;
//    ml2 = ml;

    return 0;
}
