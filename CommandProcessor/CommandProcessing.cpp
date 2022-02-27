//
// Created by Judy Lombardo on 2022-02-20.
//
#include <sstream>
#include "CommandProcessing.h"


string mapName[] ={"smallsolar","canada"}; //list of maps


string CommandProcessor::getCommand() {
    string read= readCommand();
    saveCommand(read);
    return read;
}
string CommandProcessor::readCommand(){
    string readCommandInput;
    getline(cin,readCommandInput);
    return readCommandInput;
}
void CommandProcessor::saveCommand(string readCommandInput){
    //TODO create a command objects to put in a list
    //create Command object
commandList.emplace_back(new Command(readCommandInput));

}

//validate (1) user command (2) command is used in the correct state
bool CommandProcessor::validate(string readCommandInput) {
    currentState = GameEngine::getState();

    //to parse the getline input
    string userInputCommand,userInputSecondWord;
    istringstream parse(readCommandInput);
    while (parse>>readCommandInput){
        inputWords.push_back(readCommandInput);
    }
    int vectorSize= inputWords.size();

    //size =2 for user command input
    if(vectorSize==2) {
        cout<<" I typed 2 inputWords"<<" array size: "<<vectorSize<<" current state: "<<currentState<<endl;
        userInputCommand=inputWords.at(0);
        userInputSecondWord =inputWords.at(1); //can either be a map file name or a player name
        int sizeOfMapNameArray = size(mapName); //using map list on line 8
        bool validMapName=false;

        //checks if the second input matches a map name in the mapName array
        for(int i=0;i<sizeOfMapNameArray;i++){
            if(userInputSecondWord == mapName[i])
                validMapName =true;
        }
        if (userInputCommand == "loadmap" &&validMapName && currentState == "start" || currentState == "maploaded") {
            cout << "user command:" << userInputCommand <<" map name: "<<userInputSecondWord<< " current state:" << currentState << endl;
            cout<<"the command "<<userInputCommand<< " is valid in the current game state "<< currentState<<endl;
            return true;
        } else if (userInputCommand == "addplayer" && currentState == "mapvalidated" ||
                   currentState == "playersadded") {
            cout << "user command:" << userInputCommand <<" player name: "<<userInputSecondWord<< " current state:" << currentState << endl;
            return true;
        }
    }
    //for user command input
    else if (vectorSize ==1) {
        userInputCommand=inputWords.at(0);
        cout<<"I typed 1 inputWords. User input word: "<<userInputCommand<<" "<<currentState<<endl;
        //size =1
        if (userInputCommand == "validatemap" && currentState == "maploaded") {
            cout << "user command:" << userInputCommand << " current state:" << currentState << endl;
            return true;
        } else if (userInputCommand == "gamestart" && currentState == "playersadded") {
            cout << "user command:" << userInputCommand << " current state:" << currentState << endl;
            return true;
        } else if (userInputCommand == "replay" && currentState == "win") {
            cout << "user command:" << userInputCommand << " current state:" << currentState << endl;
            return true;
        } else if (userInputCommand == "quit" && currentState == "win") {
            cout << "user command:" << userInputCommand << " current state:" << currentState << endl;
            return true;
        }else{
            cout<<"none of the above"<<endl;
            return false;
        }
    }
    //size >2 or else for user command input
    else{
        cout<<"I typed anything else inputWords"<<endl;
        cout<<"the command"<<userInputCommand<< " is not valid in the current game state: "<< currentState<<endl;
    }

    return false;
}

CommandProcessor::~CommandProcessor() {
    cout<<"Invoking delete constructor or CommandProcessor"<<endl;

}
Command::Command(string commands) {
command=commands;
effect =" ";
}

void Command::setCommand(string commands) {
command =commands;
}

void Command::setEffect(string effects) {
effect = effects;

}

void Command::saveEffect(string commandEffect) {
    //TODO add save effect to previous created object
    Command::setEffect(commandEffect);

}

