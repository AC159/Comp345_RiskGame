
#ifndef COMP345RISKGAME_LOGGINGOBSERVER_H
#define COMP345RISKGAME_LOGGINGOBSERVER_H

#include <iostream>
#include <vector>
#include <fstream>

// Interface that all observable objects (i.e. subjects) will need to implement
class ILoggable {
public:
    virtual std::string stringToLog() const = 0;
};


// Interface that all concrete observers will need to implement to respond to state changes from the observed subject
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


// Concrete observer class
class LogObserver : public Observer {
private:
    std::fstream fileStream; // reference to log file
    void openLogFileStream();
    void closeLogFileStream();
public:
    LogObserver();
    LogObserver(const LogObserver &logObserver);
    friend std::ostream& operator<<(std::ostream &out, const LogObserver &logObserver);
    LogObserver& operator=(const LogObserver &logObserver);
    void update(const ILoggable &loggable) override;
    ~LogObserver() override;
};

#endif //COMP345RISKGAME_LOGGINGOBSERVER_H
