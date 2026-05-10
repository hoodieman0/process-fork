#ifndef REIGSTRAR_HPP
#define REIGSTRAR_HPP

#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>

#include "message.hpp"
#include "MessageBuilder.hpp"

namespace registrar
{
    const std::string CONTROL_PIPE = "/tmp/broker-control-pipe";
    const std::string LOGGER_PIPE = "/tmp/logger-pipe";

    namespace 
    {
        const std::string REGISTER_PIPE = "/tmp/broker-register-pipe";
        const int TIMEOUT_MS = 1000;
    }

    namespace producer
    {
        bool reserve(const std::string& id, message::Producer& producer); 
    };

    namespace consumer
    {
        bool reserve(const std::string& id, message::Consumer& consumer); 
    };
}

#endif // REIGSTRAR_HPP
