
#ifndef COMP345RISKGAME_MAP_H
#define COMP345RISKGAME_MAP_H

#include <iostream>
#include <vector>
#include <string>

// Forward declare that there is a Player class in the Players namespace
namespace Players {
    class Player;
}

namespace Graph {
    class Territory;
    class Edge;
    class Continent;
    class Map;
    class MapLoader;
    std::ostream& operator<<(std::ostream &out, const Territory &territory);
    std::ostream& operator<<(std::ostream &out, const Edge &edge);
    std::ostream& operator<<(std::ostream &out, const Continent &continent);
    std::ostream& operator<<(std::ostream &out, const Map &map);
    std::ostream& operator<<(std::ostream &out, const MapLoader &loader);
}

// A territory/country is the smallest unit in the Map connected graph
class Graph::Territory {

public:
    Players::Player* owner;
    int numberOfArmies;
    std::string name;
    int countryNumber;
    int continent;

    Territory();
    Territory(int countryNum, int continent, std::string &name);
    Territory(const Territory &territory); // copy constructor

    Territory& operator=(const Territory &territory); // overloaded assignment operator
    bool operator<(const Territory &territory) const;
    friend std::ostream& operator<<(std::ostream &out, const Territory &territory); // overloaded stream insertion operator
    void transferOwnership(Players::Player *newOwner);
    std::vector<Territory *> adjacentEnemyTerritories(const std::vector<Edge *>& mapEdges);
    std::vector<Territory *> adjacentFriendlyTerritories(const std::vector<Edge *> &mapEdges);
    std::vector<Territory *> adjacentTerritoriesOwnedBy(const Players::Player &p, const std::vector<Edge *> &mapEdges);
    [[nodiscard]] std::string nameAndOwner() const;
    [[nodiscard]] std::string nameAndArmies() const;

    ~Territory();

};

class  Graph::Edge {
public:
    Territory* source;
    Territory* destination;

    Edge();
    Edge(Territory &src, Territory &dest);
    Edge(const Edge &edge); // copy constructor

    Edge& operator=(const Edge &edge); // overloaded assignment operator
    friend std::ostream& operator<<(std::ostream &out, const Edge &edge);

    ~Edge();

};

class  Graph::Continent {
public:
    int continentNumber;
    std::string name;
    int bonusValue;
    std::vector<Territory *> territories;

    Continent();
    Continent(int continentNum, std::string &name, int bonus);
    Continent(const Continent &continent);

    Continent& operator=(const Continent &continent);
    friend std::ostream& operator<<(std::ostream &out, const Continent &continent); // overloaded stream insertion operator

    ~Continent();
};


class  Graph::Map {

public:

    std::vector<Territory *> territories;
    std::vector<Continent *> continents;
    std::vector<Edge *> edges;

    Map();
    Map(const Map &map); // copy constructor
    Map& operator=(const Map &map); // overloaded assignment operator
    friend std::ostream& operator<<(std::ostream &out, const Map &map); // overloaded stream insertion operator

    bool validate();
    bool edgeExists(Territory *t1, Territory *t2); //returns whether given territories are adjacent

    ~Map();

};


class  Graph::MapLoader {
public:
    Map* map;

    // These values specify the minimum number of attributes that continents, countries and borders should specify in the .map file
    static const int NBR_CONTINENT_ATTR = 3;
    static const int NBR_COUNTRY_ATTR = 5;
    static const int NBR_BORDER_ATTR = 2;

    MapLoader();
    MapLoader(const MapLoader &MapLoader); // copy constructor

    MapLoader& operator=(const MapLoader &loader); // overloaded assignment operator
    friend std::ostream& operator<<(std::ostream &out, const MapLoader &loader); // overloaded stream insertion operator

    static bool isLineEmpty(std::string &line);
    static std::vector<std::string> splitLine(std::string line, std::string &delimiter); // helper function to help split lines while reading .map files
    static bool validateMapFile(const std::string &path); // helper function that makes preliminary checks on a .map file
    bool loadMap(const std::string &path) const;

    ~MapLoader();

};


#endif //COMP345RISKGAME_MAP_H
