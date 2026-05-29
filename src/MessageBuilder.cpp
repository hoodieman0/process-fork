#include "MessageBuilder.hpp"

size_t MessageBuilder::buildDataMessage(const message::Producer& producer, const uint32_t frame, const std::string& data, unsigned char* buffer, const size_t& bufferSize, const size_t& startIndex) noexcept{
    
    if (data.size() > std::numeric_limits<uint32_t>::max()) {
        return 0; // data too large
    }

    size_t requiredSize = dataMessageSize(data);
    if (buffer == nullptr || bufferSize < requiredSize) {
        return 0;
    }
    
    size_t offset = startIndex;
    std::memcpy(buffer + offset, message::MESSAGE_SIGNATURE, 4);
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
    std::memcpy(buffer + offset, message::MESSAGE_SIGNATURE, 4);
    offset += 4;
    buffer[offset] = isProducer ? static_cast<unsigned char>(message::RegisterType::Producer) : static_cast<unsigned char>(message::RegisterType::Consumer);
    offset += 1;
    offset = tool::writeUint32(buffer, offset, static_cast<uint32_t>(id.size()));
    std::memcpy(buffer + offset, id.data(), id.size());
    offset += id.size();
    buffer[offset] = '\0';
    offset += 1;
    return requiredSize;
}

size_t MessageBuilder::buildConfirmRegisterMessage(const message::UID uid, unsigned char* buffer, const size_t& bufferSize, const size_t& startIndex) {
    size_t requiredSize = confirmRegisterMessageSize();
    
    if (buffer == nullptr || bufferSize < requiredSize) {
        return 0;
    }

    size_t offset = startIndex;
    std::memcpy(buffer + offset, message::MESSAGE_SIGNATURE, 4);
    offset += 4;
    buffer[offset] = static_cast<unsigned char>(message::RegisterResponse::ACK);
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
    std::memcpy(buffer + offset, message::MESSAGE_SIGNATURE, 4);
    offset += 4;
    buffer[offset] = static_cast<unsigned char>(message::RegisterResponse::NACK);
    offset += 1;
    buffer[offset] = '\0';
    offset += 1;
    return requiredSize;
}

bool MessageBuilder::isValidMessage(const unsigned char* const buffer, const size_t count, const size_t offset)
{
    if (count < 6)
    {
        return false;
    }
    if (buffer == nullptr)
    {
        return false;
    }
    const unsigned char* arr = buffer + offset;

    int result = std::memcmp(arr, message::MESSAGE_SIGNATURE, 4);
    if (result != 0)
    {
        return false;
    }

    arr = arr + 4;

    for (size_t i = 0; i < count; i++)
    {
        if (*arr == '\0')
        {
            return true;
        }
        arr += 1;
    }

    return false;
}
