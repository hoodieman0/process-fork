#include "Registrar.hpp"

bool registrar::producer::reserve(const std::string& id, message::Producer& producer)
{
    int fd = open(REGISTER_PIPE.c_str(), O_RDWR | O_NONBLOCK);
    if (fd == -1) 
    {
        perror("Failed to open register pipe");
    }

    unsigned char buffer[256];
    size_t size = MessageBuilder::requestRegisterMessageSize(id);
    size_t written = MessageBuilder::buildRequestRegisterMessage(id, true, buffer, size, 0);

    if (written == 0) 
    {
        close(fd);
        return false;
    }

    int result = write(fd, buffer, written);
    if (result == -1)
    {
        perror("Failed to write to register pipe");
        close(fd);
        exit(EXIT_FAILURE);
    }

    struct pollfd fds[] = {{fd, POLLIN, 0}};
    result = poll(fds, 1, TIMEOUT_MS);
    if (result == -1)
    {
        perror("poll");
        close(fd);
        return false;
    }

    else if (result == 0)
    {
        fprintf(stderr, "Timeout waiting for register response\n");
        close(fd);
        return false;
    }

    int result = read(fd, buffer, 256);
    if (result == -1)
    {
        perror("read");
        close(fd);
        return false;
    }

    if (!MessageBuilder::isValidMessage(buffer, result, 0))
    {
        close(fd);
        return false;
    }


    producer.uid = static_cast<message::UID>(rand() % 256);
    producer.id = id;
    return true;
}

bool registrar::consumer::reserve(const std::string& id, message::Consumer& consumer)
{
    consumer.uid = static_cast<message::UID>(rand() % 256);
    consumer.id = id;
    return true;
}