#pragma once
#include <string>
#include <cstddef>

namespace LabSys::Channel {

// ChannelIf — pure virtual interface for all channel types.
// Currently targeting SPSC (Single Producer Single Consumer) ring buffers.
// Multi-producer or pub/sub variants will implement this same interface.
class ChannelIf
{
public:
    virtual ~ChannelIf() = default;

    virtual const std::string& name()  const = 0;
    virtual std::size_t        size()  const = 0;  // capacity in samples
    virtual bool               empty() const = 0;
    virtual bool               full()  const = 0;

    // Returns false if full (non-blocking)
    virtual bool write(const void* data, std::size_t bytes) = 0;
    // Returns false if empty (non-blocking)
    virtual bool read(void* data,        std::size_t bytes) = 0;
};

} // namespace LabSys::Channel
