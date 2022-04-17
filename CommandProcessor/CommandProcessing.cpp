#include <sstream>
#include <utility>
#include "CommandProcessing.h"
#include "../GameEngine/GameEngine.h"

// ==================== Command class ========================
// default constructor
Command::Command() = default;

// constructor with the command's name
Command::Command(string commands) : Subject() {
    command = std::move(commands);
    effect = "";
}

// copy constructor
Command::Command(const Command &command) {
    this->command = command.command;
    this->effect = command.effect;
}

// destructor
Command::~Command() = default;

// assignment operator
Command& Command::operator=(const Command &c) {
    if (this == &c) return *this;
    this->command = c.command;
    this->effect = c.effect;
    return *this;
}

// stream insertion operator
ostream& operator<<(ostream &out, const Command &command){
    out << "Command: " << command.command << "\nEffect: " << command.effect << "\n";
    return out;
}

void Command::saveEffect(string commandEffect) {
    effect = std::move(commandEffect);
    notify(*this); // notify all observers of this subject to write log to file
}

string Command::stringToLog() const {
    return "Command: " + this->command + "\tEffect: " + this->effect;
}

// ==================== CommandProcessor class ========================
// default constructor
CommandProcessor::CommandProcessor() = default;

// copy constructor
[[maybe_unused]] CommandProcessor::CommandProcessor(const CommandProcessor &commandProcessor) {
    this->currentState = commandProcessor.currentState;
    for (Command *c : commandProcessor.commandList) {
        commandList.push_back(new Command(*c));
    }
}

// destructor
CommandProcessor::~CommandProcessor() {
    cout << "Invoking delete constructor for CommandProcessor" << endl;
    for(Command *c: commandList) {
        delete c;
    }
}

string CommandProcessor::stringToLog() const {
    return "Command processor saved new command in the command list: " + this->commandList.back()->command;
}

// assignment operator
CommandProcessor& CommandProcessor::operator=(const CommandProcessor &commandProcessor) {
    if(this == &commandProcessor) return *this;

    this->currentState = commandProcessor.currentState;
    for (Command *c : this->commandList) {
        delete c;
    }
    this->commandList.clear();
    for (Command *c : commandProcessor.commandList) {
        commandList.push_back(new Command(*c));
    }
    return *this;
}

// stream insertion operator
ostream& operator<<(ostream &out, const CommandProcessor &commandProcessor){
    out << "The CommandProcessor's list of commands:\n";
    for(Command *c: commandProcessor.commandList){
        out << *c;
    }
    return out;
}

// prompts for user input, creates & returns a new command object from it while also saving it to the command list
Command& CommandProcessor::getCommand() {
    string read = readCommand();
    auto *c = new Command(read);
    saveCommand(*c);
    return *c;
}

// prompts for and returns user input
string CommandProcessor::readCommand() {
    string readCommandInput;
    cout << "Input command:";
    getline(cin, readCommandInput);
    return readCommandInput;
}

// adds command to the command list
void CommandProcessor::saveCommand(Command &command) {
    commandList.push_back(&command);
    notify(*this); // notify all observers of this subject to write log to file
}

// validate (1) user command (2) command is used in the correct state
bool CommandProcessor::validate(const string& readCommandInput, const GameEngine &gameEngine) {
    string temp = readCommandInput;

    vector<string> inputWords;
    currentState = gameEngine.getState();
    string userInputCommand, userInputSecondWord;
    istringstream parse(readCommandInput);
    while (parse >> temp){
        inputWords.push_back(temp);
    }
    int vectorSize = static_cast<int>(inputWords.size());
    userInputCommand = inputWords.at(0);

    if (vectorSize == 2) {
        if ((userInputCommand == "loadmap" && (currentState == "start" || currentState == "maploaded")) ||
            (userInputCommand == "addplayer" && (currentState == "mapvalidated" || currentState == "playersadded"))) {
            return true;
        }
    }
    else if (vectorSize == 1) {
        if ((userInputCommand == "validatemap" && currentState == "maploaded") || (userInputCommand == "gamestart" && currentState == "playersadded")
            || (userInputCommand == "replay" && currentState == "win") || (userInputCommand == "quit" && currentState == "win")) {
            return true;
        }
    }
    else if(vectorSize > 5){
        if(currentState.empty() && inputWords.at(0) == "tournament" && inputWords.at(1) == "-M"){
            string nextStr[3] = {"P", "G", "D"}; // next string in the tournament string
            int index = 0; // index for nextStr[]

            // check order of tournament commands
            for(int i = 2; i < vectorSize; i++){
                string temp = inputWords.at(i);
                if(temp.substr(0, 1) == "-") {
                    if(index < 3 && temp.substr(1, 2) == nextStr[index]){
                        index++;
                    }
                    else{
                        cout << " Invalid tournament command." << endl;
                        return false;
                    }
                }
            }

            // check number of maps
            string lineBetween = readCommandInput.substr(readCommandInput.find("-M") + 2, readCommandInput.find("-P") - readCommandInput.find("-M") -2);
            vector<string> wordsBetween;
            istringstream parse2(lineBetween);
            while(parse2 >> lineBetween){
                wordsBetween.push_back(lineBetween);
            }
            int numberOfWordsBetween = static_cast<int>(wordsBetween.size());
            if(numberOfWordsBetween < 1 || numberOfWordsBetween > 5){
                cout << "Invalid number of maps." << endl;
                return false;
            }
            wordsBetween.erase(wordsBetween.begin(), wordsBetween.end());

            // check number of player strats and if each player strategy is a Aggressive, Neutral, Benevolent or Cheater strategy
            lineBetween = readCommandInput.substr(readCommandInput.find("-P") + 2, readCommandInput.find("-G") - readCommandInput.find("-P") - 2);
            istringstream parse3(lineBetween);
            while(parse3 >> lineBetween){
                wordsBetween.push_back(lineBetween);
            }
            numberOfWordsBetween = static_cast<int>(wordsBetween.size());
            if(numberOfWordsBetween < 2 || numberOfWordsBetween > 4){
                cout << "Invalid number of player strategies." << endl;
                return false;
            }
            for(const string &ps: wordsBetween){
                if(ps != "Aggressive" && ps != "Neutral" && ps != "Benevolent" && ps != "Cheater"){
                    cout << ps << " is not a player strategy." << endl;
                    return false;
                }
            }
            wordsBetween.erase(wordsBetween.begin(), wordsBetween.end());

            // check if number of games is a number and the range
            lineBetween = readCommandInput.substr(readCommandInput.find("-G") + 2, readCommandInput.find("-D") - readCommandInput.find("-G") -2);
            istringstream parse4(lineBetween);
            while(parse4 >> lineBetween){
                wordsBetween.push_back(lineBetween);
            }
            numberOfWordsBetween = static_cast<int>(wordsBetween.size());
            if(numberOfWordsBetween != 1){
                cout << "Invalid number of games string." << endl;
                return false;
            }
            for(char const& c: wordsBetween.at(0)){
                if(!std::isdigit(c)){
                    cout << "Number of games is not a number." << endl;
                    return false;
                }
            }
            int noOfGames = std::stoi(wordsBetween.at(0));
            if(noOfGames < 1 || noOfGames > 5){
                cout << "Invalid number of games." << endl;
                return false;
            }
            wordsBetween.erase(wordsBetween.begin(), wordsBetween.end());

            // check if number of turns is a number and the range
            lineBetween = readCommandInput.substr(readCommandInput.find("-D") + 2);
            istringstream parse5(lineBetween);
            while(parse5 >> lineBetween){
                wordsBetween.push_back(lineBetween);
            }
            numberOfWordsBetween = static_cast<int>(wordsBetween.size());
            if(numberOfWordsBetween != 1){
                cout << "Invalid number of turns string." << endl;
                return false;
            }
            for(char const& c: wordsBetween.at(0)){
                if(!std::isdigit(c)){
                    cout << "Number of turns is not a number." << endl;
                    return false;
                }
            }
            int maxNoOfTurns = std::stoi(wordsBetween.at(0));
            if(maxNoOfTurns < 10 || maxNoOfTurns > 50){
                cout << "Invalid max number of turns." << endl;
                return false;
            }
            wordsBetween.erase(wordsBetween.begin(), wordsBetween.end());

            if(index == 3){
                return true;
            }
        }
    }
    cout << "Invalid command." << endl;
    return false;
}

//============================FileLineReader============================

// default constructor sets the file name to NoFileName
FileLineReader::FileLineReader() {
    fileName = "NoFileName";
    stateOfFile = false;
}

// constructor to open a file with the passed file name
FileLineReader::FileLineReader(string otherFileName) {
    fileName = otherFileName;
    stateOfFile = false;
}

// copy constructor
FileLineReader::FileLineReader(const FileLineReader &flr) {
    if(flr.inputFileStream.is_open()){
        this->inputFileStream.open(flr.fileName);
    }
    this->fileName = flr.fileName;
    this->stateOfFile = flr.stateOfFile;
}

// destructor
FileLineReader::~FileLineReader() {
    if (isFileOpen()) {
        closeFile();
    }
}

// assignment operator
FileLineReader& FileLineReader::operator=(const FileLineReader &flr) {

    if(this == &flr) return *this;

    // close stream if it's open
    if (isFileOpen()) {
        inputFileStream.close();
    }
    // open stream if parameter FLR's stream is open
    if (flr.inputFileStream.is_open()) {
        inputFileStream.open(flr.fileName);
    }
    this->fileName = flr.fileName;
    this->stateOfFile = flr.stateOfFile;
    return *this;
}

// ostream operator outputs the name of the file
ostream& operator<<(ostream &out, const FileLineReader &flr) {
    out << flr.getFileName();
    return out;
}

// reads a line from the inputFileStream
string FileLineReader::readLineFromFile(){
    // checks if no files are open
    // if true, exit program
    if(!isFileOpen()){
        cout << "No open files." << endl;
        cout << "Exiting." << endl;
        exit(0);
    }

    // get a line from a file, puts it in line variable
    char line[250];
    inputFileStream.getline(line, 250);
    string lineInString = line;

    // check if the line is a space, if so, set return string to NULL
    if(lineInString.empty()){
        lineInString = "NULL";
    }

    // check if next line is end of file, if so, close stream
    if(checkEOF()){
        closeFile();
    }

    return lineInString;
}

// opens a file with the file name the member variable fileName
void FileLineReader::openFile(){
    inputFileStream.open(fileName);

    // check if file is opened
    if(!inputFileStream.is_open()){
        cout << "Error opening file " << fileName << "." << endl;
    }
    else{
        stateOfFile = true;
    }
}

// closes a file with the file name the member variable fileName
void FileLineReader::closeFile(){
    // checks if there is not a file open, notifies user
    if(!inputFileStream.is_open()){
        cout << "Trying to close an unopened file with file name " << fileName << "." << endl;
    }
    else {
        inputFileStream.close();
        stateOfFile = false;
    }
}

// returns a boolean value depending if the next line is the end of file or not
bool FileLineReader::checkEOF(){
    return inputFileStream.peek() == EOF;
}

// check if a file is open
bool FileLineReader::isFileOpen() const{
    return stateOfFile;
}

// returns the name of the file
string FileLineReader::getFileName() const{
    return fileName;
}

//============================FileCommandProcessor============================
// default constructor
FileCommandProcessorAdapter::FileCommandProcessorAdapter(){
    flr = new FileLineReader();
}

// constructor to open a file with the fileName parameter
FileCommandProcessorAdapter::FileCommandProcessorAdapter(string fileName){
    flr = new FileLineReader(std::move(fileName));
    flr->openFile();
}

// copy constructor
FileCommandProcessorAdapter::FileCommandProcessorAdapter(const FileCommandProcessorAdapter& fcpa){
    flr = new FileLineReader(*fcpa.flr);
    if(fcpa.flr->isFileOpen()) {
        flr->openFile();
    }
}

// destructor
FileCommandProcessorAdapter::~FileCommandProcessorAdapter(){
    delete flr;
}

// assignment operator
FileCommandProcessorAdapter& FileCommandProcessorAdapter::operator=(const FileCommandProcessorAdapter &fcpa){
    if(this == &fcpa){ return *this;}

    CommandProcessor::operator=(fcpa);

    delete flr;
    flr = new FileLineReader(*fcpa.flr);
    if(fcpa.flr->isFileOpen()){
        this->flr->openFile();
    }

    return *this;
}

// ostream operator outputs the opened file name
ostream& operator<<(ostream &out, const FileCommandProcessorAdapter &fcpa){
    out << "FileCommandProcessorAdapter's file: " << fcpa.getFLRFileName() << "\n";
    out << "FileCommandProcessorAdapter's list of commands:\n";
    for (Command *c: fcpa.commandList){
        out << *c;
    }
    return out;
}

// method to open a specific file with the parameter fileName
void FileCommandProcessorAdapter::openSpecificFLRFile(string fileName){
    // deletes the flr made in initialization and makes a new one with the fileName parameter
    delete flr;
    flr = new FileLineReader(std::move(fileName));
    flr->openFile();
}

// method to close FileLineReader file
void FileCommandProcessorAdapter::closeFLRFile(){
    flr->closeFile();
}

// checks if the FileLineReader file is open
bool FileCommandProcessorAdapter::isFLRFileOpen(){
    return flr->isFileOpen();
}

// checks if there are more commands in the FineLineReader file
bool FileCommandProcessorAdapter::moreCommands(){
    return !flr->checkEOF();
}

// method get the opened file name
string FileCommandProcessorAdapter::getFLRFileName() const{
    return flr->getFileName();
}

// overridden readCommand() method from CommandProcessor that gets a line from the file and returns it as a string
string FileCommandProcessorAdapter::readCommand(){
    string command = flr->readLineFromFile();
    return command;
}
