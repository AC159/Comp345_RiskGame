#include <sstream>
#include <utility>
#include "CommandProcessing.h"
#include "../GameEngine/GameEngine.h"

// ==================== Command class ========================
// default constructor
Command::Command() = default;

// constructor with the command's name
Command::Command(string commands) {
    command = std::move(commands);
    effect = " ";
}

// copy constructor
Command::Command(const Command &command){
    this->command = command.command;
    this->effect = command.effect;
}

// destructor
Command::~Command() = default;

// assignment operator
Command& Command::operator=(const Command &command){
    if (this == &command){ return *this;}

    this->command = command.command;
    this->effect = command.effect;

    return *this;
}

// stream insertion operator
ostream& operator<<(ostream &out, const Command &command){
    out << "Command: " << command.command << "\n";
    out << "Effect: " << command.effect << "\n";

    return out;
}

void Command::saveEffect(string commandEffect) {
    effect = std::move(commandEffect);
}

// ==================== CommandProcessor class ========================
// default constructor
CommandProcessor::CommandProcessor() = default;

// copy constructor
CommandProcessor::CommandProcessor(const CommandProcessor &commandProcessor){
    this->currentState = commandProcessor.currentState;
    for(Command *c: commandList){
        commandList.push_back(new Command(*c));
    }
}

// destructor
CommandProcessor::~CommandProcessor() {
    cout << "Invoking delete constructor or CommandProcessor" << endl;
    for(Command *c: commandList){
        delete c;
    }
}

// assignment operator
CommandProcessor& CommandProcessor::operator=(const CommandProcessor &commandProcessor){
    if(this == &commandProcessor){ return *this;}

    this->currentState = commandProcessor.currentState;
    for(Command *c: commandList){
        delete c;
    }
    for(Command *c: commandList){
        commandList.push_back(new Command(*c));
    }

    return *this;
}

// stream insertion operator
ostream& operator<<(ostream &out, const CommandProcessor &commandProcessor){
    out << "The CommandProcessor's list of commands:\n\n";
    for(Command *c: commandProcessor.commandList){
        out << *c << "\n";
    }
    return out;
}

Command& CommandProcessor::getCommand() {
    string read = readCommand();
    Command *c = new Command(read);
    saveCommand(*c);
    return *c;
}

string CommandProcessor::readCommand() {
    string readCommandInput;
    cout << "Command: ";
    getline(cin, readCommandInput);
    return readCommandInput;
}

void CommandProcessor::saveCommand(Command &command){
    commandList.push_back(&command);
}

// validate (1) user command (2) command is used in the correct state
bool CommandProcessor::validate(string readCommandInput, const GameEngine &gameEngine) {
    vector<string> inputWords;
    currentState = gameEngine.getState();
    string userInputCommand, userInputSecondWord;
    istringstream parse(readCommandInput);
    while (parse >> readCommandInput){
        inputWords.push_back(readCommandInput);
    }
    int vectorSize= inputWords.size();
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
    return false;
}

//============================FileLineReader============================

// default constructor sets the file name to NoFileName
FileLineReader::FileLineReader(){
    fileName = "NoFileName";
    stateOfFile = false;
}

// constructor to open a file with the passed file name
FileLineReader::FileLineReader(string otherFileName){
    fileName = otherFileName;
    stateOfFile = false;
}

// copy constructor
FileLineReader::FileLineReader(const FileLineReader &flr){
    if(flr.inputFileStream.is_open()){
        this->inputFileStream.open(flr.fileName);
    }
    this->fileName = flr.fileName;
    this->stateOfFile = flr.stateOfFile;
}

// destructor
FileLineReader::~FileLineReader(){
    if (isFileOpen()) {
        closeFile();
    }
}

// assignment operator
FileLineReader& FileLineReader::operator=(const FileLineReader &flr) {

    if(this == &flr){return *this;}

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
    if(lineInString.size() == 0){
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
    out << "FileCommandProcessorAdapter's list of commands:\n\n";
    for (Command *c: fcpa.commandList){
        out << c;
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
