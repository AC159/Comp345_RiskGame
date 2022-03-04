#include <iterator>
#include <iostream>
#include "CommandProcessing.h"
#include "../GameEngine/GameEngine.h"

void CommandProcessor::commandProcessorDriver() {

    GameEngine *gameEngine = new GameEngine;
    CommandProcessor *commandProcessor = new CommandProcessor;
    Command *command = new Command;

    commandProcessor->saveCommand("maploaded");
    command->saveEffect("map is loaded", *commandProcessor);
    commandProcessor->saveCommand("mapvalidated");
    command->saveEffect("map is validated", *commandProcessor);

    gameEngine->mapValidatedStateChange();
    string state = gameEngine->getState();
    cout<< state;
    bool validate = commandProcessor->validate(commandProcessor->getCommand(), *gameEngine);
    cout<<state << validate;
    if (validate ==0)
        cout<<"The command is not valid"<<"command: "<< commandProcessor->commandList.at(0)->command<<"state: "<<state<<endl;
    else
        cout<<"The command is valid"<<commandProcessor->commandList.at(0)->command<<"state: "<<state<<endl;

    for (int i=0;i<commandProcessor->commandList.size();i++){
        cout<<"command: "<<commandProcessor->commandList.at(i)->command;
        cout<<" effect:  "<<commandProcessor->commandList.at(i)->effect<<endl;
    }






    delete gameEngine;
    delete commandProcessor;



}