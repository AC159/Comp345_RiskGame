//
// Created by Judy Lombardo on 2022-02-20.
//

#ifndef COMP345RISKGAME_COMMANDPROCESSING_H
#define COMP345RISKGAME_COMMANDPROCESSING_H
#include "../GameEngine/GameEngine.h"
#include <string>
#include <ostream>
using namespace std;

class CommandProcessor{
public:
    string currentState;
    CommandProcessor()=default;
    CommandProcessor(const CommandProcessor &commandProcessor);
    ~CommandProcessor();

    CommandProcessor& operator=(const CommandProcessor &commandProcessor);
    friend ostream& operator<<(ostream &out, const CommandProcessor &commandProcessor);


    static void availableCommandList();
    void getCommand();
    bool validate(string userInput);
    void saveCommand();

private:
    void readCommand();

};
class Command{
public:
    string command;
    string effect;
    Command()=default;
    Command(string command,string effect);
    Command(const Command &command);
    ~Command();

    Command& operator=(const Command &command);
    friend ostream& operator<<(ostream &out, const Command &command);
    void saveEffect();
private:
    //collection of commands
    vector<pair<Command,string>>commands;

};

//must abide to Adpater design pattern
class FileCommandProcessorAdapter: CommandProcessor{

};

#endif //COMP345RISKGAME_COMMANDPROCESSING_H
