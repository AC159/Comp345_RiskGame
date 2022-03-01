#include <iterator>
#include <iostream>
#include "CommandProcessing.h"
#include "../GameEngine/GameEngine.h"

string askUser()
{
    string input;

    cout << "Choose way of command inpu(-console, -file <filename>): ";
    getline(cin, input);

    return input;
}
string checkFileName(string userInput)
{
    vector<string> wordsFromInput;
    string temp = userInput;
    string delimiter1 = " ";
    string delimiter2 = "\n";
    int pos = 0;

    while ((pos = temp.find(delimiter1)) != std::string::npos)
    {
        string wordFromInput = temp.substr(0, pos);
        wordsFromInput.emplace_back(wordFromInput);
        temp.erase(0, pos + delimiter1.length());
    }
    wordsFromInput.emplace_back(temp);

    if (wordsFromInput.size() > 2)
    {
        return "";
    }
    else if (wordsFromInput.size() == 1)
    {
        return "";
    }
    else if (wordsFromInput[1].find("<") == std::string::npos || wordsFromInput[1].find(">") == std::string::npos)
    {
        return "";
    }
    else
    {
        return wordsFromInput[1].substr(wordsFromInput[1].find("<") + 1, wordsFromInput[1].find(">") - wordsFromInput[1].find("<") - 1);
    }
}

int main()
{

    string userInput, fileName;
    GameEngine *gameEngine = new GameEngine();
    CommandProcessor *cp[2];
    cp[0] = new CommandProcessor();
    cp[1] = new FileCommandProcessorAdapter();

    userInput = askUser();
    if (userInput == "-console")
    {
        cout << "console" << endl;
    }
    else if (userInput.substr(0, userInput.find(" ")) == "-file")
    {
        fileName = checkFileName(userInput);
        if (fileName.empty()){
            cout << "Invalid file name." << endl;
        }
        else if (!fileName.empty()){
            dynamic_cast<FileCommandProcessorAdapter*>(cp[1])->openSpecificFLRFile(fileName);
            while(dynamic_cast<FileCommandProcessorAdapter*>(cp[1])->moreCommands()){
                cout << cp[1]->getCommand() << endl;
            }
        }
    }
    else {
        cout << "Invalid input." << endl;
    }

    delete gameEngine;
    delete cp[0];
    delete cp[1];

    return 0;
}