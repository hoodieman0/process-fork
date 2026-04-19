#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <iostream>

const char* const PRODUCER_PATH = "./build/producer";
const char* const CONSUMER_PATH = "./build/consumer";
const char* const MESSAGE_BROKER_PATH = "./build/messagebroker";
const char* const CONTROLLER_PATH = "./build/controller";
const char* const LOGGER_PATH = "./build/logger";

int setupMessageBroker();
int setupProducer();
int setupConsumer();
int setupLogger();

int main(void)
{
    pid_t pids[2];


    // Message Broker
    pids[0] = fork();
    if (pids[0] == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    else if (pids[0] == 0) {
        setupLogger();
    }
    else {
        std::cout << "[setup] logger PID: " << (intmax_t) pids[0] << std::endl;
    }

    // Logger
    pids[1] = fork();
    if (pids[1] == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    else if (pids[1] == 0) {
        setupMessageBroker();
    }
    else {
        std::cout << "[setup] broker PID: " << (intmax_t) pids[1] << std::endl;
    }

    int status;
    while (waitpid(-1, &status, 0) > 0); 

    std::cout << "[setup] All child processes have terminated. Exiting." << std::endl;
    return EXIT_SUCCESS;
}
    
int setupMessageBroker()
{
    std::cout << "[setup] Initializing broker" << std::endl;
    int result = execl(MESSAGE_BROKER_PATH, "messagebroker", (char *) NULL);
    if (result == -1) {
        perror("execl");
        exit(EXIT_FAILURE);
    }
    return EXIT_FAILURE; // execl should not return if successful
}

int setupLogger()
{
    std::cout << "[setup] Initializing logger" << std::endl;
    int result = execl(LOGGER_PATH, "logger", (char *) NULL);
    if (result == -1) {
        perror("execl");
        exit(EXIT_FAILURE);
    }
    return EXIT_FAILURE; // execl should not return if successful
}

