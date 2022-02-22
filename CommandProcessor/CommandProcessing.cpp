//
// Created by Judy Lombardo on 2022-02-20.
//
#include<fstream>
#include "CommandProcessing.h"

// =================Command=================
// default
Command::Command() {
    command = "No name";
    effect = "No effect";
}

// command constructor with the command name
Command::Command(string commandName){
    command = commandName;
    effect = "No effect";
}

// copy constructor
Command::Command(const Command &otherCommand){
    this->command = otherCommand.command;
    this->effect = otherCommand.effect;
}

// equal operator
Command &Command::operator=(const Command &otherCommand){
    if(this == &otherCommand) {return *this;}
    this->command = otherCommand.command;
    this->effect = otherCommand.effect;
}

// ostream operator
//ostream& operator<<(ostream& out, const Command &command){}

// method to save the effect of the command
void Command::saveEffect(string e) {
    effect = e;
}

// accessor method for command name
string Command::getCommand() {
    return command;
}

// accessor method for command effect
string Command::getEffect() {
    return effect;
}

// =================FileLineReader=================
string FileLineReader::readFromFile(string fileLocation) {
    string command;
    ifstream input(fileLocation);
    input >> command;
    return command;
}

// =================FileCommandProcessorAdapter=================
// default constructor
FileCommandProcessorAdapter::FileCommandProcessorAdapter() : flr(), listOfCommands()  {}

// copy constructor
FileCommandProcessorAdapter::FileCommandProcessorAdapter(const FileCommandProcessorAdapter &fcpa) : flr(), listOfCommands(){
    for(Command* command: fcpa.listOfCommands){
        this->listOfCommands.emplace_back(command);
    }
}

// destructor
FileCommandProcessorAdapter::~FileCommandProcessorAdapter(){
    delete flr;
    listOfCommands.erase(listOfCommands.begin(),listOfCommands.end());
}

// equal operator
FileCommandProcessorAdapter &FileCommandProcessorAdapter::operator=(const FileCommandProcessorAdapter &fcpa){
    if(this == &fcpa) return *this;

    this->flr = new FileLineReader();

    this->listOfCommands.erase(this->listOfCommands.begin(), this->listOfCommands.end());
    for(Command* command: fcpa.listOfCommands){
        this->listOfCommands.emplace_back(command);
    }

    return *this;
}

// ostream operator
//ostream &operator<<(ostream &out, const FileCommandProcessorAdapter &fcpa){
//
//}

// validates command based on the current state of the game engine
bool FileCommandProcessorAdapter::validate(Command& command){
    string state;
    // check the state of the game engine then validate the command within the state
    if(state.compare("start")){
        if(!command.getCommand().compare(0, 6, "loadmap")){return false;}
    }
    else if(state.compare("map loaded")){
        if(!command.getCommand().compare("loadmap") || !command.getCommand().compare("validatemap")){return false;}
    }
    else if(state.compare("map validated")){
        if(!command.getCommand().compare(0, 8, "addplayer")){return false;}
    }
    else if(state.compare("players added")){
        if(!command.getCommand().compare(0, 8, "addplayer") || !command.getCommand().compare("gamestart")){return false;}
    }
    else if(state.compare("win")){
        if(!command.getCommand().compare("replay") || !command.getCommand().compare("quit")){return false;}
    }
    else{return true;}
}

// reads a command from a file located from the file location
void FileCommandProcessorAdapter::readCommand(string fileLocation) {
    string state;
    Command* command = new Command(flr->readFromFile(fileLocation));
    if(!validate(*command)){
        command->saveEffect("Invalid command in state " + state);
    }
    saveCommand(command);
    delete command;
}

// saves the command to the list of commands
void FileCommandProcessorAdapter::saveCommand(Command &command) {
    listOfCommands.emplace_back(command);
}