#ifndef COMP345RISKGAME_COMMANDPROCESSING_H
#define COMP345RISKGAME_COMMANDPROCESSING_H

#include <string>
#include <fstream>
#include <ostream>
#include <vector>

using namespace std;

class GameEngine;

class Command {
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

class CommandProcessor {
public:
    string currentState;
    vector<Command *> commandList;
    CommandProcessor()=default;
    CommandProcessor(const CommandProcessor &commandProcessor);
    virtual ~CommandProcessor();

    CommandProcessor& operator=(const CommandProcessor &commandProcessor);
    friend ostream& operator<<(ostream &out, const CommandProcessor &commandProcessor);

    string getCommand();
    bool validate(string userInput, const GameEngine &gameEngine);

private:
    virtual string readCommand();
    void saveCommand(string readCommandInput);

};

class FileLineReader{
    public:

        FileLineReader();
        FileLineReader(string otherFileName);
        FileLineReader(const FileLineReader &flr);
        ~FileLineReader();

        FileLineReader& operator=(const FileLineReader &flr);
        friend ostream& operator<<(ostream &out, const CommandProcessor &flr);

        string readLineFromFile();

        void openFile();
        void closeFile();
        bool checkEOF();
        bool isFileOpen();
        string getFileName() const;

    private:
        ifstream inputFileStream;
        string fileName;
};

//must abide to Adpater design pattern
class FileCommandProcessorAdapter: public CommandProcessor{
    public:
        FileCommandProcessorAdapter();
        FileCommandProcessorAdapter(string fileName);
        FileCommandProcessorAdapter(const FileCommandProcessorAdapter &fcpa);
        ~FileCommandProcessorAdapter();

        FileCommandProcessorAdapter& operator=(const FileCommandProcessorAdapter &fcpa);
        friend ostream& operator<<(ostream &out, const FileCommandProcessorAdapter &fcpa);

        void openSpecificFLRFile(string fileName);
        bool isFLRFileOpen();
        bool moreCommands();
        string getFLRFileName() const;

    private:
        void closeFLRFile();
        string readCommand();

        string mapFile;
        FileLineReader *flr;
};

#endif //COMP345RISKGAME_COMMANDPROCESSING_H
