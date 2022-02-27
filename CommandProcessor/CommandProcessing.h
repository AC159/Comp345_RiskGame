//
// Created by Judy Lombardo on 2022-02-20.
//

#ifndef COMP345RISKGAME_COMMANDPROCESSING_H
#define COMP345RISKGAME_COMMANDPROCESSING_H
#include "../GameEngine/GameEngine.h"
#include <string>
#include <ostream>
#include <vector>

using namespace std;

class Command{
public:
    string command;
    string effect;
    Command();
    Command(string command);
    Command(const Command &command);
    ~Command();

    Command& operator=(const Command &command);
    friend ostream& operator<<(ostream &out, const Command &command);

    void setCommand(string command);
    void setEffect(string effect);
    void saveEffect(string commandEffect);

};
class CommandProcessor{
public:
    string currentState;
    vector<string>inputWords;
    //collection of commands
    vector<Command *> commandList;
    CommandProcessor()=default;
    CommandProcessor(const CommandProcessor &commandProcessor);
    ~CommandProcessor();

    CommandProcessor& operator=(const CommandProcessor &commandProcessor);
    friend ostream& operator<<(ostream &out, const CommandProcessor &commandProcessor);


    string getCommand();
    bool validate(string userInput);




private:
    string readCommand();
    void saveCommand(string readCommandInput);

};


//must abide to Adpater design pattern
class FileCommandProcessorAdapter: CommandProcessor{

};

#endif //COMP345RISKGAME_COMMANDPROCESSING_H
