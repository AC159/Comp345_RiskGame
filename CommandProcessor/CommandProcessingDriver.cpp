#include <iterator>
#include <iostream>
#include "CommandProcessing.h"
#include "../GameEngine/GameEngine.h"

// prompts user for input from the console
string askUser()
{
    string input;

    cout << "Choose way of command input(-console, -file <filename>): ";
    getline(cin, input);

    return input;
}

// checks the name of the file
string checkFileName(string userInput)
{
    vector<string> wordsFromInput;
    string temp = userInput;
    string delimiter1 = " ";
    string delimiter2 = "\n";
    int pos = 0;

    // while loop to parse string based on the delimiter and put the words in a vector
    while ((pos = temp.find(delimiter1)) != std::string::npos)
    {
        string wordFromInput = temp.substr(0, pos);
        wordsFromInput.emplace_back(wordFromInput);
        temp.erase(0, pos + delimiter1.length());
    }
    wordsFromInput.emplace_back(temp); // put the last word of the line in a vector

    // more than 2 words is invalid
    if (wordsFromInput.size() > 2)
    {
        return "";
    }
    // if words is only 1, because -file needs a file name
    else if (wordsFromInput.size() == 1)
    {
        return "";
    }
    // if file name does not either or both angled brackets
    else if (wordsFromInput[1].find("<") == std::string::npos || wordsFromInput[1].find(">") == std::string::npos)
    {
        return "";
    }
    // return the file name without the angled brackets
    else
    {
        // string returnString;
        return "..\\FileProcessorText\\" + wordsFromInput[1].substr(wordsFromInput[1].find("<") + 1, wordsFromInput[1].find(">") - wordsFromInput[1].find("<") - 1);
    }
}

// checks the commands from the file of FileCommandProcessorAdapter
bool checkForCommands(FileCommandProcessorAdapter &fcpa, GameEngine &ge, bool &gameEnd)
{
    bool commandValid{false}; 
    
    // loop if there are more commands or the command is invalid in the state
    while (fcpa.moreCommands() && !commandValid)
    {
        string commandName = fcpa.getCommand();
        commandValid = fcpa.validate(commandName, ge);

        if (commandValid)
        {
            // check for loop backs in map loaded and player added state
            if (commandName.substr(0, 7) == "loadmap" && ge.getState() == "map loaded" || commandName.substr(0, 9) == "addplayer" && ge.getState() == "players added")
            {
                commandValid = false;
            }
            // check for game end commands
            if (commandName.substr(0, 4) == "quit" && ge.getState() == "win")
            {
                gameEnd = true;
            }
            string commandEffect = "Command " + commandName + " valid in state " + ge.getState() + ".";
            cout << commandEffect << endl;
            // save effect
        }
        else
        {
            string commandEffect = "Command " + commandName + " invalid in state " + ge.getState() + ".";
            cout << commandEffect << endl;
            // save effect
        }
    }
    return commandValid;
}

// opens the file in the parameter FileCommandProcessorAdapter with the file parameter
void openFile(FileCommandProcessorAdapter &fcpa, string fileName)
{
    // check if the file is open, close file if so
    if (fcpa.isFLRFileOpen())
    {
        fcpa.closeFLRFile();
    }
    fcpa.openSpecificFLRFile(fileName);
}

int main()
{

    string userInput, fileName;
    GameEngine *gameEngine = new GameEngine();
    CommandProcessor *cp[2];
    cp[0] = new CommandProcessor();
    cp[1] = new FileCommandProcessorAdapter();

    bool gameEnd(false);
    bool commandValid(false);

start:
    //==========================start==========================
    gameEngine->startStateChange();
    gameEnd = false;
    commandValid = false;
    // check if file is still open from other states
    // search for a valid command in the checkForCommands method
    if (dynamic_cast<FileCommandProcessorAdapter *>(cp[1])->isFLRFileOpen())
    {
        commandValid = checkForCommands(*dynamic_cast<FileCommandProcessorAdapter *>(cp[1]), *gameEngine, gameEnd);
    }
    // loop while command is invalid
    while (!commandValid)
    {
        // prompts user for input
        userInput = askUser();
        if (userInput == "-console")
        {
            cout << "console" << endl;
        }
        else if (userInput.substr(0, userInput.find(" ")) == "-file")
        {
            fileName = checkFileName(userInput);
            // checks if file has an invalid name
            if (fileName.empty())
            {
                cout << "Invalid file name." << endl;
            }
            // otherwise, open the file in FileCommandProcessorAdapter then check for commands in the file
            else if (!fileName.empty())
            {
                openFile(*dynamic_cast<FileCommandProcessorAdapter *>(cp[1]), fileName);
                commandValid = checkForCommands(*dynamic_cast<FileCommandProcessorAdapter *>(cp[1]), *gameEngine, gameEnd);
            }
        }
        else
        {
            cout << "Invalid input." << endl;
        }
    }
    cout << endl;

    //==========================map loaded==========================
    gameEngine->mapLoadedStateChange();
    gameEnd = false;
    commandValid = false;
    // check if file is still open from other states
    // search for a valid command in the checkForCommands method
    if (dynamic_cast<FileCommandProcessorAdapter *>(cp[1])->isFLRFileOpen())
    {
        commandValid = checkForCommands(*dynamic_cast<FileCommandProcessorAdapter *>(cp[1]), *gameEngine, gameEnd);
    }
    // loop while command is invalid
    while (!commandValid)
    {
        // prompts user for input
        userInput = askUser();
        if (userInput == "-console")
        {
            cout << "console" << endl;
        }
        else if (userInput.substr(0, userInput.find(" ")) == "-file")
        {
            fileName = checkFileName(userInput);
            // checks if file has an invalid name
            if (fileName.empty())
            {
                cout << "Invalid file name." << endl;
            }
            // otherwise, open the file in FileCommandProcessorAdapter then check for commands in the file
            else if (!fileName.empty())
            {
                openFile(*dynamic_cast<FileCommandProcessorAdapter *>(cp[1]), fileName);
                commandValid = checkForCommands(*dynamic_cast<FileCommandProcessorAdapter *>(cp[1]), *gameEngine, gameEnd);
            }
        }
        else
        {
            cout << "Invalid input." << endl;
        }
    }
    cout << endl;

    //==========================map validated==========================
    gameEngine->mapValidatedStateChange();
    gameEnd = false;
    commandValid = false;
    // check if file is still open from other states
    // search for a valid command in the checkForCommands method
    if (dynamic_cast<FileCommandProcessorAdapter *>(cp[1])->isFLRFileOpen())
    {
        commandValid = checkForCommands(*dynamic_cast<FileCommandProcessorAdapter *>(cp[1]), *gameEngine, gameEnd);
    }
    // loop while command is invalid
    while (!commandValid)
    {
        // prompts user for input
        userInput = askUser();
        if (userInput == "-console")
        {
            cout << "console" << endl;
        }
        else if (userInput.substr(0, userInput.find(" ")) == "-file")
        {
            fileName = checkFileName(userInput);
            // checks if file has an invalid name
            if (fileName.empty())
            {
                cout << "Invalid file name." << endl;
            }
            // otherwise, open the file in FileCommandProcessorAdapter then check for commands in the file
            else if (!fileName.empty())
            {
                openFile(*dynamic_cast<FileCommandProcessorAdapter *>(cp[1]), fileName);
                commandValid = checkForCommands(*dynamic_cast<FileCommandProcessorAdapter *>(cp[1]), *gameEngine, gameEnd);
            }
        }
        else
        {
            cout << "Invalid input." << endl;
        }
    }
    cout << endl;

    //==========================players added==========================
    gameEngine->playersAddedStateChange();
    gameEnd = false;
    commandValid = false;
    // check if file is still open from other states
    // search for a valid command in the checkForCommands method
    if (dynamic_cast<FileCommandProcessorAdapter *>(cp[1])->isFLRFileOpen())
    {
        commandValid = checkForCommands(*dynamic_cast<FileCommandProcessorAdapter *>(cp[1]), *gameEngine, gameEnd);
    }
    // loop while command is invalid
    while (!commandValid)
    {
        // prompts user for input
        userInput = askUser();
        if (userInput == "-console")
        {
            cout << "console" << endl;
        }
        else if (userInput.substr(0, userInput.find(" ")) == "-file")
        {
            fileName = checkFileName(userInput);
            // checks if file has an invalid name
            if (fileName.empty())
            {
                cout << "Invalid file name." << endl;
            }
            // otherwise, open the file in FileCommandProcessorAdapter then check for commands in the file
            else if (!fileName.empty())
            {
                openFile(*dynamic_cast<FileCommandProcessorAdapter *>(cp[1]), fileName);
                commandValid = checkForCommands(*dynamic_cast<FileCommandProcessorAdapter *>(cp[1]), *gameEngine, gameEnd);
            }
        }
        else
        {
            cout << "Invalid input." << endl;
        }
    }
    cout << endl;

    //---> assign countries here: to be implemented next phase
assignReinforcement:
    //==========================assign reinforcement==========================
    gameEngine->assignReinforcementStateChange();
    gameEngine->validateAssignReinforcementCommand();

    //==========================issue orders==========================
    gameEngine->issueOrdersStateChange();
    gameEngine->validateIssueOrdersCommand();

    //==========================execute orders==========================
    gameEngine->executeOrdersStateChange();
    if (gameEngine->validateExecuteOrdersCommand() == 1)
        goto assignReinforcement;

    //==========================win==========================
    gameEngine->winStateChange();
    gameEnd = false;
    commandValid = false;
    // check if file is still open from other states
    // search for a valid command in the checkForCommands method
    if (dynamic_cast<FileCommandProcessorAdapter *>(cp[1])->isFLRFileOpen())
    {
        commandValid = checkForCommands(*dynamic_cast<FileCommandProcessorAdapter *>(cp[1]), *gameEngine, gameEnd);
    }
    // loop while command is invalid
    while (!commandValid)
    {
        // prompts user for input
        userInput = askUser();
        if (userInput == "-console")
        {
            cout << "console" << endl;
        }
        else if (userInput.substr(0, userInput.find(" ")) == "-file")
        {
            fileName = checkFileName(userInput);
            // checks if file has an invalid name
            if (fileName.empty())
            {
                cout << "Invalid file name." << endl;
            }
            // otherwise, open the file in FileCommandProcessorAdapter then check for commands in the file
            else if (!fileName.empty())
            {
                openFile(*dynamic_cast<FileCommandProcessorAdapter *>(cp[1]), fileName);
                commandValid = checkForCommands(*dynamic_cast<FileCommandProcessorAdapter *>(cp[1]), *gameEngine, gameEnd);
            }
        }
        else
        {
            cout << "Invalid input." << endl;
        }
    }
    cout << endl;
    // goes back to start if command is not quit in win state
    if (!gameEnd)
        goto start;

    delete gameEngine;
    delete cp[0];
    delete cp[1];

    return 0;
}