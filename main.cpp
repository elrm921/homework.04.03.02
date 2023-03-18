#include <iostream>
#include <fstream>
#include <vector>

class Observer {
public:
    virtual void onWarning(const std::string& message) const {}
    virtual void onError(const std::string& message) const {}
    virtual void onFatalError(const std::string& message) const {}
};

class WarningObserver : public Observer {
public:
    WarningObserver() {}
    void onWarning(const std::string& message) const override {
        std::cout << "Warning: " << message << std::endl;
    }
};

class ErrorObserver : public Observer {
public:
    ErrorObserver(const std::string& path) : log_file(path) {}
    void onError(const std::string& message) const override {
        std::ofstream out;
        out.open(log_file, std::ios_base::app);
        out << "Error: " << message << std::endl;
        out.close();
    }
private:
    std::string log_file;
};

class FatalErrorObserver : public Observer {
public:
    FatalErrorObserver(const std::string& path) : log_file(path) {}
    void onFatalError(const std::string& message) const override {
        std::ofstream out;
        out.open(log_file, std::ios_base::app);
        out << "FatalError: " << message << std::endl;
        std::cout << "FatalError: " << message << std::endl;
        out.close();
    }
private:
    std::string log_file;
};

class Object {
public:
    Object() {}
    void warning(const std::string& message) const { for (auto observer : observers_) { observer->onWarning(message); } }
    void error(const std::string& message) const { for (auto observer : observers_) { observer->onError(message); } }
    void fatalError(const std::string& message) const { for (auto observer : observers_) { observer->onFatalError(message); } }
    void add_observer(const Observer& observer) { observers_.push_back(&observer); }
private:
    std::vector<const Observer*> observers_;
};

int main() {
    auto obj = Object();

    obj.add_observer(WarningObserver());
    obj.add_observer(ErrorObserver("error_log.txt"));
    obj.add_observer(FatalErrorObserver("fatal_error_log.txt"));

    obj.warning("Test Warning");
    obj.error("Test Error");
    obj.fatalError("Test Fatal Error");

    return 0;
}