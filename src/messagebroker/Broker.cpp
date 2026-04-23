#include "Broker.hpp"

Broker::Broker()
{
    controlFd = createControlPipe();
    if (controlFd == -1)
    {
        perror(("open: " + CONTROL_PIPE).c_str());
        exit(EXIT_FAILURE);
    }

    registerFd = createRegisterPipe();
    if (registerFd == -1)
    {
        perror(("open: " + REGISTER_PIPE).c_str());
        exit(EXIT_FAILURE);
    }

    loggerFd = openLoggerPipe();
    if (loggerFd == -1)
    {
        perror(("open: " + LOGGER_PIPE).c_str());
        std::cerr << "errno: " << errno << std::endl;
        exit(EXIT_FAILURE);
    }

    running = true;
}

Broker::~Broker()
{
    for (auto& p : producers)
    {
        close(p.dataFd);
        close(p.ctrlFd);
        unlink(p.dataPipe.c_str());
        unlink(p.ctrlPipe.c_str());
    }
    for (auto& c : consumers)
    {
        close(c.dataFd);
        close(c.ctrlFd);
        unlink(c.dataPipe.c_str());
        unlink(c.ctrlPipe.c_str());
    }
    close(controlFd);
    close(registerFd);
    close(loggerFd);
    unlink(CONTROL_PIPE.c_str());
    unlink(REGISTER_PIPE.c_str());
    unlink(LOGGER_PIPE.c_str());
}


void Broker::work()
{
    log("[broker] Working...");
    while (running)
    {
        std::vector<struct pollfd> fds;
        fds.push_back({controlFd, POLLIN, 0});
        fds.push_back({registerFd, POLLIN, 0});
        for (size_t i = 0; i < producers.size(); ++i)
        {
            fds.push_back({producers[i].dataFd, POLLIN, 0});
        }

        int result = poll(fds.data(), fds.size(), -1);
        if (result == -1)
        {
            perror("poll");
            break;
        }

        if (fds[0].revents & POLLIN)
        {
            handleControlMessage();
        }
        
        if (fds[1].revents & POLLIN)
        {
            handleRegistration();
        }

        for (size_t i = 2; i < fds.size(); i++)
        {
            if (fds[i].revents & POLLIN)
            {
                pushToConsumers("Message from producer " + producers[i-2].id);
            }
        }
    }
    log("[broker] stopped");
}

void Broker::createPipe(const std::string& path)
{
    unlink(path.c_str());
    if (mkfifo(path.c_str(), 0666) == -1)
    {
        perror(("mkfifo: " + path).c_str());
        exit(EXIT_FAILURE);
    }
}

int Broker::createControlPipe()
{
    createPipe(CONTROL_PIPE);
    return open(CONTROL_PIPE.c_str(),  O_RDONLY | O_NONBLOCK);
}

int Broker::createRegisterPipe()
{
    createPipe(REGISTER_PIPE);
    return open(REGISTER_PIPE.c_str(),  O_RDWR | O_NONBLOCK);
}

int Broker::openLoggerPipe()
{
    return open(LOGGER_PIPE.c_str(),  O_WRONLY | O_NONBLOCK);
}

void Broker::handleControlMessage()
{
    // send control message to appropriate service
    char buffer[256];
    ssize_t n = read(controlFd, buffer, sizeof(buffer) - 1);
    if (n > 0) {
        buffer[n] = '\0';
        std::string cmd(buffer);
        if (!cmd.empty() && cmd.back() == '\n')
        {
            cmd.pop_back();
        }
        if (cmd == "STOP")
        {
            running = false;
        }
    }
}

void Broker::handleRegistration()
{
    // read registration message and add producer/consumer to list
}

void Broker::pushToConsumers(const std::string& message)
{
    // send data to all consumers
    for (auto& c : consumers)
    {
        write(c.dataFd, message.c_str(), message.size());
    }
}

void Broker::log(const std::string& message)
{
    int result = write(loggerFd, message.c_str(), message.size());
    if (result == -1)
    {
        perror("write");
    }
}