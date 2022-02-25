//
// Created by Judy Lombardo on 2022-02-20.
//
#include <sstream>
#include "CommandProcessing.h"


string mapName[] ={"smallsolar","canada"};


void CommandProcessor::availableCommandList() {
    cout<<"List of available commands:\nloadmap <filename>\nvalidatemap\naddplayer <playername>\ngamestart\nreplay\nwin"<<endl;
}

void CommandProcessor::getCommand() {
}

//validate (1) user command (2) command is used in the correct state
//TODO: If the command is not valid, a corresponding error message should be saved in the effect of the command.
bool CommandProcessor::validate(string userInput) {
    currentState = GameEngine::getState();

    //to parse the getline input
    vector<string>words;
    string userInputCommand,userInputSecondWord;
    istringstream parse(userInput);
    while (parse>>userInput){
        words.push_back(userInput);
    }
    int vectorSize= words.size();

    //size =2 for user command input
    if(vectorSize==2) {
        cout<<" I typed 2 words"<<" array size: "<<vectorSize<<" current state: "<<currentState<<endl;
        userInputCommand=words.at(0);
        userInputSecondWord =words.at(1);
        int sizeOfMapNameArray = size(mapName);
        bool validMapName=false;

        //checks if the second input matches a map name in the mapName array
        for(int i=0;i<sizeOfMapNameArray;i++){
            if(userInputSecondWord == mapName[i])
                validMapName =true;
        }
        if (userInputCommand == "loadmap" && validMapName && currentState == "start" || currentState == "maploaded") {
            cout << "user command:" << userInputCommand <<" map name: "<<userInputSecondWord<< " current state:" << currentState << endl;
            cout<<"the command "<<userInputCommand<< " is valid in the current game state "<< currentState<<endl;
            return true;
        } else if (userInputCommand == "addplayer" && currentState == "mapvalidated" ||
                   currentState == "playersadded") {
            string playerName =userInputSecondWord;
            cout << "user command:" << userInputCommand <<" player name: "<<playerName<< " current state:" << currentState << endl;
            return true;
        }
    }
    //for user command input
    else if (vectorSize ==1) {
        userInputCommand=words.at(0);
        cout<<"I typed 1 words. User input word: "<<userInputCommand<<" "<<currentState<<endl;
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
        cout<<"I typed anything else words"<<endl;
        cout<<"the command"<<userInputCommand<< " is not valid in the current game state: "<< currentState<<endl;
    }

    return false;
}

CommandProcessor::~CommandProcessor() {
    cout<<"Invoking delete constructor or CommandProcessor"<<endl;

}

