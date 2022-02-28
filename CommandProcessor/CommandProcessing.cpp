//
// Created by Judy Lombardo on 2022-02-20.
//
#include <sstream>
#include "CommandProcessing.h"


string mapName[] ={"smallsolar","canada"}; //list of maps


string CommandProcessor::getCommand() {
    string read= readCommand();
    saveCommand(read);
    return read;
}
string CommandProcessor::readCommand(){
    string readCommandInput;
    getline(cin,readCommandInput);
    return readCommandInput;
}
void CommandProcessor::saveCommand(string readCommandInput){
    //TODO create a command objects to put in a list
    //create Command object
commandList.emplace_back(new Command(readCommandInput));

}

//validate (1) user command (2) command is used in the correct state
bool CommandProcessor::validate(string readCommandInput) {
    currentState = GameEngine::getState();

    //to parse the getline input
    string userInputCommand,userInputSecondWord;
    istringstream parse(readCommandInput);
    while (parse>>readCommandInput){
        inputWords.push_back(readCommandInput);
    }
    int vectorSize= inputWords.size();

    //size =2 for user command input
    if(vectorSize==2) {
        cout<<" I typed 2 inputWords"<<" array size: "<<vectorSize<<" current state: "<<currentState<<endl;
        userInputCommand=inputWords.at(0);
        userInputSecondWord =inputWords.at(1); //can either be a map file name or a player name
        int sizeOfMapNameArray = size(mapName); //using map list on line 8
        bool validMapName=false;

        //checks if the second input matches a map name in the mapName array
        for(int i=0;i<sizeOfMapNameArray;i++){
            if(userInputSecondWord == mapName[i])
                validMapName =true;
        }
        if (userInputCommand == "loadmap" &&validMapName && currentState == "start" || currentState == "maploaded") {
            cout << "user command:" << userInputCommand <<" map name: "<<userInputSecondWord<< " current state:" << currentState << endl;
            cout<<"the command "<<userInputCommand<< " is valid in the current game state "<< currentState<<endl;
            return true;
        } else if (userInputCommand == "addplayer" && currentState == "mapvalidated" ||
                   currentState == "playersadded") {
            cout << "user command:" << userInputCommand <<" player name: "<<userInputSecondWord<< " current state:" << currentState << endl;
            return true;
        }
    }
    //for user command input
    else if (vectorSize ==1) {
        userInputCommand=inputWords.at(0);
        cout<<"I typed 1 inputWords. User input word: "<<userInputCommand<<" "<<currentState<<endl;
        //size =1
        if (userInputCommand == "validatemap" && currentState == "maploaded") {
            cout << "user command:" << userInputCommand << " current state:" << currentState << endl;
            return true;
        } else if (userInputCommand == "gamestart" && currentState == "playersadded") {
            cout << "user command:" << userInputCommand << " current state:" << currentState << endl;
            return true;
        } else if (userInputCommand == "replay" && currentState == "win") {
            cout << "user command:" << userInputCommand << " current state:" << currentState << endl;
            return true;
        } else if (userInputCommand == "quit" && currentState == "win") {
            cout << "user command:" << userInputCommand << " current state:" << currentState << endl;
            return true;
        }else{
            cout<<"none of the above"<<endl;
            return false;
        }
    }
    //size >2 or else for user command input
    else{
        cout<<"I typed anything else inputWords"<<endl;
        cout<<"the command"<<userInputCommand<< " is not valid in the current game state: "<< currentState<<endl;
    }

    return false;
}

CommandProcessor::~CommandProcessor() {
    cout<<"Invoking delete constructor or CommandProcessor"<<endl;

}
Command::Command(string commands) {
command=commands;
effect =" ";
}

void Command::setCommand(string commands) {
command =commands;
}

void Command::setEffect(string effects) {
effect = effects;

}

void Command::saveEffect(string commandEffect) {
    //TODO add save effect to previous created object
    Command::setEffect(commandEffect);
}

//=========FileLineReader=========
// constructor to open a file with the passed file name
FileLineReader::FileLineReader(string otherFileName){
    fileEOF = false;
    fileName = otherFileName;
    inputFileStream.open(fileName);
}

// copy constructor
FileLineReader::FileLineReader(const FileLineReader &flr){
    if(flr.inputFileStream.is_open()){
        this->fileName = flr.fileName;
        this->fileEOF = flr.fileEOF;
        inputFileStream.open(flr.fileName);
    }
}

// destructor
FileLineReader::~FileLineReader(){
    if(inputFileStream.is_open()){
        inputFileStream.close();
    }
}

// equal operator
FileLineReader& FileLineReader::operator=(const FileLineReader &flr){

    if(this == &flr){return *this;}

    if(inputFileStream.is_open()){
        inputFileStream.close();
    }
    if(flr.inputFileStream.is_open()){
        this->fileEOF = flr.fileEOF;
        this->fileName = flr.fileName;
        inputFileStream.open(flr.fileName);
    }

    return *this;
}

// ostream operator outputs the name of the file
ostream& operator<<(ostream &out, const FileLineReader &flr){
    out << "File name: " << flr.getFileName();
    return out;
}

// reads a line from the inputFileStream
string FileLineReader::readLineFromFile(){
    // checks if no files are open
    if(!inputFileStream.is_open()){
        cout << "No open files." << endl;
        cout << "Exiting." << endl;
        exit(1);
    }

    string line;
    // get a line from a file, puts it in line variable and then check if it is end of file or not depending on the return value of getline()
    if(getline(inputFileStream, line)){
        fileEOF = false;
    }
    else{
        fileEOF = true;
    }

    return line;
}

// returns a boolean value depending if the line reader is on the end of the file
bool FileLineReader::checkEOF(){
    return fileEOF;
}

// returns the name of the file
string FileLineReader::getFileName() const{
    return fileName;
}

//=========FileCommandProcessor=========

// FileCommandProcessorAdapter::FileCommandProcessorAdapter(string fileName){
//     flr = new FileLineReader(fileName);
// }

// copy constructor
FileCommandProcessorAdapter::FileCommandProcessorAdapter(const FileCommandProcessorAdapter& fcpa): CommandProcessor(fcpa){
    flr = new FileLineReader(*fcpa.flr);
}

// destructor
FileCommandProcessorAdapter::~FileCommandProcessorAdapter(){
    delete flr;
}

// equal operator(not finished)
FileCommandProcessorAdapter& FileCommandProcessorAdapter::operator=(const FileCommandProcessorAdapter &fcpa){
    if(this == &fcpa){ return *this;}

    delete flr;
    flr = new FileLineReader(*fcpa.flr);

    return *this;
}

// ostream operator ouputs the opened file name
ostream& operator<<(ostream &out, const FileCommandProcessorAdapter &fcpa){
    out << "FileCommandProcessorAdapter's file: " << fcpa.getFileName();
    return out;
}

// method to open a file with the parameter fileName
void FileCommandProcessorAdapter::openFile(string fileName){
    flr = new FileLineReader(fileName);
}

// method to close a file
void FileCommandProcessorAdapter::closeFile(){
    delete flr;
}

// checks if there are more commands
bool FileCommandProcessorAdapter::moreCommands(){
    if(flr->checkEOF()){
        closeFile();
    }
    return !flr->checkEOF();
}

// method get the opened file name
string FileCommandProcessorAdapter::getFileName() const{
    return flr->getFileName();
}

// overridden readCommand() method from CommandProcessor that returns a command string
string FileCommandProcessorAdapter::readCommand(){
    string command = flr->readLineFromFile();
    return command;
}