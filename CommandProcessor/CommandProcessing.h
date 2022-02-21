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

//must abide to Adpater design pattern
class FileCommandProcessorAdapter: CommandProcessor{

};

#endif //COMP345RISKGAME_COMMANDPROCESSING_H
