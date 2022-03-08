
#ifndef COMP345RISKGAME_LOGGINGOBSERVER_H
#define COMP345RISKGAME_LOGGINGOBSERVER_H

#include <iostream>
#include <vector>
#include <fstream>

class ILoggable {
public:
    virtual std::string stringToLog() const = 0;
};


class Observer {
public:
    virtual void update(const ILoggable &loggable) = 0;
    virtual ~Observer();
};


class Subject {
    // this class should not be instantiated since it's only goal is to record and notify observers.
    // There is no need to have individual subject objects.
private:
    static std::vector<Observer*> observers;
public:
    Subject();
    Subject(const Subject &subject);
    friend std::ostream& operator<<(std::ostream &out, const Subject &subject);
    Subject& operator=(const Subject &subject);
    static void attach(Observer *observer);
    static void detach(const Observer *observer);
    static void notify(const ILoggable &loggable);
    ~Subject();
};


class LogObserver : public Observer {
private:
    std::vector<Subject*> subjects;
    std::fstream fileStream; // reference to log file
    void openLogFileStream();
    void closeLogFileStream();
public:
    LogObserver();
    LogObserver(const LogObserver &logObserver);
    friend std::ostream& operator<<(std::ostream &out, const LogObserver &logObserver);
    LogObserver& operator=(const LogObserver &logObserver);
    void update(const ILoggable &loggable) override;
    static void LoggingDriver();
    ~LogObserver() override;
};

#endif //COMP345RISKGAME_LOGGINGOBSERVER_H
