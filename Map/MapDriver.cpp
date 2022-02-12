#include <iostream>
#include "Map.h"

using namespace std;

int main(int argc, char *argv[]) {

//    MapLoader map {};
//    cout << map << endl;
//    bool valid = map.loadMap("../WarzoneMaps/bigeurope/bigeurope.map");
//
//    if (!valid) {
//        cout << "Invalid .map file...exiting." << endl;
//        exit(1);
//    }

    MapLoader ml2{};
    // bool result = ml2.loadMap("../WarzoneMaps/canada/canada.map");
//    bool result = ml2.loadMap("../WarzoneMaps/solar/smallsolar.map");
//    cout << ml2 << endl;
    // invalid: not connected graph
     bool invalidResult = ml2.loadMap("../WarzoneMaps/solar/invalidsmallsolar.map");
    //invalid: duplicate country
//    bool invalidResult = ml2.loadMap("../WarzoneMaps/solar/smallsolarduplicates.map");

    bool mapIsValid = ml2.map->validate();
    cout << boolalpha << "map is valid = " << mapIsValid << endl;

    string s1 = "quebec";
    string s2 = "ontario";
    Territory *t1 = new Territory(1, 1, s1);
    Territory *t2 = new Territory(2, 2, s2);

    Edge e {*t1, *t2};
    Edge e2;
    e2 = e;
    e2.source->name = "alberta";
    cout << "Edge 1: " << e << endl;
    cout << "Edge 2: " << e2 << endl;

    delete t1;
    delete t2;

    return 0;
}
