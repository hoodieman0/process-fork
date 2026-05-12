#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <cstdint>
#include <string>

namespace message
{
    using UID = std::byte;

    const char * const MESSAGE_SIGNATURE = "miku";

    struct Producer {
        UID uid;
        std::string id;
    };

    struct Consumer {
        UID uid;
        std::string id;
    };

    struct DataMessage {
        UID producerUid;
        uint32_t frameNumber;
        uint32_t messageLength;
        char* message;
    };

    enum class CommandType : uint8_t {
        Start = 1,
        Stop = 2,
        Restart = 3,
        Status = 4,
        Dump = 5
    };

    enum class RegisterType : uint8_t {
        Producer = 1,
        Consumer = 2
    };

    enum class RegisterResponse : uint8_t {
        ACK = 0x06,
        NACK = 0x25
    };
}

#endif // MESSAGE_HPP
