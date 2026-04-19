#include <iostream>
#include <fcntl.h>
#include <poll.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fstream>

void createPipe(const std::string& path)
{
    unlink(path.c_str());
    if (mkfifo(path.c_str(), 0666) == -1)
    {
        perror(("mkfifo: " + path).c_str());
        exit(EXIT_FAILURE);
    }
}

int main()
{
    std::cout << "[logger] Startup" << std::endl;
    const std::string LOG_FILE = "./process.log";
    const std::string LOGGER_PIPE = "/tmp/logger-pipe";
    std::ofstream logFile(LOG_FILE, std::ios::app);
    if (!logFile.is_open())
    {
        std::cerr << "[logger] Failed to open log file: " << LOG_FILE << std::endl;
        return EXIT_FAILURE;
    }

    createPipe(LOGGER_PIPE);
    int loggerFd = open(LOGGER_PIPE.c_str(), O_RDONLY | O_NONBLOCK);

    while (true)
    {
        char buffer[256];
        ssize_t bytesRead = read(loggerFd, buffer, sizeof(buffer) - 1);
        if (bytesRead > 0)
        {
            buffer[bytesRead] = '\0';
            logFile << "[logger] Received: " << buffer << std::endl;
            logFile.flush();
        }
        else if (bytesRead == -1 && errno != EAGAIN)
        {
            perror("read");
            break;
        }
    }

    std::cout << "[logger] Finished" << std::endl;
    return EXIT_SUCCESS;
}
