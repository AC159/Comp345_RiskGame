#include <fstream>
#include <string>
#include "Map.h"

// ================= Territory Class =====================

Territory::Territory() {
    owner = nullptr;
    continent = -1;
    countryNumber = -1;
    numberOfArmies = -1;
}

Territory::Territory(int countryNum, int continent, string &name) {
    this->name = name;
    this->countryNumber = countryNum;
    this->continent = continent;
    this->owner = nullptr;
    this->numberOfArmies = 0;
}

Territory::Territory(const Territory &territory) {
    cout << "Inside copy constructor of Territory: " << territory.name << endl;
    this->continent = territory.continent;
    this->countryNumber = territory.countryNumber;
    this->name = territory.name;
    this->numberOfArmies = territory.numberOfArmies;
    this->owner = new Player(*territory.owner); // deep copy of Player object

    // this->borders = territory.borders; // shallow copy
//    this->borders = vector<Territory *>(territory.borders.size());
//    // make a deep copy of all the borders of this territory
//    for (const Territory* t : territory.borders) {
//        this->borders.push_back(new Territory(*t));
//    }
}

Territory &Territory::operator=(const Territory &territory) {
    cout << "Inside = overload of Territory: " << endl;

    // Check if the current object and the passed object is the same
    if (this == &territory) return *this;

    this->name = territory.name;
    this->continent = territory.continent;
    this->countryNumber = territory.countryNumber;
    this->numberOfArmies = territory.numberOfArmies;
//    this->borders.clear();
//    this->borders = territory.borders;
    delete this->owner;
    this->owner = territory.owner; // deep copy will be made as overload assignment operator will be called for the Player class
    return *this;
}

ostream &operator<<(ostream &out, const Territory &territory) {
    out << "Name: " << territory.name << "\tContinent: " << territory.continent << "\tCountry: " << territory.countryNumber << "\tNbrOfArmies: " << territory.numberOfArmies << endl;
    return out;
}

Territory::~Territory() {
    cout << "Territory destructor invoked..." << endl;
    delete this->owner;
}

// ================= Edge Class =====================

Edge::Edge() {
    this->source = nullptr;
    this->destination = nullptr;
}

Edge::Edge(Territory &src, Territory &dest) {
    this->source = &src;
    this->destination = &dest;
}

ostream& operator<<(ostream &out, const Edge &edge) {
    out << "Edge: " << endl;
    out << "\tSource: " << "\t" << *edge.source << endl;
    out << "\tDestination: " << "\t" << *edge.destination << endl;
    return out;
}

Edge::~Edge() = default;

// ================= Continent Class =====================

Continent::Continent() {
    this->name = "";
    this->bonusValue = 0;
    this->continentNumber = -1;
}

Continent::Continent(int continentNum, string &name, int bonus) {
    this->continentNumber = continentNum;
    this->name = name;
    this->bonusValue = bonus;
}

ostream& operator<<(ostream &out, const Continent &continent) {
    out << "Name: " << continent.name << "\tBonus value: " << continent.bonusValue << "\tContinent Nbr: " << continent.continentNumber << endl;
    return out;
}

Continent::~Continent() = default;

// ================= Map Class =====================

Map::Map() = default;

Map::Map(const Map &map) {
    cout << "Map copy constructor invoked" << endl;
    // make a deep copy of all the territories in the map
    this->territories = vector<Territory *>(map.territories.size());
    for(auto *territory : map.territories) {
        this->territories.push_back(new Territory(*territory));
    }
}

Map &Map::operator=(const Map &map) {
    // Check if the current object and the passed object is the same
    if (this == &map) return *this;

    // deallocate all the memory associated with the territory pointers of this map
    for(auto *territory : this->territories) {
        delete territory;
        territory = nullptr; // pointer no longer dangling
    }
    this->territories.clear();

    // make a deep copy of all the territories in the map
    for(auto *territory : map.territories) {
        this->territories.push_back(new Territory(*territory));
    }
    return *this;
}

ostream &operator<<(ostream &out, const Map &map) {
    out << "Inside << overload for Map" << endl;
    // iterate over all elements of the map and print all countries by continent
    for(const auto *territory : map.territories) {
        out << *territory;
    }
    return out;
}

bool Map::validate() {
    // todo: implement this method
    return false;
}

Map::~Map() {
    cout << "Map destructor invoked..." << endl;
    for (Continent *cont : this->continents) {
        delete cont;
    }
    for (Territory *territory : this->territories) {
        delete territory;
    }
    for (Edge *edge : this->edges) {
        delete edge;
    }
}


// ================= MapLoader Class =====================

MapLoader::MapLoader() {
    map = new Map();
}

MapLoader::MapLoader(const MapLoader &MapLoader) {
    this->map = new Map(*MapLoader.map);
}

MapLoader &MapLoader::operator=(const MapLoader &loader) {
    cout << "Inside = overload for MapLoader..." << endl;
    if (this == &loader) return *this;
    delete this->map;
    this->map = new Map(*loader.map); // deep clone of map object
    return *this;
}

ostream &operator<<(ostream &out, const MapLoader &loader) {
    out << "Inside << overload for MapLoader" << endl;
    out << *loader.map;
    return out;
}

bool MapLoader::validateMapFile(const string &path) {
    cout << "Validating .map file..." << endl;
    ifstream file;

    try {
        file.open(path);
    } catch (const ifstream::failure& e) {
        cout << "Cannot open file" << endl;
        return false;
    }

    if (!file.is_open()) {
        cout << "Unable to open file: " << path << endl;
        return false;
    }

    string line;
    string space_delimiter {" "};

    // Determine the presence of countries, continents and border identifiers
    bool countriesFlag {false};
    bool continentsFlag {false};
    bool bordersFlag {false};

    while(getline(file, line)) {
        if (line.contains("[countries]")) countriesFlag = true;
        if (line.contains("[continents]")) continentsFlag = true;
        if (line.contains("[borders]")) bordersFlag = true;
    }
    file.close();

    if (!countriesFlag) {
        cout << "Invalid .map file: missing \"[countries]\" indicator..." << endl;
        return false;
    }
    if (!continentsFlag) {
        cout << "Invalid .map file: missing \"[continents]\" indicator..." << endl;
        return false;
    }
    if (!bordersFlag) {
        cout << "Invalid .map file: missing \"[borders]\" indicator..." << endl;
        return false;
    }

    return true;
}

vector<string> MapLoader::splitLine(string &line, string &delimiter) {
    vector<string> words {};
    size_t pos {0};
    while ((pos = line.find(delimiter)) != string::npos) {
        words.push_back(line.substr(0, pos));
        line.erase(0, pos + delimiter.length());
    }
    if (!line.empty() && line != " " && line != "\r" && line != "\n") {
        words.push_back(line.substr(0, line.length()));
    }
    return words;
}

bool MapLoader::loadMap(const string &path) const {

    bool validated = validateMapFile(path);
    if (!validated) return validated;

    ifstream file {path};
    string line;

    bool continents {false};
    int continentId {0};
    bool borders {false};
    bool countries {false};
    string space_delimiter {" "};

    while(getline(file, line)) {

        if (line.empty() || line == "\r") continue;

        if (line.contains("[countries]")) {
            countries = true;
            continents = false;
        }
        if (line.contains("[continents]")) continents = true;
        if (line.contains("[borders]")) {
            countries = false;
            borders = true;
        }

        if (continents && !line.contains("[continents]")) {
            continentId++;
            vector<string> words = splitLine(line, space_delimiter);
            Continent* c = new Continent(continentId, words.at(0), stoi(words.at(1)));
            this->map->continents.push_back(c);
            cout << *c << endl;
            words.clear();
        }

        if (countries && !line.contains("[countries]")) {
            vector<string> words = splitLine(line, space_delimiter);
            int territoryId = stoi(words.at(0));
            string territoryName = words.at(1);
            int continentOfTerritory = stoi(words.at(2));

            Territory* t = new Territory(territoryId, continentOfTerritory, territoryName);

            this->map->territories.push_back(t);
            // place this territory into its respective continent in the map
            // -1 because the continents vector is 0 indexed
            this->map->continents.at(continentOfTerritory-1)->territories.push_back(t);
            cout << *t << endl;
            words.clear();
        }

        if (borders && !line.contains("[borders]")) {
            vector<string> words = splitLine(line, space_delimiter);
            int countryId = stoi(words.at(0));
            Territory* src = this->map->territories.at(countryId-1);
            for (int i = 1; i < words.size(); i++) {
                int destinationId = stoi(words.at(i));
                Territory* dest = this->map->territories.at(destinationId-1);
                Edge* edge = new Edge(*src, *dest);
                this->map->edges.push_back(edge);
                cout << *edge;
            }
            words.clear();
        }

    }

    file.close();
}

MapLoader::~MapLoader() {
    cout << "MapLoader destructor invoked..." << endl;
    delete map;
}


