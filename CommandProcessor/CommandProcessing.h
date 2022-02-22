//
// Created by Judy Lombardo on 2022-02-20.
//

#ifndef COMP345RISKGAME_COMMANDPROCESSING_H
#define COMP345RISKGAME_COMMANDPROCESSING_H
#include "../GameEngine/GameEngine.h"
#include <string>
#include <ostream>
using namespace std;

// =================Command=================
// class for command objects
class Command{
public:
    Command(); // default constructor
    Command(string commandName); // constructor to create a command object with a name
    Command(const Command &command); // copy constructor
    ~Command(); // destructor

    Command& operator=(const Command &command); // equal operator
    friend ostream& operator<<(ostream &out, const Command &command); // ostream operator

    void saveEffect(string);
    string getCommand();
    string getEffect();
private:
    string command;
    string effect;
};

// =================FileLineReader=================
// class for reading a file
class FileLineReader{
public:
    FileLineReader() = default; // default constructor
    FileLineReader(const FileLineReader &flr) = default; // copy constructor
    ~FileLineReader() = default; // destructor

    string readFromFile(string fileLocation);
};

// =================CommandProcessor=================
class CommandProcessor{
public:
    CommandProcessor();
    CommandProcessor(const CommandProcessor &commandProcessor);
    ~CommandProcessor();

    CommandProcessor& operator=(const CommandProcessor &commandProcessor);
    friend ostream& operator<<(ostream &out, const CommandProcessor &commandProcessor);

    void getCommand();
    void saveEffect();
    bool validate();
    void saveCommand();

private:
    void readCommand();

};

// =================FileCommandProcessorAdapter=================
//must abide to Adpater design pattern
class FileCommandProcessorAdapter: CommandProcessor{
public:
    FileCommandProcessorAdapter(); // default constructor
    FileCommandProcessorAdapter(const FileCommandProcessorAdapter &fcpa); // copy constructor
    ~FileCommandProcessorAdapter(); // destructor

    FileCommandProcessorAdapter& operator=(const FileCommandProcessorAdapter &fcpa); // equal operator
    friend ostream& operator<<(ostream &out, const FileCommandProcessorAdapter &fcpa); // ostream operator

    void getCommand();
    bool validate(Command &command);
    void saveCommand(Command &command);

private:
    list<Command*> listOfCommands;
    FileLineReader *flr;

    void readCommand(string fileLocation);
};


#endif //COMP345RISKGAME_COMMANDPROCESSING_H
