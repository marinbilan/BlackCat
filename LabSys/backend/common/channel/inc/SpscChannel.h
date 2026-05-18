#pragma once
#include "ChannelIf.h"
#include <vector>
#include <atomic>
#include <string>

namespace LabSys::Channel {

// SpscChannel — lock-free Single Producer / Single Consumer ring buffer.
// Uses shared memory (later: POSIX shm_open) for inter-process communication.
// For now backed by a plain std::vector for intra-process use.
class SpscChannel : public ChannelIf
{
public:
    SpscChannel(std::string name, std::size_t capacityBytes);
    ~SpscChannel() override = default;

    const std::string& name()  const override;
    std::size_t        size()  const override;
    bool               empty() const override;
    bool               full()  const override;

    bool write(const void* data, std::size_t bytes) override;
    bool read(void*        data, std::size_t bytes) override;

private:
    std::string              m_name;
    std::size_t              m_capacity;
    std::vector<std::byte>   m_buffer;

    alignas(64) std::atomic<std::size_t> m_head{0};  // producer writes here
    alignas(64) std::atomic<std::size_t> m_tail{0};  // consumer reads here
};

} // namespace LabSys::Channel
