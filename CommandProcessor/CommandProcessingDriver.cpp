#include <iterator>
#include <iostream>
#include "CommandProcessing.h"
#include "../GameEngine/GameEngine.h"

void CommandProcessor::commandProcessorDriver() {

    GameEngine *gameEngine = new GameEngine();
    CommandProcessor *commandProcessor = nullptr;

    string userInput;

    cout << "-INPUTTING COMMANDS-" << endl;
    cout << "Choose way of inputting commands (-console, -file <filename>): ";
    getline(cin, userInput);

    if (userInput == "-console") {
        commandProcessor = new CommandProcessor();
        Command *ml = new Command("maploaded");
        commandProcessor->saveCommand(*ml);
        ml->saveEffect("map is loaded");
        Command *mv = new Command("mapvalidated");
        commandProcessor->saveCommand(*mv);
        mv->saveEffect("map is validated");

        gameEngine->mapValidatedStateChange();
        string state = gameEngine->getState();

        cout << "Possible commands in state " << state << ": " << endl;
        cout << "addplayer <playername>" << endl;

        Command *command = &commandProcessor->getCommand();
        bool validate = commandProcessor->validate(command->command, *gameEngine);
        if (validate == 0) {
            cout << "The command " << command->command << " is not valid at state " << state << endl;
            command->saveEffect("Invalid command in state " + state);
        }
        else {
            cout << "The command " << command->command << "  is valid at state " << state << endl;
            command->saveEffect("player added");
        }

        cout << *commandProcessor << endl;
    } else if(userInput.substr(0, 5) == "-file") {
        string fileName;
        vector<string> wordsFromInput;
        string temp = userInput;
        string delimiter = " ";
        int pos;

        // while loop to parse string based on the delimiter and put the words in a vector
        while ((pos = temp.find(delimiter)) != std::string::npos) {
            string wordFromInput = temp.substr(0, pos);
            wordsFromInput.emplace_back(wordFromInput);
            temp.erase(0, pos + delimiter.length());
        }
        wordsFromInput.emplace_back(temp); // put the last word of the line in a vector

        // more than 2 words is invalid
        if (wordsFromInput.size() > 2) {
            cout << "Invalid -file command." << endl;
            delete gameEngine;
            return;
        }
            // if words is only 1, because -file needs a file name
        else if (wordsFromInput.size() == 1) {
            cout << "No file name." << endl;
            delete gameEngine;
            return;
        }
            // return the file name without the angle brackets
        else {
            // string returnString;
            fileName = "../FileProcessorText/" + wordsFromInput[1] + ".txt";
        }

        commandProcessor = new FileCommandProcessorAdapter(fileName);

       if (!dynamic_cast<FileCommandProcessorAdapter*>(commandProcessor)->isFLRFileOpen()) {
            cout << "Invalid file name." << endl;
            delete gameEngine;
            delete commandProcessor;
            return;
        }

        bool validate;

        if(dynamic_cast<FileCommandProcessorAdapter*>(commandProcessor)->moreCommands()) {
            gameEngine->playersAddedStateChange();
            string state = gameEngine->getState();
            string saveEffect;
            while (dynamic_cast<FileCommandProcessorAdapter *>(commandProcessor)->moreCommands()) {
                Command *c = &commandProcessor->getCommand();
                validate = commandProcessor->validate(c->command, *gameEngine);

                if (validate) {
                    cout << "The command " << c->command << " is valid at state "
                         << state << endl;
                    if (c->command.substr(0, 9) == "addplayer"){
                        saveEffect = "player is added";
                    }
                    else {
                        saveEffect = "game is started";
                    }
                } else {
                    cout << "The command " << c->command << " is not valid at state " << state << endl;
                    saveEffect = "Invalid command in state " + state;
                }
                c->saveEffect(saveEffect);
            }
            cout << *commandProcessor << endl;
        } else {
            cout << "The file is empty." << endl;
        }
    }

    delete gameEngine;
    delete commandProcessor;
}