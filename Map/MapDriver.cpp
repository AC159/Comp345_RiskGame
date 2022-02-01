#include <iostream>
#include "Map.h"

using namespace std;

int main(int argc, char *argv[]) {

    MapLoader ml {};
    cout << ml << endl;
    bool result = ml.loadMap("../WarzoneMaps/bigeurope/bigeurope.map");

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
