#include "Player.h"
// #include "..\Map\Map.h"
// #include "..\Cards\Cards.h"
#include <iostream>
using namespace std;

int main(){
    // test territories
    Territory *Portugal = new Territory(1, 1, *(new string("Portugal")));
    Territory *Spain = new Territory(2, 1, *(new string("Spain")));
    Territory *France = new Territory(3, 1, *(new string("France")));
    Territory *Germany = new Territory(4, 1, *(new string("Germany")));
    Territory *Switzerland = new Territory(5, 1, *(new string("Switzerland")));
    Territory *Italy = new Territory(6, 1, *(new string("Italy")));
    Territory *Belgium = new Territory(7, 1, *(new string("Belgium")));
    Territory *Netherlands = new Territory(8, 1, *(new string("Netherlands")));

    // test edges of the territories
    Edge *PS = new Edge(*Portugal, *Spain);
    Edge *SF = new Edge(*Spain, *France);
    Edge *FG = new Edge(*France, *Germany);
    Edge *GS = new Edge(*Germany, *Spain);
    Edge *FSw = new Edge(*France, *Switzerland);
    Edge *GSw = new Edge(*Germany, *Switzerland);
    Edge *SwI = new Edge(*Switzerland, *Italy);
    Edge *FB = new Edge(*France, *Belgium);
    Edge *BN = new Edge(*Belgium, *Netherlands);
    Edge *GB = new Edge(*Germany, *Belgium);
    Edge *GN = new Edge(*Germany, *Netherlands);

    // create a list of the edges
    list<Edge*> *edges = new list<Edge*>;
    list<Edge*>::iterator it = edges->begin();
    edges->insert(it, PS);
    it++;
    edges->insert(it,SF);
    it++;
    edges->insert(it,FG);
    it++;
    edges->insert(it,FSw);
    it++;
    edges->insert(it,GSw);
    it++;
    edges->insert(it,SwI);
    it++;
    edges->insert(it,FB);
    it++;
    edges->insert(it,BN);
    it++;
    edges->insert(it,GB);
    it++;
    edges->insert(it,GN);

    // test players
    string *playerOneName = new string("One");
    Player *playerOne = new Player(*playerOneName);

    cout << endl;
    cout << "===========================" << endl;
    cout << "Created player " << *playerOne << endl;
    

    // insert France, Germany and Switzerland to player one's territories
    // playerOne->getTerritories()->insert(pair<int, Territory*>(Portugal->countryNumber, Portugal));
    // playerOne->getTerritories()->insert(pair<int, Territory*>(Spain->countryNumber, Spain));
    playerOne->territories.insert(pair<int, Territory*>(France->countryNumber, France));
    playerOne->territories.insert(pair<int, Territory*>(Germany->countryNumber, Germany));
    playerOne->territories.insert(pair<int, Territory*>(Switzerland->countryNumber, Switzerland));
    // playerOne->getTerritories()->insert(pair<int, Territory*>(Italy->countryNumber, Italy));
    // playerOne->getTerritories()->insert(pair<int, Territory*>(Belgium->countryNumber, Belgium));
    // playerOne->getTerritories()->insert(pair<int, Territory*>(Netherlands->countryNumber, Netherlands));

    cout << "Inserted Territory: Adjacent Territories in player's territories" << endl;
    cout << "\tFrance: Spain, Germany, Belgium, Switzerland" << endl;
    cout << "\tGermany: France, Switzerland, Belgium, Netherlands" << endl;
    cout << "\tSwitzerland: France, Germany" << endl;
    cout << endl;

    // test cards
    Bomb *bombCard = new Bomb();
    Airlift *airliftCard = new Airlift();

    // insert bomb and airlift cards
    playerOne->hand->cards.emplace_back(bombCard);
    playerOne->hand->cards.emplace_back(airliftCard);

    // playerOne's hand
    cout << "Player " << *playerOne << "' hand:" << endl;
    for(vector<Card*>::iterator it = playerOne->hand->cards.begin(); it != playerOne->hand->cards.end(); it++){
        cout << (*it)->getType() << endl;
    }
    cout << endl;

    // test method Player::toDefend()
    map<int, Territory*> territoriesToDefend = playerOne->toDefend();
    cout << "Player " << *playerOne << "'s returned list from method Player::toDefend(): " << endl;
    for(map<int,Territory*>::iterator it = territoriesToDefend.begin(); it != territoriesToDefend.end(); it++){
        cout << it->second->name << endl;
    }
    cout << endl;
 
 
    // test method Player::toAttack()
    map<int, Territory*> temp = playerOne->toAttack(*edges);
    cout << "Player " << (*playerOne) << "'s returned list from method Player::toAttack(): " << endl;
    for(map<int, Territory*>::iterator it = temp.begin(); it != temp.end(); it++){
        cout << (it->second->name) << endl;
    }

    cout << endl;
    cout << "===========================" << endl;
    cout << "Test assignment operator"<< endl;

    // test assignment operator
    string *playerTwoName = new string("Two");
    Player *playerTwo = new Player(*playerTwoName);
    cout << "Created Player " << *playerTwo << endl;
    *playerTwo = *playerOne;
    cout << "Assigned playerOne to playerTwo" << endl;
    delete playerOne;
    cout << "Deleted playerOne" << endl;
    cout << endl;

    // playerTwo's hand
    cout << "Player " << *playerTwo << "' hand:" << endl;
    for(vector<Card*>::iterator it = playerTwo->hand->cards.begin(); it != playerTwo->hand->cards.end(); it++){
        cout << (*it)->getType() << endl;
    }
    cout << endl;

    // territories to defend for player two
    territoriesToDefend = playerTwo->toDefend();
    cout << "Player " << *playerTwo << "'s returned list from method Player::toDefend(): " << endl;
    for(map<int,Territory*>::iterator it = territoriesToDefend.begin(); it != territoriesToDefend.end(); it++){
        cout << (it->second->name) << endl;
    }
    cout << endl;

    // territories to attack for player two
    cout << "Player " << *playerTwo << "'s returned list from method Player::toAttack(): " << endl;
    temp = playerTwo->toAttack(*edges);
    for(map<int, Territory*>::iterator it = temp.begin(); it != temp.end(); it++){
        cout << (it->second->name) << endl;
    }

    cout << endl;
    cout << "===========================" << endl;
    cout << "Test copy constructor"<< endl;

    Player *playerThree = new Player(*playerTwo);
    cout << "Copied playerTwo to playerThree" << endl;
    delete playerTwo;
    cout << "Deleted playerTwo" << endl;
    cout << endl;

    // playerThree's hand
    cout << "Player " << *playerThree << "' hand:" << endl;
    for(vector<Card*>::iterator it = playerThree->hand->cards.begin(); it != playerThree->hand->cards.end(); it++){
        cout << (*it)->getType() << endl;
    }
    cout << endl;

    // territories to defend for player three
    territoriesToDefend = playerThree->toDefend();
    cout << "Player " << *playerThree << "'s returned list from method Player::toDefend(): " << endl;
    for(map<int,Territory*>::iterator it = territoriesToDefend.begin(); it != territoriesToDefend.end(); it++){
        cout << (it->second->name) << endl;
    }
    cout << endl;

    // territories to attack for player three
    cout << "Player " << *playerThree << "'s returned list from method Player::toAttack(): " << endl;
    temp = playerThree->toAttack(*edges);
    for(map<int, Territory*>::iterator it = temp.begin(); it != temp.end(); it++){
        cout << (it->second->name) << endl;
    }

    return 0;
}
