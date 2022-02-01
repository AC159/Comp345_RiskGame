#include "Player.h"
#include "..\Map\Map.h"
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
    Player *playerOne = new Player("One");
    Player *playerTwo = new Player("Two");
    
    // insert France, Germany and Switzerland to player one's territories
    // playerOne->getTerritories()->insert(pair<int, Territory*>(Portugal->countryNumber, Portugal));
    // playerOne->getTerritories()->insert(pair<int, Territory*>(Spain->countryNumber, Spain));
    playerOne->getTerritories()->insert(pair<int, Territory*>(France->countryNumber, France));
    playerOne->getTerritories()->insert(pair<int, Territory*>(Germany->countryNumber, Germany));
    playerOne->getTerritories()->insert(pair<int, Territory*>(Switzerland->countryNumber, Switzerland));
    // playerOne->getTerritories()->insert(pair<int, Territory*>(Italy->countryNumber, Italy));
    // playerOne->getTerritories()->insert(pair<int, Territory*>(Belgium->countryNumber, Belgium));
    // playerOne->getTerritories()->insert(pair<int, Territory*>(Netherlands->countryNumber, Netherlands));

    // test method Player::toDefend()
    map<int, Territory*> *territoriesToDefend = playerOne->toDefend();
    cout << "Player " << *playerOne << " returned list of method Player::toDefend(): " << endl;
    for(map<int,Territory*>::iterator it = territoriesToDefend->begin(); it != territoriesToDefend->end(); it++){
        cout << (it->second->name) << endl;
    }
    cout << endl;
    delete(territoriesToDefend);
    territoriesToDefend = nullptr;
 
    // test method Player::toAttack()
    map<int, Territory*> *temp = playerOne->toAttack(*edges);
    cout << "Player " << (*playerOne) << " returned list of method::toAttack(): " << endl;
    for(map<int, Territory*>::iterator it = temp->begin(); it != temp->end(); it++){
        cout << (it->second->name) << endl;
    }


    // test assignment operator
    playerTwo = playerOne;
    delete playerOne;

    cout << "===========================" << endl;
    cout << "Test assignment operator"<< endl;

    // territories to defend for player two
    territoriesToDefend = playerTwo->toDefend();
    cout << "Player " << *playerTwo << " returned list of method Player::toDefend(): " << endl;
    for(map<int,Territory*>::iterator it = territoriesToDefend->begin(); it != territoriesToDefend->end(); it++){
        cout << (it->second->name) << endl;
    }
    cout << endl;
    delete(territoriesToDefend);
    territoriesToDefend = nullptr;

    // territories to attack for player two
    cout << "Player " << *playerTwo << " returned list of method::toAttack(): " << endl;
    temp = playerTwo->toAttack(*edges);
    for(map<int, Territory*>::iterator it = temp->begin(); it != temp->end(); it++){
        cout << (it->second->name) << endl;
    }

    return 0;
}
