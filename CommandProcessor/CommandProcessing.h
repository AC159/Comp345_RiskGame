#ifndef COMP345RISKGAME_COMMANDPROCESSING_H
#define COMP345RISKGAME_COMMANDPROCESSING_H

#include <string>
#include <ostream>
#include <vector>

using namespace std;

class GameEngine;
class CommandProcessor;

class Command {
public:
    //should this be private?
    string command;
    string effect;
    //
    Command()=default;
    Command(string command);
    Command(const Command &command);
    ~Command();

    Command& operator=(const Command &command);
    friend ostream& operator<<(ostream &out, const Command &command);

    void saveEffect(string commandEffect, const CommandProcessor &processor);

};

class CommandProcessor {
public:
    string currentState;
    vector<Command *> commandList;
    CommandProcessor()=default;
    CommandProcessor(const CommandProcessor &commandProcessor);
    ~CommandProcessor();

    CommandProcessor& operator=(const CommandProcessor &commandProcessor);
    friend ostream& operator<<(ostream &out, const CommandProcessor &commandProcessor);

    string getCommand();
    bool validate(string userInput, const GameEngine &gameEngine);
    void saveCommand(string readCommandInput);
    static void commandProcessorDriver();
private:
    string readCommand();
    //void saveCommand(string readCommandInput);

};


//must abide to Adapter design pattern
class FileCommandProcessorAdapter: CommandProcessor{

};

#endif //COMP345RISKGAME_COMMANDPROCESSING_H
