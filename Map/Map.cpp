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
    delete this->owner;
}


// ================= Map Class =====================

Map::Map() {

    string s = "Quebec";
    Territory* t1  = new Territory{1, 1, s};

    string s2 = "Ontario";
    Territory* t2 = new Territory{1, 1, s2};

    string s3 = "Alberta";
    Territory* t3 = new Territory{2, 1, s3};

    cout << "Inserting territories into map" << endl;
    vector<Territory *> v;
    v.push_back(t1);
    v.push_back(t2);
    v.push_back(t3);

}

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
    for (Territory *territory : this->territories) {
        delete territory;
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

void MapLoader::loadMap(const string &path) {
    ifstream file {path};
    string line;

    bool continents {false};
    bool borders {false};
    bool countries {false};
    vector<string> words {};
    string space_delimiter {" "};

    while(getline(file, line)) {
        if (line.contains("[countries]")) {
            countries = true;
            continents = false;
        }
        if (line.contains("[continents]")) continents = true;
        if (line.contains("[borders]")) {
            countries = false;
        }
        if (countries && !line.contains("[countries]")) {
            if (line.empty() || line == "\r") continue;

            size_t pos {0};
            while ((pos = line.find(space_delimiter)) != string::npos) {
                words.push_back(line.substr(0, pos));
                line.erase(0, pos + space_delimiter.length());
            }
            Territory* t = new Territory(stoi(words.at(0)), stoi(words.at(2)), words.at(1));
            this->map->territories.push_back(t);
            words.clear();
            cout << *t << endl;
//            delete t;
        }
    }

    file.close();
}

MapLoader::~MapLoader() {
    delete map;
}


