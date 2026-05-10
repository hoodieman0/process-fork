#include "Publisher.hpp"

Publisher::Publisher(Producer& producer, int fd)
    : producer(producer), fd(fd)
{

}

void Publisher::send(unsigned char* buffer, size_t amount) const
{
    int result = write(fd, buffer, amount);
    if (result == -1)
    {
        return;
    }
}

void Publisher::sendInt32(const int32_t value) const
{
    std::string data = std::to_string(value);
    size_t size = MessageBuilder::dataMessageSize(data);
    size_t written = MessageBuilder::buildDataMessage(producer, frameNumber, data, buffer, size, 0);
    if (written == 0)
    {
        return;
    }

    send(buffer, size);
    frameNumber++;
}
