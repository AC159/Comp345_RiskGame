#include "LoggingObserver.h"


Observer::~Observer() = default;

Subject::Subject() = default;

Subject::Subject(const Subject &subject) {
    // shallow copy of the subject's observers
    for (auto* observer : subject.observers) {
        this->observers.push_back(observer);
    }
}

Subject &Subject::operator=(const Subject &subject) {
    if (&subject == this) return *this;
    for (auto* observer : this->observers) {
        delete observer;
    }
    this->observers.clear();
    // shallow copy of the subject's observers
    for (auto* observer : subject.observers) {
        this->observers.push_back(observer);
    }
}

std::ostream& operator<<(std::ostream &out, const Subject &subject) {
    out << "Subject: \n\tNbr of observers: " << subject.observers.size() << std::endl;
    return out;
}

void Subject::attach(Observer *observer) {
    // add an observer to the subject's list of observers
    this->observers.push_back(observer);
}

void Subject::detach(const Observer *observer) {
    // remove an observer from the subject's list of observers
    auto it = std::find(this->observers.begin(), this->observers.end(), observer);
    if (it == this->observers.end()) this->observers.erase(this->observers.begin(), it-1);
    else this->observers.erase(this->observers.begin(), it);
}

void Subject::notify(const ILoggable &loggable) {
    // update the state of every observer subscribed to the subject
    for (auto *observer : this->observers) {
        observer->update(loggable);
    }
}

Subject::~Subject() = default;

// ===================== LogObserver class =====================

LogObserver::LogObserver() {
    openLogFileStream();
}

LogObserver::LogObserver(const LogObserver &logObserver) {
    // shallow copy of all subjects in the logObserver object
    for (auto* subject : logObserver.subjects) {
        this->subjects.push_back(subject);
    }
}

void LogObserver::openLogFileStream() {
    this->fileStream.open("gamelog.txt", std::ios::out);
}

void LogObserver::closeLogFileStream() {
    this->fileStream.close();
}

LogObserver &LogObserver::operator=(const LogObserver &logObserver) {
    if (&logObserver == this) return *this;
    for (auto* subject : this->subjects) {
        delete subject;
    }
    this->subjects.clear();
    // shallow copy of all subjects in the logObserver object
    for (auto* subject : logObserver.subjects) {
        this->subjects.push_back(subject);
    }
    return *this;
}

void LogObserver::update(const ILoggable &loggable) {
    // write log to file
    std::string log = loggable.stringToLog(); // get the string to log from the observed subject
    fileStream << log << std::endl;
    fileStream << "============================" << std::endl;
}

std::ostream &operator<<(std::ostream &out, const LogObserver &logObserver) {
    out << "LogObserver: \n\tNbr of observed subjects: " << logObserver.subjects.size() << std::endl;
    return out;
}

LogObserver::~LogObserver() {
    closeLogFileStream();
}
