#include "Orders.h"
#include "../Player/Player.h"
#include <iostream>

using namespace std;
using namespace Orders;
using namespace Graph;
using namespace Players;

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCDFAInspection" //ignore irrelevant warning

//prints territory's neighbors
void showAdjacent(const vector<Edge *> &edges, Territory *territory) {
    for (auto e: edges) {
        if (e->source == territory) {
            cout << "\t-" << e->destination->name << "" << endl;
        }
    }
}

#pragma clang diagnostic pop //warning suppression ends here

//assigns territory to given owner with given num of armies & displays territory info
void assignTerritory(const vector<Edge *> &edges, Territory *territory, Player *owner, int armyCount) {
    cout << "• Giving territory " << territory->name << " to " << owner->getName() << endl;
    cout << " > " << territory->name << " has " << armyCount << " armies and its neighbors are:" << endl;
    showAdjacent(edges, territory);
    territory->numberOfArmies = armyCount;
    territory->transferOwnership(owner);
}

//returns a deck filled with one of each type of card
Cards::Deck createDeck() {
    Cards::Deck deck;
    deck.cards.push_back(new Cards::Airlift());
    deck.cards.push_back(new Cards::Reinforcement());
    deck.cards.push_back(new Cards::Blockade());
    deck.cards.push_back(new Cards::Diplomacy());
    deck.cards.push_back(new Cards::Bomb());
    return deck;
}

//creates advance order with given parameters and prints stats before and after its execution
void testAdvance(Player *issuer, Map *map, Territory *source, Territory *target, int armies) {
    issuer->receivesCard = false;
    Advance advance = Advance(issuer, map, source, target, armies);
    cout << issuer->getName() << " issued: " << advance << endl;
    cout << "Stats before execution < " << source->name << " has " << source->numberOfArmies << " armies" << endl;
    cout << "                       < " << target->name << " has " << target->numberOfArmies << " armies"
                                                                                                " and is owned by "
         << target->owner->getName() << endl;
    cout << "Calling execute()------- ";
    advance.execute();
    cout << "Stats after execution  > " << source->name << " has " << source->numberOfArmies << " armies" << endl;
    cout << "                       > " << target->name << " has " << target->numberOfArmies << " armies"
                                                                                                " and is owned by "
         << target->owner->getName() << endl;

    //draw card at the end of turn if issuer conquered a territory while executing advance order
    if (issuer->receivesCard) {
        Cards::Card *card = createDeck().draw();
        issuer->hand->cards.push_back(card);
        cout << "                       > " << issuer->getName() << " receives " << card->getType() << " card " << endl;
    }
    cout << endl;
}

//creates airlift order with given parameters and prints stats before and after its execution
void testAirlift(Player *issuer, Territory *source, Territory *target, int armies) {
    Airlift airlift = Airlift(issuer, source, target, armies);
    cout << issuer->getName() << " issued: " << airlift << endl;
    cout << "Stats before execution < " << source->name << " has " << source->numberOfArmies << " armies" << endl;
    cout << "                       < " << target->name << " has " << target->numberOfArmies << " armies" << endl;
    cout << "Calling execute()------- ";
    airlift.execute();
    cout << "Stats after execution  > " << source->name << " has " << source->numberOfArmies << " armies" << endl;
    cout << "                       > " << target->name << " has " << target->numberOfArmies << " armies\n" << endl;
}

//creates deploy order with given parameters and prints stats before and after its execution
void testDeploy(Player *issuer, Territory *target, int armies) {
    Deploy deploy = Deploy(issuer, target, armies);
    cout << issuer->getName() << " issued: " << deploy << endl;
    cout << "Stats before execution > " << target->name << " has " << target->numberOfArmies << " armies" << endl;
    cout << "Calling execute()------- ";
    deploy.execute();
    cout << "Stats after execution  < " << target->name << " has " << target->numberOfArmies << " armies\n" << endl;
}

//driver for Part 4: Order execution implementation
void Orders::ordersDriver() {
    cout << "\nCreating map loader and loading smallsolar map..." << endl;
    MapLoader ml;
    ml.loadMap("../WarzoneMaps/solar/solar.map");
    cout << "*******************************************************************************************" << endl;

    cout << "\n\nCreating players named PlayerOne and PlayerTwo..." << endl;
    Player p1 = Player("PlayerOne");
    Player p2 = Player("PlayerTwo");

    cout << "\n\nAssigning territories..." << endl;
    Territory *southNeptune = ml.map->territories[0];
    Territory *northNeptune = ml.map->territories[3];
    Territory *eastPluto = ml.map->territories[4];
    Territory *westPluto = ml.map->territories[5];
    assignTerritory(ml.map->edges, eastPluto, &p1, 2);
    assignTerritory(ml.map->edges, southNeptune, &p1, 2);
    assignTerritory(ml.map->edges, westPluto, &p2, 2);

    cout << "\n\nTesting Deploy execution..." << endl;
    p1.reinforcementPool = 10; //preparing reinforcement pool for deploy order
    testDeploy(&p1, eastPluto, 7);//valid
    testDeploy(&p1, westPluto, 1);//invalid- target doesn't belong to issuer

    cout << "\nTesting Advance execution..." << endl;
    testAdvance(&p1, ml.map, eastPluto, westPluto, 4);//valid: conquers territory
    testAdvance(&p1, ml.map, eastPluto, southNeptune, 2);//invalid: target not adjacent

    cout << "\nTesting Airlift execution..." << endl;
    testAirlift(&p1, eastPluto, southNeptune, 3);//valid
    testAirlift(&p1, eastPluto, northNeptune, 2);//invalid-:target doesn't belong to issuer

    //-----------------------------------------------------------------------------
//    //giving player one the required cards to execute orders, displaying player one's hand, and displaying deck content
//    Cards::Card *card1 = new Cards::Bomb();
//    Cards::Card *card2 = new Cards::Blockade();
//    Cards::Card *card3 = new Cards::Diplomacy();
//    p1.hand->cards.push_back(card1);
//    p1.hand->cards.push_back(card2);
//    p1.hand->cards.push_back(card3);
//    p1.displayCards();
//    cout << "Deck size: " << Player::deck.cards.size() << endl;
//    cout << "Deck content: " << endl;
//    for (auto card: Player::deck.cards)
//        cout << "\t" << card->getType() << endl;


    //code block testing bomb execution
    {
        cout << "\nTesting Bomb execution..." << endl;
        cout << "-----------------------------" << endl;

        //invalid; bombing your own territory
        Bomb bomb = Bomb(&p1, westPluto, ml.map);
        cout << "Pluto-West number of armies: " << westPluto->numberOfArmies << endl;
        bomb.execute();
        cout << "Pluto-West number of armies: " << westPluto->numberOfArmies << endl << endl;

        //invalid; territory not adjacent to any of issuing player's territories
        westPluto->transferOwnership(&p2);
        eastPluto->transferOwnership(&p2);
        bomb = Bomb(&p1, westPluto, ml.map);
        cout << "Pluto-West number of armies: " << westPluto->numberOfArmies << endl;
        bomb.execute();
        cout << "Pluto-West number of armies: " << westPluto->numberOfArmies << endl << endl;

        //valid; bombing enemy adjacent territory
        eastPluto->transferOwnership(&p1);
        bomb = Bomb(&p1, westPluto, ml.map);
        cout << "Pluto-West number of armies: " << westPluto->numberOfArmies << endl;
        bomb.execute();
        cout << "Pluto-West number of armies: " << westPluto->numberOfArmies << endl << endl;
    }

    //code block testing blockade execution
    {
        cout << "Testing Blockade execution..." << endl;
        cout << "-----------------------------" << endl;

        //invalid; placing blockade on territory not owned by player
        cout << "Pluto-West number of armies: " << westPluto->numberOfArmies << endl;
        cout << "Pluto-West owner: " << westPluto->owner->getName() << endl;
        Blockade blockade = Blockade(&p1, westPluto);
        blockade.execute();
        cout << "Pluto-West number of armies: " << westPluto->numberOfArmies << endl;
        cout << "Pluto-West owner: " << westPluto->owner->getName() << endl << endl;

        //valid; placing blockade on territory owned by player
        cout << "Pluto-East number of armies: " << eastPluto->numberOfArmies << endl;
        cout << "Pluto-East owner: " << eastPluto->owner->getName() << endl;
        blockade = Blockade(&p1, eastPluto);
        blockade.execute();
        cout << "Pluto-East number of armies: " << eastPluto->numberOfArmies << endl;
        cout << "Pluto-East owner: " << eastPluto->owner->getName() << endl << endl;
    }

    //code block testing negotiate execution
    {
        cout << "Testing Negotiate execution..." << endl;
        cout << "-----------------------------" << endl;

        cout << "Player 1 cannotAttack size: " << p1.cannotAttack.size() << endl;
        cout << "Player 1 cannotAttack content: " << endl;
        for (auto s: p1.cannotAttack)
            cout << "  - " << s << endl;
        cout << "Player 2 cannotAttack size: " << p2.cannotAttack.size() << endl;
        cout << "Player 2 cannotAttack content: " << endl;
        for (auto s: p2.cannotAttack)
            cout << "  - " << s << endl;

        Negotiate negotiate = Negotiate(&p1, &p2);
        negotiate.execute();

        cout << "Player 1 cannotAttack size: " << p1.cannotAttack.size() << endl;
        cout << "Player 1 cannotAttack content: " << endl;
        for (auto s: p1.cannotAttack)
            cout << "  - " << s << endl;
        cout << "Player 2 cannotAttack size: " << p2.cannotAttack.size() << endl;
        cout << "Player 2 cannotAttack content: " << endl;
        for (auto s: p2.cannotAttack)
            cout << "  - " << s << endl;
        cout << endl;

        //attempting to bomb a player with negotiation in effect
        cout<<"Attempting to bomb a territory whose owner has a negotiation in effect: " << endl;
        Bomb bomb = Bomb(&p1, westPluto, ml.map);
        bomb.execute();
        cout << endl;

        //attempting to advance on a territory owned by a player with negotiation in effect
        cout<<"Attempting to advance on a territory whose owner has a negotiation in effect: " << endl;
        eastPluto->transferOwnership(&p1);
        testAdvance(&p1, ml.map, eastPluto, westPluto, 4);//valid: conquers territory
        cout << endl;
    }

//    //displaying player one's hand and deck content
//    p1.displayCards();
//    cout << "Deck size: " << Player::deck.cards.size() << endl;
//    cout << "Deck content: " << endl;
//    for (auto card: Player::deck.cards)
//        cout << "\t" << card->getType() << endl;
}