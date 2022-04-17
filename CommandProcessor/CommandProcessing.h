#ifndef COMP345RISKGAME_COMMANDPROCESSING_H
#define COMP345RISKGAME_COMMANDPROCESSING_H

#include <string>
#include <fstream>
#include <ostream>
#include <vector>
#include "../Logging/LoggingObserver.h"

using namespace std;

class GameEngine;

// stores a command taken from a file or console input along with its effect
class Command : public ILoggable, public Subject {
public:
    string command;
    string effect;

    Command();
    Command(string command);
    Command(const Command &command);
    ~Command();

    Command& operator=(const Command &c);
    friend ostream& operator<<(ostream &out, const Command &command);
    string stringToLog() const override;
    void saveEffect(string commandEffect);
};

// provides functionalities for commands such as reading them, storing them, and validating them based on states
class CommandProcessor : public ILoggable, public Subject {
public:
    string currentState;
    vector<Command *> commandList;
    CommandProcessor();
    [[maybe_unused]] CommandProcessor(const CommandProcessor &commandProcessor);
    virtual ~CommandProcessor();

    CommandProcessor& operator=(const CommandProcessor &commandProcessor);
    friend ostream& operator<<(ostream &out, const CommandProcessor &commandProcessor);

    string stringToLog() const;
    Command& getCommand();
    bool validate(const string& readCommandInput, const GameEngine &gameEngine);
private:
    virtual string readCommand();
    void saveCommand(Command &command);
};

// provides functionalities for reading from files
class FileLineReader {
public:
    FileLineReader();
    FileLineReader(string otherFileName);
    FileLineReader(const FileLineReader &flr);
    ~FileLineReader();

    FileLineReader &operator=(const FileLineReader &flr);
    friend ostream &operator<<(ostream &out, const CommandProcessor &flr);

    // reads a line from a file
    string readLineFromFile();

    // opens a file with the file name the member variable fileName
    void openFile();
    // closes a file with the file name the member variable fileName
    void closeFile();
    // returns a boolean value depending on if the next line is the end of file or not
    bool checkEOF();
    // check if a file is open
    bool isFileOpen() const;
    // returns the name of the file
    string getFileName() const;

private:
    ifstream inputFileStream; // input file stream for file reading
    string fileName; // name of file
    bool stateOfFile; // the state of file(open(true) or close(false))
};

//must abide to Adapter design pattern
class FileCommandProcessorAdapter : public CommandProcessor
{
public:
    FileCommandProcessorAdapter();
    FileCommandProcessorAdapter(string fileName);
    FileCommandProcessorAdapter(const FileCommandProcessorAdapter &fcpa);
    ~FileCommandProcessorAdapter();

    FileCommandProcessorAdapter &operator=(const FileCommandProcessorAdapter &fcpa);
    friend ostream &operator<<(ostream &out, const FileCommandProcessorAdapter &fcpa);

    // method to open a specific file with the parameter fileName
    void openSpecificFLRFile(string fileName);
    // method to close FileLineReader file
    void closeFLRFile();
    // checks if the FileLineReader file is open
    bool isFLRFileOpen();
    // checks if there are more commands in the FineLineReader file
    bool moreCommands();
    // method get the opened file name
    string getFLRFileName() const;

private:
    // overridden readCommand() method from CommandProcessor that gets a line from the file and returns it as a string
    string readCommand();

    // string mapFile;
    FileLineReader *flr; // object for reading files
};

#endif //COMP345RISKGAME_COMMANDPROCESSING_H
