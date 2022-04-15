#include <fstream>
#include <string>
#include "Map.h"
#include "../Player/Player.h"

using namespace std;
using namespace Graph;

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
    this->owner = territory.owner; // shallow copy will be made as overload assignment operator will be called for the Player class
    return *this;
}

bool Territory::operator<(const Territory &territory) const {
    return this->numberOfArmies < territory.numberOfArmies;
}

ostream& Graph::operator<<(ostream &out, const Territory &territory) {
    out << "Name: " << territory.name << "\tContinent: " << territory.continent << "\tCountry: " << territory.countryNumber << "\tNbrOfArmies: " << territory.numberOfArmies << endl;
    return out;
}

// updates this territory's owner and the old and new owners' list of territories accordingly
void Territory::transferOwnership(Players::Player *newOwner) {
    if (owner != nullptr) {
        owner->removeTerritory(*this);
    }
    if (newOwner != nullptr) {
        newOwner->addTerritory(*this);
    }
    owner = newOwner;
}

// returns all territories that are both adjacent to this territory and have a different owner
vector<Territory *> Territory::adjacentEnemyTerritories(const vector<Edge *> &mapEdges) {
    vector<Territory *> adjacentEnemyTerritories;
    for (const auto &edge : mapEdges) {
        if (edge->source == this && edge->destination->owner != owner) {
            adjacentEnemyTerritories.push_back(edge->destination);
        }
    }
    return adjacentEnemyTerritories;
}

// returns all territories that are both adjacent to this territory and have same owner
vector<Territory *> Territory::adjacentFriendlyTerritories(const vector<Edge *> &mapEdges) {
    vector<Territory *> adjacentFriendlyTerritories;
    for (const auto &edge : mapEdges) {
        if ((edge->source == this && edge->destination->owner == owner)) {
            adjacentFriendlyTerritories.push_back(edge->destination);
        }
    }
    return adjacentFriendlyTerritories;
}

// returns all territories that are both adjacent to this territory and have the given owner
std::vector<Territory *> Territory::adjacentTerritoriesOwnedBy(const Players::Player &p,
                                                               const vector<Edge *> &mapEdges) {
    vector<Territory *> adjacentTerritories;
    for (const auto &edge : mapEdges) {
        if ((edge->source == this && edge->destination->owner == &p)) {
            adjacentTerritories.push_back(edge->destination);
        }
    }
    return adjacentTerritories;
}

// returns a string of the territory's name and its current owner
std::string Territory::nameAndOwner() const {
    return name + " is owned by " + owner->getName() + "\n";
}

// returns a string of the territory's name and its current armies
std::string Territory::nameAndArmies() const {
    return name + " has " + to_string(numberOfArmies) + " armies\n";
}

// memory de-allocation of territory's owner should be handled externally
Territory::~Territory() = default;

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

ostream& Graph::operator<<(ostream &out, const Edge &edge) {
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

ostream& Graph::operator<<(ostream &out, const Continent &continent) {
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

ostream& Graph::operator<<(ostream &out, const Map &map) {
    out << "Inside << overload for Map" << endl;
    // iterate over all elements of the map and print all countries by continent
    for(const auto *territory : map.territories) {
        out << *territory;
    }
    return out;
}

// Checks (1) the map is a connected graph (2) continent are connected subgraphs (3) each country belongs to one and only one continent
bool Map::validate() {
    cout << "----------Inside Map::validate() method----------\n";

    //check for empty map or empty portions of the map
    if (Map::continents.size() == 0 || Map::territories.size() == 0 || Map::edges.size() == 0){
        cout << "ERROR: INVALID MAP! Check that continents, territories/countries, and edges on this map are not empty or undefined." << endl;
        return false;
    }

    //----------------------Part 3 -------------------------
    // validate that each country belongs to one and only one continent
    vector<string> *territoryName = new vector<string>;
    for (int i = 0; i < Map::territories.size(); i++) {
        territoryName->push_back(Map::territories.at(i)->name);
        cout << "territory name: " << territoryName->at(i) << endl;
    }

    //iterate over vector and count the duplicates encountered
    for (int i = 0; i < territoryName->size(); i++) {
        string tName1 = territoryName->at(i);
        for (int k = i + 1; k < territoryName->size(); k++) {
            string tName2 = territoryName->at(k);
            if (tName1 == (tName2)) {
                cout << "ERROR: INVALID MAP! Duplicate territory/country name found!" << endl;
                delete territoryName;
                return false;
            }
        }
    }

    delete territoryName; //delete territoryName vector from heap, as it is no longer needed

    //----------------------Part 2 -------------------------
    //method/lambda to delete an edge and its reverse edge, given the edge index, reverse edge index, and an edge pair list
    auto deleteEdgeAndReverseEdge = [&](int edgeIndex, int reverseEdgeIndex, vector<pair<int, int>> *edgePairList) {
        if (edgeIndex < 0 || reverseEdgeIndex < 0 || edgeIndex == reverseEdgeIndex || reverseEdgeIndex < edgeIndex || edgePairList->empty()) {
            cout << "ERROR: check edgeIndex and reverseEdgeIndex values; check that edgePairList is not empty." << endl;
            return false;
        }
        edgePairList->erase(edgePairList->begin() + edgeIndex);
        edgePairList->erase(edgePairList->begin() + reverseEdgeIndex -
                            1); //'-1' is required to adjust location of i, since the vector shrinks due to element deleted in previous line
        return true;
    };

    //method/lambda to find reverse edge of a given edge, in a given list; returns -1 if a reverse edge was not found
    auto findReverseEdge = [&](int edgeIndex, vector<pair<int, int>> *edgePairList) {
        int src = edgePairList->at(edgeIndex).first;
        int dst = edgePairList->at(edgeIndex).second;
        for (int i = 0; i < edgePairList->size(); i++) {
            if (edgePairList->at(i).first == dst && edgePairList->at(i).second == src) {
                return i;
            }
        }
        return -1;
    };

    //method/lambda to check if a given territory number is part of the connected graph
    auto isConnected = [=](int territoryNumber, vector<int> *connected) {
        for (int i = 0; i < connected->size(); i++) {
            if (territoryNumber == connected->at(i))
                return true;
        }
        return false;
    };

    auto isInContinents = [=](int continentNumber, vector<int> *continentNumbers) {
        for (int i = 0; i < continentNumbers->size(); i++) {
            if (continentNumber == continentNumbers->at(i))
                return true;
        }
        return false;
    };

    //if the number of edges is odd, then the map is invalid
    //each node must have 2 edges to any given node (outbound + inbound), so total number of edges must be even
    if (Map::edges.size() % 2 != 0) {
        cout << "ERROR: INVALID MAP! One of the edges on this map goes in only one direction between territories."
             << endl;
        return false;
    }

    vector<pair<int, int>> *edgePairList = new vector<pair<int, int>>; //vector of type int number pair
    vector<int> *connected = new vector<int>; //vector to store connected territories
    vector<pair<int, int>> *disconnected = new vector<pair<int, int>>; //vector to store disconnected territories as a pair
    vector<int> *continentNumbers = new vector<int>;

    //creating edges as pair list; adding continent numbers to continentNumbers vector for continent validation
    for (int i = 0; i < Map::edges.size(); i++) {
        edgePairList->push_back(
                make_pair(Map::edges.at(i)->source->countryNumber, Map::edges.at(i)->destination->countryNumber));
        if (!isInContinents(edges.at(i)->source->continent, continentNumbers))
            continentNumbers->push_back(edges.at(i)->source->continent);
        if (!isInContinents(edges.at(i)->destination->continent, continentNumbers))
            continentNumbers->push_back(edges.at(i)->destination->continent);
    }

    //check if there are any duplicated edges in the map file; if yes, the map is invalid
    //iterate over vector and count the duplicate edges encountered
    for(int i =0; i<edgePairList->size();i++) {
       pair<int,int> edgePair1 = make_pair(edgePairList->at(i).first,edgePairList->at(i).second);
        for (int k = i + 1; k < edgePairList->size(); k++) {
            pair<int,int> edgePair2 = make_pair(edgePairList->at(k).first,edgePairList->at(k).second);
            if ((edgePair1.first == edgePair2.first) && (edgePair1.second == edgePair2.second)) {
                cout << "ERROR: INVALID MAP! Duplicate edges found!" << endl;
                delete continentNumbers;
                delete disconnected;
                delete connected;
                delete edgePairList;
                return false;
            }
        }
    }

    //check to see if one or more continents are disconnected; if so, map is invalid!
    if (continentNumbers->size() != Map::continents.size()) {
        cout << "ERROR: INVALID MAP! One or more of the continents are not connected, or is duplicated." << endl;
        delete continentNumbers;
        delete disconnected;
        delete connected;
        delete edgePairList;
        return false;
    } else
        delete continentNumbers; //delete continentNumbers vector from heap, as it is no longer needed

    //using first element in the edgePairList as our starting point
    int reverseEdgeIndex = findReverseEdge(0, edgePairList);
    if (reverseEdgeIndex != -1) {
        connected->push_back(edgePairList->at(0).first);
        connected->push_back(edgePairList->at(0).second);
        deleteEdgeAndReverseEdge(0, reverseEdgeIndex, edgePairList);
    } else { //this handles cases where there is only an outbound/inbound edge, without its reverse existing; invalid map
        cout << "ERROR: INVALID MAP! Could not find a reverse edge. This should not be possible!" << endl;
        delete disconnected;
        delete connected;
        delete edgePairList;
        return false;
    }

    //filter out edges from edgePairList into connected or disconnected depending on their status in the graph
    while (edgePairList->size() != 0) {
        int reverseEdgeIndex = findReverseEdge(0, edgePairList);
        if (reverseEdgeIndex != -1) {
            int edgeTerritory1 = edgePairList->at(0).first;
            int edgeTerritory2 = edgePairList->at(0).second;
            if (isConnected(edgeTerritory1, connected)) {
                if (!isConnected(edgeTerritory2,
                                 connected)) { //handles cases where first territory in the edge is connected, but second territory in edge is not connected
                    connected->push_back(edgeTerritory2);
                    deleteEdgeAndReverseEdge(0, reverseEdgeIndex, edgePairList);
                } else { //handles cases where first and second territory in the edge are connected
                    deleteEdgeAndReverseEdge(0, reverseEdgeIndex, edgePairList);
                }
            } else if (isConnected(edgeTerritory2,
                                   connected)) { //handles cases where second territory in the edge is connected, but first territory in edge is not connected
                connected->push_back(edgeTerritory1);
                deleteEdgeAndReverseEdge(0, reverseEdgeIndex, edgePairList);
            } else { //handles cases where neither first nor second territory in the edge are connected (i.e. both territories are disconnected)
                disconnected->push_back(make_pair(edgeTerritory1, edgeTerritory2));
                deleteEdgeAndReverseEdge(0, reverseEdgeIndex, edgePairList);
            }
        } else { //this handles cases where there is only an outbound/inbound edge, without its reverse existing; invalid map
            cout << "ERROR: INVALID MAP! Could not find a reverse edge. This should not be possible!" << endl;
            delete disconnected;
            delete connected;
            delete edgePairList;
            return false;
        }
    }

    //variables and loop to check disconnected graph/list and ensure that "disconnected" edges that later become connected are correctly moved to the connected graph/list
    int disconnectedSize = disconnected->size();
    int disconnectedCounter = 0;
    while (disconnected->size() != 0) {
        int edgeTerritory1 = disconnected->at(disconnectedCounter).first;
        int edgeTerritory2 = disconnected->at(disconnectedCounter).second;
        if (isConnected(edgeTerritory1, connected)) {
            if (!isConnected(edgeTerritory2,
                             connected)) { //handles cases where first territory in the edge is connected, but second territory in edge is not connected
                connected->push_back(edgeTerritory2);
                disconnected->erase(disconnected->begin() + disconnectedCounter);
            } else { //handles cases where first and second territory in the edge are connected
                disconnected->erase(disconnected->begin() + disconnectedCounter);
            }
        } else if (isConnected(edgeTerritory2,
                               connected)) { //handles cases where second territory in the edge is connected, but first territory in edge is not connected
            connected->push_back(edgeTerritory1);
            disconnected->erase(disconnected->begin() + disconnectedCounter);
        } else { //handles cases where neither first nor second territory in the edge are connected (i.e. both territories are disconnected)
            disconnectedCounter++;
        }

        if (disconnectedSize > disconnected->size())
            disconnectedSize = disconnected->size();
        else if (disconnectedSize == disconnected->size()) {
            //displaying final state of graph when disconnected; edgePairList size must be 0; disconnected size must NOT be 0!
            cout << "Map::edges size = " << Map::edges.size() << endl;
            cout << "edgePairList size = " << edgePairList->size() << endl;
            cout << "number of territories = " << Map::territories.size() << endl;
            cout << "number of connected territories = " << connected->size() << endl;
            cout << "number of disconnected territories = " << Map::territories.size() - connected->size() << endl;
            cout << "ERROR: INVALID MAP! This map is disconnected." << endl;
            delete disconnected;
            delete connected;
            delete edgePairList;
            return false;
        }
    }

    //displaying final state of graph when connected; edgePairList and disconnected size must be 0!
    cout << "Map::edges size = " << Map::edges.size() << endl;
    cout << "edgePairList size = " << edgePairList->size() << endl;
    cout << "number of territories = " << Map::territories.size() << endl;
    cout << "number of connected territories = " << connected->size() << endl;
    cout << "number of disconnected territories = " << Map::territories.size() - connected->size() << endl;

    //calling destructors to delete vectors from heap
    delete disconnected;
    delete connected;
    delete edgePairList;

    return true;
}

Map::~Map() {
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

//returns whether given territories are neighbors
bool Map::edgeExists(Territory *t1, Territory *t2) {
    return std::find_if(edges.begin(), edges.end(),
                        [t1, t2](Edge *e){return e->source == t1 && e->destination == t2;}) != edges.end();
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

ostream& Graph::operator<<(ostream &out, const MapLoader &loader) {
    out << "Inside << overload for MapLoader" << endl;
    out << *loader.map;
    return out;
}

bool MapLoader::validateMapFile(const string &path) {
    cout << "Validating .map file..." << endl;
    ifstream file;

    try {
        file.open(path);
    } catch (const ifstream::failure &e) {
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
            int continentIndex = continentOfTerritory-1; // -1 because the continents vector is 0 indexed
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
    delete map;
    map = nullptr;
}


