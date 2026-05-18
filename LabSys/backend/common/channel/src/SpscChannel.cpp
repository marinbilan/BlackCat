#include "SpscChannel.h"
#include <cstring>
#include <stdexcept>

namespace LabSys::Channel {

SpscChannel::SpscChannel(std::string name, std::size_t capacityBytes)
    : m_name(std::move(name))
    , m_capacity(capacityBytes)
    , m_buffer(capacityBytes)
{
    if (capacityBytes == 0)
        throw std::invalid_argument("SpscChannel: capacity must be > 0");
}

const std::string& SpscChannel::name() const { return m_name; }
std::size_t        SpscChannel::size() const { return m_capacity; }

bool SpscChannel::empty() const
{
    return m_head.load(std::memory_order_acquire) ==
           m_tail.load(std::memory_order_acquire);
}

bool SpscChannel::full() const
{
    // TODO: proper ring-buffer full check when variable-length writes are added
    return false;
}

bool SpscChannel::write(const void* data, std::size_t bytes)
{
    // Placeholder — simple overwrite at head for now
    // TODO: implement proper SPSC ring-buffer write
    (void)data; (void)bytes;
    return true;
}

bool SpscChannel::read(void* data, std::size_t bytes)
{
    // Placeholder
    // TODO: implement proper SPSC ring-buffer read
    (void)data; (void)bytes;
    return true;
}

} // namespace LabSys::Channel
