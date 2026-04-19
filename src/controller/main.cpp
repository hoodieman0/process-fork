#include <iostream>
#include <fcntl.h>
#include <unistd.h>

int main()
{
    std::cout << "[controller] Startup" << std::endl;
    const std::string CONTROL_PIPE = "/tmp/broker-control-pipe";
    int controlFd = open(CONTROL_PIPE.c_str(),  O_WRONLY | O_NONBLOCK);
    if (controlFd == -1) {
        perror("open control pipe");
        return EXIT_FAILURE;
    }

    std::string command = "STOP";
    ssize_t result = write(controlFd, command.c_str(), command.size());
    if (result == -1) {
        perror("write to control pipe");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
