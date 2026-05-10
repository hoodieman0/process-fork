#include "MessageBuilder.hpp"

size_t MessageBuilder::buildDataMessage(const Producer& producer, const uint32_t frame, const std::string& data, unsigned char* buffer, const size_t& bufferSize, const size_t& startIndex) noexcept{
    
    if (data.size() > std::numeric_limits<uint32_t>::max()) {
        return 0; // data too large
    }

    size_t requiredSize = dataMessageSize(data);
    if (buffer == nullptr || bufferSize < requiredSize) {
        return 0;
    }
    
    size_t offset = startIndex;
    std::memcpy(buffer + offset, MESSAGE_SIGNATURE, 4);
    offset += 4;

    buffer[offset] = std::to_integer<unsigned char>(producer.uid);
    offset += 1;

    offset = tool::writeUint32(buffer, offset, frame);

    offset = tool::writeUint32(buffer, offset, static_cast<uint32_t>(data.size()));
    
    std::memcpy(buffer + offset, data.data(), data.size());
    offset += data.size();
    buffer[offset] = '\0';
    offset += 1;

    return requiredSize;
}

size_t MessageBuilder::buildRequestRegisterMessage(const std::string& id, bool isProducer, unsigned char* buffer, const size_t& bufferSize, const size_t& startIndex) {
    if (id.size() > std::numeric_limits<uint32_t>::max()) {
        return 0; // id too large
    }

    size_t requiredSize = requestRegisterMessageSize(id);
    if (buffer == nullptr || bufferSize < requiredSize) {
        return 0;
    }

    size_t offset = startIndex;
    std::memcpy(buffer + offset, MESSAGE_SIGNATURE, 4);
    offset += 4;
    buffer[offset] = isProducer ? static_cast<unsigned char>(RegisterType::Producer) : static_cast<unsigned char>(RegisterType::Consumer);
    offset += 1;
    offset = tool::writeUint32(buffer, offset, static_cast<uint32_t>(id.size()));
    std::memcpy(buffer + offset, id.data(), id.size());
    offset += id.size();
    buffer[offset] = '\0';
    offset += 1;
    return requiredSize;
}

size_t MessageBuilder::buildConfirmRegisterMessage(const UID uid, unsigned char* buffer, const size_t& bufferSize, const size_t& startIndex) {
    size_t requiredSize = confirmRegisterMessageSize();
    
    if (buffer == nullptr || bufferSize < requiredSize) {
        return 0;
    }

    size_t offset = startIndex;
    std::memcpy(buffer + offset, MESSAGE_SIGNATURE, 4);
    offset += 4;
    buffer[offset] = static_cast<unsigned char>(RegisterResponse::ACK);
    offset += 1;
    buffer[offset] = std::to_integer<unsigned char>(uid);
    offset += 1;
    buffer[offset] = '\0';
    offset += 1;
    return requiredSize;
}

size_t MessageBuilder::buildFailedRegisterMessage(unsigned char* buffer, const size_t& bufferSize, const size_t& startIndex) noexcept 
{
    size_t requiredSize = failedRegisterMessageSize();
    if (buffer == nullptr || bufferSize < requiredSize) 
    {
        return 0;
    }

    size_t offset = startIndex;
    std::memcpy(buffer + offset, MESSAGE_SIGNATURE, 4);
    offset += 4;
    buffer[offset] = static_cast<unsigned char>(RegisterResponse::NACK);
    offset += 1;
    buffer[offset] = '\0';
    offset += 1;
    return requiredSize;
}
