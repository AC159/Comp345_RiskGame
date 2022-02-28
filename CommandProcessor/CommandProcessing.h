//
// Created by Judy Lombardo on 2022-02-20.
//

#ifndef COMP345RISKGAME_COMMANDPROCESSING_H
#define COMP345RISKGAME_COMMANDPROCESSING_H
#include "../GameEngine/GameEngine.h"
#include <string>
#include <ostream>
#include <vector>
#include <fstream>

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

class FileLineReader{
    public:

        FileLineReader() = default;
        FileLineReader(string otherFileName);
        FileLineReader(const FileLineReader &flr);
        ~FileLineReader();

        FileLineReader& operator=(const FileLineReader &flr);
        friend ostream& operator<<(ostream &out, const CommandProcessor &flr);

        string readLineFromFile();

        bool checkEOF();
        string getFileName() const;

    private:
        ifstream inputFileStream;
        bool fileEOF;
        string fileName;
};

//must abide to Adpater design pattern
class FileCommandProcessorAdapter: CommandProcessor{
    public:
        FileCommandProcessorAdapter() = default;
        // FileCommandProcessorAdapter(string fileName);
        FileCommandProcessorAdapter(const FileCommandProcessorAdapter &fcpa);
        ~FileCommandProcessorAdapter();

        FileCommandProcessorAdapter& operator=(const FileCommandProcessorAdapter &fcpa);
        friend ostream& operator<<(ostream &out, const FileCommandProcessorAdapter &fcpa);

        void openFile(string fileName);
        bool moreCommands();
        string getFileName() const;

    private:
        void closeFile();
        string readCommand();
        FileLineReader *flr;
};

#endif //COMP345RISKGAME_COMMANDPROCESSING_H
