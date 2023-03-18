#include <iostream>
#include <fstream>
#include <vector>

class Observer {
public:
    virtual void onWarning(const std::string& message) {}
    virtual void onError(const std::string& message) {}
    virtual void onFatalError(const std::string& message) {}
};

class WarningObserver : public Observer {
public:
    WarningObserver() {}
    void onWarning(const std::string& message) override {
        std::cout << "Warning: " << message << std::endl;
    }
};

class ErrorObserver : public Observer {
public:
    ErrorObserver(const std::string& path) : log_file_(std::ofstream(path)) {}
    void onError(const std::string& message) override {
        log_file_ << "Error: " << message << std::endl;
    }
private:
    std::ofstream log_file_;
};

class FatalErrorObserver : public Observer {
public:
    FatalErrorObserver(const std::string& path) : log_file_(std::ofstream(path)) {}
    void onFatalError(const std::string& message) override {
        log_file_ << "FatalError: " << message << std::endl;
    }
private:
    std::ofstream log_file_;
};

class Object {
public:
    Object() {}
    void warning(const std::string& message) const { for (auto observer : observers_) { observer->onWarning(message); } }
    void error(const std::string& message) const { for (auto observer : observers_) { observer->onError(message); } }
    void fatalError(const std::string& message) const { for (auto observer : observers_) { observer->onFatalError(message); } }
    void add_observer(Observer& observer) { observers_.push_back(&observer); }
private:
    std::vector<Observer*> observers_;
};

int main() {
    auto obj = Object();
    
    auto obs1 = WarningObserver();
    auto obs2 = ErrorObserver("error_log.txt");
    auto obs3 = FatalErrorObserver("fatal_error_log.txt");

    obj.add_observer(obs1);
    obj.add_observer(obs2);
    obj.add_observer(obs3);

    obj.warning("Test Warning");
    obj.error("Test Error");
    obj.fatalError("Test Fatal Error");

    return 0;
}