
#ifndef COMP345RISKGAME_LOGGINGOBSERVER_H
#define COMP345RISKGAME_LOGGINGOBSERVER_H

#include <iostream>
#include <vector>

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
private:
    std::vector<Observer*> observers;
public:
    Subject();
    Subject(const Subject &subject);
    friend std::ostream& operator<<(std::ostream &out, const Subject &subject);
    Subject& operator=(const Subject &subject);
    virtual void attach(Observer *observer);
    virtual void detach(const Observer *observer);
    virtual void notify(const ILoggable &loggable);
    virtual ~Subject();
};


class LogObserver : public Observer {
private:
    std::vector<Subject*> subjects;
public:
    LogObserver();
    LogObserver(const LogObserver &logObserver);
    friend std::ostream& operator<<(std::ostream &out, const LogObserver &logObserver);
    LogObserver& operator=(const LogObserver &logObserver);
    void update(const ILoggable &loggable) override;
    ~LogObserver() override;
};

#endif //COMP345RISKGAME_LOGGINGOBSERVER_H
