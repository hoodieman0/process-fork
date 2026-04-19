#include <iostream>

#include "Broker.hpp"

int main()
{
    std::cout << "[broker] Startup" << std::endl;
    Broker broker;
    broker.work();
    std::cout << "[broker] Finished" << std::endl;
    return EXIT_SUCCESS;
}
