#ifndef BROKER_HPP
#define BROKER_HPP

#include <string>
#include <vector>
#include <fcntl.h>
#include <poll.h>
#include <unistd.h>
#include <sys/stat.h>

#include <iostream>
#include <cstring>

struct Producer
{
    std::string id;
    std::string dataPipe;
    std::string ctrlPipe;
    int dataFd;
    int ctrlFd;
};

struct Consumer
{
    std::string id;
    std::string dataPipe;
    std::string ctrlPipe;
    int dataFd;
    int ctrlFd;
};

class Broker
{
public:
    Broker();
    ~Broker();

    void work();

private:
    const std::string CONTROL_PIPE = "/tmp/broker-control-pipe";
    const std::string REGISTER_PIPE = "/tmp/broker-register-pipe";
    const std::string LOGGER_PIPE = "/tmp/logger-pipe";

    int controlFd;
    int registerFd;
    int loggerFd;

    std::vector<Producer> producers; // poll producers
    std::vector<Consumer> consumers; // send to consumers

    bool running;

    void createPipe(const std::string& path);
    int createControlPipe();
    int createRegisterPipe();
    int openLoggerPipe();

    void handleControlMessage();
    void handleRegistration();
    void pushToConsumers(const std::string& message);

    void log(const std::string& message);
};

#endif // BROKER_HPP
