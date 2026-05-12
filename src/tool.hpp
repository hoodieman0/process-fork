#ifndef TOOL_HPP
#define TOOL_HPP

#include <cstdint>

namespace tool
{
    size_t writeUint32(unsigned char* buffer, const std::size_t offset, const uint32_t value)
    {
        buffer[offset] = static_cast<unsigned char>(value & 0xFF);
        buffer[offset + 1] = static_cast<unsigned char>((value >> 8)  & 0xFF);
        buffer[offset + 2] = static_cast<unsigned char>((value >> 16) & 0xFF);
        buffer[offset + 3] = static_cast<unsigned char>((value >> 24) & 0xFF);
        return offset + 4;
    }
} // namespace tool

#endif // TOOL_HPP