#ifndef CONTROLHANDLER_HPP
#define CONTROLHANDLER_HPP


#include "message.hpp"

class ControlHandler
{
public:
    ControlHandler(int fd);
    ~ControlHandler();

    void handleControlMessage();
};

#endif // CONTROLHANDLER_HPP
