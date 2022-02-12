#include <iostream>
#include "GameEngine.h"
#include "../Map"

using namespace std;

int main(int argc, char *argv[]) {

    GameEngine::Start start{};
    start.welcomeMessage();
    if (start.validateCommand()) {
        GameEngine::MapLoaded map1{};
        map1.chooseMapToLoad();
        map1.validateCommand();
        //validate map method here
        GameEngine::MapValidated mapValidated;
        mapValidated.validateCommand();
        //add players into map
        //assign countries here
        loop:
        GameEngine::AssignReinforcement assignReinforcement;
        assignReinforcement.assignReinforcement();
        assignReinforcement.validateCommand();
        //create orders/add orders here
        GameEngine::IssueOrders issueOrder;
        issueOrder.issueOrdersStateChange();
        issueOrder.validateCommand();
        //execute orders here
        GameEngine::ExecuteOrders executeOrders;
        executeOrders.executeOrdersStateChange();
        if (executeOrders.validateCommand() == 1)
            goto loop;
        GameEngine::Win win;
        win.winStateChange();
    };


    return 0;
}
