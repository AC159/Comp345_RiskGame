//
// Created by Judy Lombardo on 2022-02-21.
//
#include <sstream>
#include <iterator>
#include <iostream>
#include "CommandProcessing.h"

int main() {


//    vector<string> inputWords;
//    string str;
//
//    cout<<"Enter command:";
//    getline(cin,str);
//    istringstream parse(str);
//    while (parse>>str){
//        inputWords.push_back(str);
//    }
//    cout<< inputWords[0];

string str,str1,str2;
bool validate,validate1;
GameEngine *gameEngine = new GameEngine();
CommandProcessor *cp = new CommandProcessor();
//gameEngine->welcomeMessage();
//    str="loadmap canada";
//    validate = cp->validate(str);
    gameEngine->mapValidatedStateChange();
    str="addplayer bob";
    validate = cp->validate(str);

//    str1="start canada";
//    validate1 = cp->validate(str1);
//gameEngine->mapLoadedStateChange();
////    cout<<"Enter command:";
////    getline(cin,str);
//str2="validatedmap";
//    validate = cp->validate(str2);
cout<<validate;
delete gameEngine;
delete cp;












    return 0;
}