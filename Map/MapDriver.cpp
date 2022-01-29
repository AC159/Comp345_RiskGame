#include <iostream>
#include "Map.h"

using namespace std;

int main(int argc, char *argv[]) {

    MapLoader ml {};
    cout << ml << endl;
    bool result = ml.loadMap("../WarzoneMaps/bigeurope/bigeurope.map");

//    MapLoader ml2;
//    ml2 = ml;

    return 0;
}
