#include "LoggingObserver.h"

std::vector<Observer*> Subject::observers;

Observer::~Observer() = default;

Subject::Subject() = default;

Subject::Subject(const Subject &subject) = default;

Subject &Subject::operator=(const Subject &subject) = default;

std::ostream& operator<<(std::ostream &out, const Subject &subject) {
    out << "Subject: \n\tNbr of observers: " << Subject::observers.size() << std::endl;
    return out;
}

void Subject::attach(Observer *observer) {
    // add an observer to the subject's list of observers
    observers.push_back(observer);
}

void Subject::detach(const Observer *observer) {
    // remove an observer from the subject's list of observers
    auto it = std::find(observers.begin(), observers.end(), observer);
    if (it == observers.end()) observers.erase(it-1);
    else observers.erase(it);
}

void Subject::notify(const ILoggable &loggable) {
    // update the state of every observer subscribed to the subject
    for (auto *observer : observers) {
        observer->update(loggable);
    }
}

Subject::~Subject() = default;

// ===================== LogObserver class =====================

LogObserver::LogObserver() {
    openLogFileStream();
}

LogObserver::LogObserver(const LogObserver &logObserver) {
    this->openLogFileStream();
}

void LogObserver::openLogFileStream() {
    this->fileStream.open("gamelog.txt", std::ios::app);
}

void LogObserver::closeLogFileStream() {
    this->fileStream.close();
}

LogObserver &LogObserver::operator=(const LogObserver &logObserver) {
    if (this == &logObserver) return *this;
    return *this; // nothing to copy
}

void LogObserver::update(const ILoggable &loggable) {
    // write log to file
    std::string log = loggable.stringToLog(); // get the string to log from the observed
    fileStream << log << std::endl;
    fileStream << "============================" << std::endl;
}

std::ostream &operator<<(std::ostream &out, const LogObserver &logObserver) {
    out << "LogObserver: \n\tLog file status: \n\topen:" << logObserver.fileStream.is_open() << std::endl;
    return out;
}

LogObserver::~LogObserver() {
    closeLogFileStream();
}
