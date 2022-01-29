
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

    Edge();
    Edge(Territory &src, Territory &dest);
    // todo: implement overloaded assignment operator + copy constructor
    friend ostream& operator<<(ostream &out, const Edge &edge);

    ~Edge();

};

class Continent {
public:
    int continentNumber;
    string name;
    int bonusValue;
    vector<Territory *> territories;

    Continent();
    Continent(int continentNum, string &name, int bonus);
    friend ostream& operator<<(ostream &out, const Continent &continent); // overloaded stream insertion operator

    // todo: implement overloaded assignment operator + copy constructor

    ~Continent();

};


class Map {

public:

    vector<Territory *> territories;
    vector<Continent *> continents;
    vector<Edge *> edges;

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

    static vector<string> splitLine(string &line, string &delimiter); // helper function to help split lines while reading .map files
    static bool validateMapFile(const string &path); // helper function that makes preliminary checks on a .map file
    bool loadMap(const string &path) const;

    ~MapLoader();

};


#endif //COMP345RISKGAME_MAP_H
