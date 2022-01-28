
#ifndef COMP345RISKGAME_MAP_H
#define COMP345RISKGAME_MAP_H

#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include "../Player/Player.h"

using namespace std;

// todo: add getters and setters for each class

// A territory/country is the smallest unit in the Map connected graph
class Territory {

public:
//    vector<Territory *> borders;
    Player* owner;
    int numberOfArmies;
    string name;
    int countryNumber;
    int continent;

    Territory();
    Territory(int countryNum, int continent, string &name);
    Territory(const Territory &territory); // copy constructor

    Territory& operator=(const Territory &territory); // overloaded assignment operator
    friend ostream& operator<<(ostream &out, const Territory &territory); // overloaded stream insertion operator

    ~Territory();

};

class Edge {
public:
    Territory* source;
    Territory* destination;

};

class Continent {
public:
    int continentNumber;
    string name;
    int bonusValue;
    vector<Territory *> territories;
};


class Map {

public:

    vector<Territory *> territories;
//    vector<Continent *> continents;
//    vector<Edge *> edges;

    Map();
    Map(const Map &map); // copy constructor
    Map& operator=(const Map &map); // overloaded assignment operator
    friend ostream& operator<<(ostream &out, const Map &map); // overloaded stream insertion operator

    bool validate();

    ~Map();

};


class MapLoader {

public:
    Map* map;

    MapLoader();
    MapLoader(const MapLoader &MapLoader); // copy constructor

    MapLoader& operator=(const MapLoader &loader); // overloaded assignment operator
    friend ostream& operator<<(ostream &out, const MapLoader &loader); // overloaded stream insertion operator

    void loadMap(const string &path);

    ~MapLoader();

};


#endif //COMP345RISKGAME_MAP_H
