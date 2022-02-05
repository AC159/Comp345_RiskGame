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
    this->owner = territory.owner; // shallow copy of Player object
}

Territory& Territory::operator=(const Territory &territory) {
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
    this->owner = territory.owner; // shallow copy will be made as overload assignment operator will be called for the Player class
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

Edge::Edge(const Edge &edge) {
    // shallow copy of the Territory objects
    this->source = edge.source;
    this->destination = edge.destination;
}

Edge& Edge::operator=(const Edge &edge) {
     if (this == &edge) return *this;
     delete this->source;
     delete this->destination;
     // shallow copy
     this->source = edge.source;
     this->destination = edge.destination;
     return *this;
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

Continent::Continent(const Continent &continent) {
    this->continentNumber = continent.continentNumber;
    this->bonusValue = continent.bonusValue;
    this->name = continent.name;

    // make shallow copies of all territories in this continent
    for (Territory *t : continent.territories) {
        Territory *territory = t;
        this->territories.push_back(territory);
    }
}

Continent& Continent::operator=(const Continent &continent) {
    if (this == &continent) return *this;

    this->continentNumber = continent.continentNumber;
    this->bonusValue = continent.bonusValue;
    this->name = continent.name;
    this->territories.clear(); // remove all the territory pointers owned by the current continent
    for (Territory *t : continent.territories) {
        Territory *territory = t;
        this->territories.push_back(territory);
    }
    return *this;
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
    // make a shallow copy of all the territories in the map
    this->territories = vector<Territory *>(map.territories.size());
    this->continents = vector<Continent *>(map.continents.size());
    this->edges = vector<Edge *>(map.edges.size());
    for(auto *territory : map.territories) {
        this->territories.push_back(territory); // territory pointer will be copied when pushed into vector
    }
    for(auto *continent : map.continents) {
        this->continents.push_back(continent); // continent pointer will be copied when pushed into vector
    }
    for(auto *edge : map.edges) {
        this->edges.push_back(edge); // edge pointer will be copied when pushed into vector
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
    this->continents.clear();
    this->edges.clear();

    // make a shallow copy of all the territories in the map
    for(auto *territory : map.territories) {
        this->territories.push_back(territory);
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
    for (Edge *edge : this->edges) {
        delete edge;
        edge = nullptr;
    }
    for (Continent *cont : this->continents) {
        delete cont;
        cont = nullptr;
    }
    for (Territory *territory : this->territories) {
        delete territory;
        territory = nullptr;
    }
    this->territories.clear();
    this->continents.clear();
    this->edges.clear();
}

// ================= MapLoader Class =====================

MapLoader::MapLoader() {
    this->map = new Map();
}

MapLoader::MapLoader(const MapLoader &MapLoader) {
    this->map = MapLoader.map; // shallow copy of map object
}

MapLoader &MapLoader::operator=(const MapLoader &loader) {
    cout << "Inside = overload for MapLoader..." << endl;
    if (this == &loader) return *this;
    delete this->map;
    this->map = loader.map; // shallow copy of map object
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

    bool countriesSection {false};
    bool continentsSection {false};
    bool bordersSection {false};

    bool hasContinents {false};
    bool invalidContinent {false};
    bool hasCountries {false};
    bool invalidCountry {false};
    bool hasBorders {false};
    bool invalidBorder {false};

    int nbrOfContinents {0};
    int nbrOfCountries {0};

    while(getline(file, line)) {
        bool emptyLine = isLineEmpty(line);
        if (line.contains("[countries]")) {
            countriesFlag = true;
            countriesSection = true;
            continentsSection = false;
        }
        if (line.contains("[continents]")) {
            continentsFlag = true;
            continentsSection = true;
        }
        if (line.contains("[borders]")) {
            bordersFlag = true;
            bordersSection = true;
            countriesSection = false;
            continentsSection = false;
        }
        if (continentsSection && !line.contains("[continents]")) {
            vector<string> words = splitLine(line, space_delimiter);
            if (words.size() < NBR_CONTINENT_ATTR && !emptyLine) {
                invalidContinent = true;
                break;
            } else if (!emptyLine) {
                hasContinents = true;
                nbrOfContinents++;
            }
        }
        if (countriesSection && !line.contains("[countries]")) {
            vector<string> words = splitLine(line, space_delimiter);
            if (words.size() < NBR_COUNTRY_ATTR && !emptyLine) {
                invalidCountry = true;
                break;
            } else if (!emptyLine) {
                hasCountries = true;
                nbrOfCountries++;
                // Determine if the country references an invalid continent number
                int continentId = stoi(words.at(2));
                if (continentId-1 >= nbrOfContinents) {
                    cout << "Invalid .map file: country " << words.at(1) << " references invalid continent #" << continentId << endl;
                    return false;
                }
            }
        }
        if (bordersSection && !line.contains("[borders]")) {
            vector<string> words = splitLine(line, space_delimiter);
            if (words.size() < NBR_BORDER_ATTR && !emptyLine) {
                invalidBorder = true;
                break;
            } else if (!emptyLine) {
                hasBorders = true;
                // Determine if the border references invalid country numbers
                int sourceCountryId = stoi(words.at(0));
                if (sourceCountryId-1 >= nbrOfCountries) {
                    cout << "Invalid .map file: border references invalid country #" << sourceCountryId << endl;
                    return false;
                }
                for (int i = 1; i < words.size(); i++) {
                    int destinationId = stoi(words.at(i));
                    if (destinationId-1 >= nbrOfCountries) {
                        cout << "Invalid .map file: border references invalid country #" << destinationId << endl;
                        return false;
                    }
                }
            }
        }
    }
    file.close();

    if (invalidContinent) {
        cout << "Invalid .map file: invalid continent specified... continents should have at least " << NBR_CONTINENT_ATTR << " attributes." << endl;
        return false;
    }
    if (invalidCountry) {
        cout << "Invalid .map file: invalid country specified... countries should have at least " << NBR_COUNTRY_ATTR << " attributes." << endl;
        return false;
    }
    if (invalidBorder) {
        cout << "Invalid .map file: invalid border specified... borders should have at least " << NBR_BORDER_ATTR << " attributes." << endl;
        return false;
    }
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
    if (!hasContinents || !hasCountries || !hasBorders) {
        cout << "Invalid .map file: there must be at least one continent, country and border..." << endl;
        return false;
    }

    return true;
}

bool MapLoader::isLineEmpty(string &line) {
    for (const char &c : line) {
        if (c != '\r' && c != '\t' && c != '\n' && c != ' ') return false;
    }
    return true;
}

vector<string> MapLoader::splitLine(string line, string &delimiter) {
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

    bool validated {false};
    try {
        validated = validateMapFile(path);
    } catch (const exception& ex) {
        cout << "Invalid .map file: " << ex.what() << endl;
        return false;
    }
    if (!validated) return validated;
    cout << "Valid .map file" << endl;

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
            int continentIndex = continentOfTerritory-1;
            this->map->continents.at(continentIndex)->territories.push_back(t);
            cout << *t << endl;
            words.clear();
        }

        if (borders && !line.contains("[borders]")) {
            vector<string> words = splitLine(line, space_delimiter);
            int sourceCountryId = stoi(words.at(0));
            Territory* src = this->map->territories.at(sourceCountryId-1);
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
    return true;
}

MapLoader::~MapLoader() {
    cout << "MapLoader destructor invoked..." << endl;
    delete map;
    map = nullptr;
}
