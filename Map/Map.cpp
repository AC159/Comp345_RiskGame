#include <fstream>
#include <string>
#include "Map.h"

// ================= Territory Class =====================
//default constructor
Territory::Territory() {
    owner = nullptr;
    continent = -1;
    countryNumber = -1;
    numberOfArmies = -1;
}

// creates a territory
Territory::Territory(int countryNum, int continent, string &name) {
    this->name = name;
    this->countryNumber = countryNum;
    this->continent = continent;
    this->owner = nullptr; //player
    this->numberOfArmies = 0; //calculated depending on the number of territories a player has
}

// copy constructor
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

// how does the operator works/purpose? and what about delete owner line 50?
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
    out << "Name: " << territory.name << "\tContinent: " << territory.continent << "\tCountry: "
        << territory.countryNumber << "\tNbrOfArmies: " << territory.numberOfArmies << endl;
    return out;
}

// destructor
Territory::~Territory() {
//    cout << "Territory destructor invoked..." << endl;
    delete this->owner; // to confirm because we used new keyword on owner on line 28?
}

// ================= Edge Class =====================
// connection between territories and continents
Edge::Edge() {
    this->source = nullptr;
    this->destination = nullptr;
}

Edge::Edge(Territory &src, Territory &dest) {
    this->source = &src;
    this->destination = &dest;
}

// TODO copy constructor and assignment operator
ostream &operator<<(ostream &out, const Edge &edge) {
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

//TODO copy constructor and assignment operator
ostream &operator<<(ostream &out, const Continent &continent) {
    out << "Name: " << continent.name << "\tBonus value: " << continent.bonusValue << "\tContinent Nbr: "
        << continent.continentNumber << endl;
    return out;
}

Continent::~Continent() = default;

// ================= Map Class =====================

Map::Map() = default;

// copy constructor
Map::Map(const Map &map) {
    cout << "Map copy constructor invoked" << endl;
    // make a deep copy of all the territories in the map
    this->territories = vector<Territory *>(map.territories.size());
    for (auto *territory: map.territories) {
        this->territories.push_back(new Territory(*territory));
    }
}

Map &Map::operator=(const Map &map) {
    // Check if the current object and the passed object is the same
    if (this == &map) return *this;

    // deallocate all the memory associated with the territory pointers of this map
    for (auto *territory: this->territories) {
        delete territory;
        territory = nullptr; // pointer no longer dangling
    }
    this->territories.clear();

    // make a deep copy of all the territories in the map
    for (auto *territory: map.territories) {
        this->territories.push_back(new Territory(*territory));
    }
    return *this;
}

ostream &operator<<(ostream &out, const Map &map) {
    out << "Inside << overload for Map" << endl;
    // iterate over all elements of the map and print all countries by continent
    for (const auto *territory: map.territories) {
        out << *territory;
    }

    return out;
}

// Checks (1) the map is a connected graph (2) continent are connected subgraphs (3) each country belongs to one and only one continent
bool Map::validate() {
    // todo: implement this method

    //----------------------Part 3 -------------------------
    // validate that each country belongs to one and only one continent
    vector<string> *territoryName = new vector<string>;
    for(int i =0; i<Map::territories.size();i++){
        territoryName->push_back(Map::territories.at(i)->name);
        cout << "territory name: "<< territoryName->at(i)<<endl;
    }
    //iterate over vector and count the duplicates encountered
    vector<int> *counter = new vector<int>;
    for(int i =0; i<territoryName->size();i++){
        string tName1 = territoryName->at(i);
        for(int k=i+1;k<territoryName->size();k++){

            string tName2 = territoryName->at(k);
            //cout<< "name 1: " <<tName1 <<" name2: "<<tName2<<endl;
            if(tName1 == (tName2)){
                counter->push_back(1);
                cout<< "The same name! name 1: " <<tName1 <<" name2: "<<tName2<<endl;
                cout<<"INVALID MAP"<< endl;
            }
        }
    }cout<<"counter size: "<<counter->size()<<endl;

    //check if there are any duplicated egdes in the map file, if yes the map is invalid


    //TODO check for duplicate edges

    //----------------------Part 2 -------------------------
    //method/lambda to delete an edge and its reverse edge, given the edge index, reverse edge index, and an edge pair list
    auto deleteEdgeAndReverseEdge = [&](int edgeIndex, int reverseEdgeIndex, vector<pair<int, int>> *edgePairList) {
        if (edgeIndex < 0 || reverseEdgeIndex < 0 || edgeIndex == reverseEdgeIndex || reverseEdgeIndex < edgeIndex ||
            edgePairList->empty() == true) {
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
//                cout << "original edge: (" << edgePairList->at(edgeIndex).first << ", "
//                     << edgePairList->at(edgeIndex).second << ")\nreverse edge index: " << i << "\nreverse edge: ("
//                     << edgePairList->at(i).first << ", " << edgePairList->at(i).second << ")\n\n"; //printing original edge, reverse edge index, reverse edge
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

    cout << "----------Inside Map::validate() method----------\n";
    vector<pair<int, int>> *edgePairList = new vector<pair<int, int>>; //vector of type int number pair
    vector<int> *connected = new vector<int>; //vector to store connected territories
    vector<pair<int, int>> *disconnected = new vector<pair<int, int>>; //vector to store disconnected territories as a pair

    //if the number of edges is odd, then the map is invalid
    //each node must have 2 edges to any given node (outbound + inbound), so total number of edges must be even
    if (Map::edges.size() % 2 != 0) {
        cout << "One of the edges on this map goes in only one direction between territories. INVALID MAP!" << endl;
        return false;
    }

    //creating edges as pair list
    for (int i = 0; i < Map::edges.size(); i++) {
        edgePairList->push_back(
                make_pair(Map::edges.at(i)->source->countryNumber, Map::edges.at(i)->destination->countryNumber));
    }

    //using first element in the edgePairList as our starting point
    int reverseEdgeIndex = findReverseEdge(0, edgePairList);
    if (reverseEdgeIndex != -1) {
        connected->push_back(edgePairList->at(0).first);
        connected->push_back(edgePairList->at(0).second);
        deleteEdgeAndReverseEdge(0, reverseEdgeIndex, edgePairList);
    } else { //this handles cases where there is only an outbound/inbound edge, without its reverse existing; invalid map
        cout << "Could not find a reverse edge. This should not be possible! INVALID MAP!" << endl;
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
                if (!isConnected(edgeTerritory2, connected)) { //handles cases where first territory in the edge is connected, but second territory in edge is not connected
                    connected->push_back(edgeTerritory2);
                    deleteEdgeAndReverseEdge(0, reverseEdgeIndex, edgePairList);
                } else { //handles cases where first and second territory in the edge are connected
                    deleteEdgeAndReverseEdge(0, reverseEdgeIndex, edgePairList);
                }
            } else if (isConnected(edgeTerritory2, connected)) { //handles cases where second territory in the edge is connected, but first territory in edge is not connected
                connected->push_back(edgeTerritory1);
                deleteEdgeAndReverseEdge(0, reverseEdgeIndex, edgePairList);
            } else { //handles cases where neither first nor second territory in the edge are connected (i.e. both territories are disconnected)
                disconnected->push_back(make_pair(edgeTerritory1, edgeTerritory2));
                deleteEdgeAndReverseEdge(0, reverseEdgeIndex, edgePairList);
            }
        } else { //this handles cases where there is only an outbound/inbound edge, without its reverse existing; invalid map
            cout << "Could not find a reverse edge. This should not be possible! INVALID MAP!" << endl;
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
            if (!isConnected(edgeTerritory2, connected)) { //handles cases where first territory in the edge is connected, but second territory in edge is not connected
                connected->push_back(edgeTerritory2);
                disconnected->erase(disconnected->begin() + disconnectedCounter);
            } else { //handles cases where first and second territory in the edge are connected
                disconnected->erase(disconnected->begin() + disconnectedCounter);
            }
        } else if (isConnected(edgeTerritory2, connected)) { //handles cases where second territory in the edge is connected, but first territory in edge is not connected
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
            cout << "This map is disconnected. INVALID MAP!" << endl;
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
    delete territoryName;
    delete counter;

    return true;
}

Map::~Map() { //why are we destroying continent and edges when there is no new keyword
    cout << "Map destructor invoked..." << endl;
    for (Continent *cont: this->continents) {
        delete cont;
    }
    for (Territory *territory: this->territories) {
        delete territory;
    }
    for (Edge *edge: this->edges) {
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
    } catch (const ifstream::failure &e) {
        cout << "Cannot open file" << endl;
        return false;
    }

    if (!file.is_open()) {
        cout << "Unable to open file: " << path << endl;
        return false;
    }

    string line;
    string space_delimiter{" "};

    // Determine the presence of countries, continents and border identifiers
    bool countriesFlag{false};
    bool continentsFlag{false};
    bool bordersFlag{false};

    while (getline(file, line)) {
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
    vector<string> words{};
    size_t pos{0};
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

    ifstream file{path};
    string line;

    bool continents{false};
    int continentId{0};
    bool borders{false};
    bool countries{false};
    string space_delimiter{" "};

    while (getline(file, line)) {

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
            Continent *c = new Continent(continentId, words.at(0), stoi(words.at(1)));
            this->map->continents.push_back(c);
//            cout << *c << endl;
            words.clear();
        }

        if (countries && !line.contains("[countries]")) {
            vector<string> words = splitLine(line, space_delimiter);
            int territoryId = stoi(words.at(0));
            string territoryName = words.at(1);
            int continentOfTerritory = stoi(words.at(2));

            Territory *t = new Territory(territoryId, continentOfTerritory, territoryName);

            this->map->territories.push_back(t);
            // place this territory into its respective continent in the map
            // -1 because the continents vector is 0 indexed
            this->map->continents.at(continentOfTerritory - 1)->territories.push_back(t);
//            cout << *t << endl;
            words.clear();
        }

        if (borders && !line.contains("[borders]")) {
            vector<string> words = splitLine(line, space_delimiter);
            int countryId = stoi(words.at(0));
            Territory *src = this->map->territories.at(countryId - 1);
            for (int i = 1; i < words.size(); i++) {
                int destinationId = stoi(words.at(i));
                Territory *dest = this->map->territories.at(destinationId - 1);
                Edge *edge = new Edge(*src, *dest);
                this->map->edges.push_back(edge);
//                cout << *edge;
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


