#ifndef MESSAGEBUILDER_HPP
#define MESSAGEBUILDER_HPP

#include <cstring>
#include <limits>

#include "Tool.hpp"
#include "message.hpp"

class MessageBuilder {
public:
    static size_t dataMessageSize(const std::string& data) noexcept {
        // signature + producer uid + frame number + message length + message + null terminator
        return 4 + 1 + 4 + 4 + data.size() + 1; 
    }
    
    static size_t requestRegisterMessageSize(const std::string& id) noexcept {
        // signature + type + id length + id + null terminator
        return 4 + 1 + 4 + id.size() + 1;
    }

    static size_t confirmRegisterMessageSize() noexcept {
        // signature + ACK + uid + null terminator
        return 4 + 1 + 1 + 1;
    }

    static size_t failedRegisterMessageSize() noexcept {
        // signature + NACK + null terminator
        return 4 + 1 + 1;
    }

    static size_t buildDataMessage(const Producer& producer, const uint32_t frame, const std::string& data, unsigned char* buffer, const size_t& bufferSize, const size_t& startIndex) noexcept;
    static size_t buildRequestRegisterMessage(const std::string& id, bool isProducer, unsigned char* buffer, const size_t& bufferSize, const size_t& startIndex) noexcept;
    static size_t buildConfirmRegisterMessage(const UID uid, unsigned char* buffer, const size_t& bufferSize, const size_t& startIndex) noexcept;
    static size_t buildFailedRegisterMessage(unsigned char* buffer, const size_t& bufferSize, const size_t& startIndex) noexcept;
};

#endif // MESSAGEBUILDER_HPP
