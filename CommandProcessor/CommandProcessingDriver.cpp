#include <iterator>
#include <iostream>
#include "CommandProcessing.h"
#include "../GameEngine/GameEngine.h"

void CommandProcessor::commandProcessorDriver() {

    GameEngine *gameEngine = new GameEngine;
    CommandProcessor *commandProcessor;
    Command *command = new Command;
    string userInput;

    cout << "Choose way of input(-console, -file <filename>): ";
    getline(cin, userInput);

    if(userInput == "-console"){
        commandProcessor = new CommandProcessor();
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
    } else if(userInput.substr(0, 5) == "-file"){
        string fileName;
        vector<string> wordsFromInput;
        string temp = userInput;
        string delimiter = " ";
        int pos;

        // while loop to parse string based on the delimiter and put the words in a vector
        while ((pos = temp.find(delimiter)) != std::string::npos)
        {
            string wordFromInput = temp.substr(0, pos);
            wordsFromInput.emplace_back(wordFromInput);
            temp.erase(0, pos + delimiter.length());
        }
        wordsFromInput.emplace_back(temp); // put the last word of the line in a vector

        // more than 2 words is invalid
        if (wordsFromInput.size() > 2)
        {
            cout << "Invalid -file command." << endl;
            delete gameEngine;
            delete commandProcessor;
            delete command;
            return;
        }
            // if words is only 1, because -file needs a file name
        else if (wordsFromInput.size() == 1)
        {
            cout << "No file name." << endl;
            delete gameEngine;
            delete commandProcessor;
            delete command;
            return;
        }
            // if file name does not either or both angled brackets
        else if (wordsFromInput[1].find("<") == std::string::npos || wordsFromInput[1].find(">") == std::string::npos)
        {
            cout << "Invalid file name." << endl;
            delete gameEngine;
            delete commandProcessor;
            delete command;
            return;
        }
            // return the file name without the angled brackets
        else
        {
            // string returnString;
            fileName = "..\\FileProcessorText\\" + wordsFromInput[1].substr(wordsFromInput[1].find("<") + 1, wordsFromInput[1].find(">") - wordsFromInput[1].find("<") - 1);
        }

        commandProcessor = new FileCommandProcessorAdapter(fileName);

        gameEngine->playersAddedStateChange();
        string state = gameEngine->getState();

        bool validate;

        if(dynamic_cast<FileCommandProcessorAdapter*>(commandProcessor)->moreCommands()) {
            string saveEffect;
            while (dynamic_cast<FileCommandProcessorAdapter *>(commandProcessor)->moreCommands()) {
                string commandName = commandProcessor->getCommand();
                validate = commandProcessor->validate(commandName, *gameEngine);

                if (validate) {
                    cout << "The command " << commandName << " is valid at state: "
                         << state << endl;
                    if (commandName.substr(0, 9) == "addplayer"){
                        saveEffect = "player is added";
                    }
                    else {
                        saveEffect = "game is started";
                    }
                } else {
                    cout << "The command " << commandName << " is not valid at state: "
                         << state << endl;
                    saveEffect = "invalid command in state " + state;
                }
                command->saveEffect(saveEffect, *commandProcessor);
            }
        } else {
            cout << "The file is empty." << endl;
        }

        for (int i = 0; i < commandProcessor->commandList.size(); i++) {
            cout << "command: " << commandProcessor->commandList.at(i)->command << " ";
            cout << " effect: " << commandProcessor->commandList.at(i)->effect << endl;
        }
    }

    delete gameEngine;
    delete commandProcessor;
    delete command;
}