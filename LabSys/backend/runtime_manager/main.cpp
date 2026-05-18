// runtime_manager/main.cpp
// Entry point for the Runtime Manager process.
// Usage: runtime_manager [--nats-url nats://host:4222]

#include "RuntimeManager.h"
#include <string>

int main(int argc, char* argv[])
{
    std::string natsUrl = "nats://localhost:4222";
    for (int i = 1; i < argc - 1; ++i) {
        if (std::string(argv[i]) == "--nats-url")
            natsUrl = argv[i + 1];
    }

    LabSys::RuntimeManager rm(natsUrl);
    rm.run();
    return 0;
}
