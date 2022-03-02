#include <sstream>
#include <utility>
#include "CommandProcessing.h"
#include "../GameEngine/GameEngine.h"

string CommandProcessor::getCommand() {
    cout << "Command: ";
    string read = readCommand();
    saveCommand(read);
    return read;
}

string CommandProcessor::readCommand() {
    string readCommandInput;
    getline(cin, readCommandInput);
    return readCommandInput;
}

void CommandProcessor::saveCommand(string readCommandInput){
    commandList.push_back(new Command(std::move(readCommandInput)));
}

// validate (1) user command (2) command is used in the correct state
bool CommandProcessor::validate(string readCommandInput, const GameEngine &gameEngine) {
    vector<string>inputWords;
    currentState = gameEngine.getState();
    string userInputCommand, userInputSecondWord;
    istringstream parse(readCommandInput);
    while (parse>>readCommandInput){
        inputWords.push_back(readCommandInput);
    }
    int vectorSize= inputWords.size();
    userInputCommand = inputWords.at(0);

    if (vectorSize == 2) {
        if ((userInputCommand == "loadmap" && (currentState == "start" || currentState == "maploaded")) ||
            (userInputCommand == "addplayer" && (currentState == "mapvalidated" || currentState == "playersadded"))) {
            return true;
        }
    }
    else if (vectorSize == 1) {
        if ((userInputCommand == "validatemap" && currentState == "maploaded") || (userInputCommand == "gamestart" && currentState == "playersadded")
            || (userInputCommand == "replay" && currentState == "win") || (userInputCommand == "quit" && currentState == "win")) {
            return true;
        }
    }
    return false;
}

CommandProcessor::~CommandProcessor() {
   cout << "Invoking delete constructor or CommandProcessor" << endl;
    commandList.clear();
}
Command::Command(string commands) {
    command=std::move(commands);
    effect =" ";
}

void Command::saveEffect(string commandEffect, const CommandProcessor &processor) {
    effect = commandEffect;
    static int i=0;
    processor.commandList.at(i)->effect.assign(effect);
    i++;
}

