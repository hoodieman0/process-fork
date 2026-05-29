#include <iostream>
#include <fstream>
#include <unistd.h>

#include "Generator.hpp"
#include "Publisher.hpp"


int main()
{
    std::cout << "[producer] Startup" << std::endl;

    RampingGenerator gen(0, 100, 1, 1);
    int publishFd = open("/tmp/producer-out", O_WRONLY | O_NONBLOCK);
    if (publishFd == -1)
    {
        perror("open");
        return EXIT_FAILURE;
    }

    Publisher publisher(, publishFd);

    for (size_t i = 0; i < 10; i++)
    {
        std::cout << "[producer] Iteration " << i << std::endl;
        int val = gen.advance();
        publisher.sendInt32(val);
        std::cout << "[producer] Generated value: " << val << std::endl;
    }

    return 0;
}
