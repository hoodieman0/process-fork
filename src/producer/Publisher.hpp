#ifndef COMMS_HPP
#define COMMS_HPP

#include <string>
#include <cstdint>
#include <unistd.h>

#include "MessageBuilder.hpp"
#include "message.hpp"


namespace producer
{
    namespace
    {
        const std::size_t MAX_BUFFER_SIZE = 256U;
    }

    class Publisher 
    {
    public:
        Publisher(Producer& producer, int fd);
        ~Publisher() = default;

        void sendInt32(const int32_t value) const;

    private:
        void send(unsigned char* buffer, size_t amount) const;

        Producer& producer;
        int fd;
        uint32_t frameNumber = 0;
        unsigned char buffer[MAX_BUFFER_SIZE];
    };
}

#endif // COMMS_HPP