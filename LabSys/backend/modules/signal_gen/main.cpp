// modules/signal_gen/main.cpp
// Entry point for the Signal Generator module process.
// Usage: mod_signal_gen [--nats-url nats://host:4222]

#include "SignalGenMaster.h"
#include <string>

int main(int argc, char* argv[])
{
    std::string natsUrl = "nats://localhost:4222";
    for (int i = 1; i < argc - 1; ++i) {
        if (std::string(argv[i]) == "--nats-url")
            natsUrl = argv[i + 1];
    }

    LabSys::Modules::SignalGen::SignalGenMaster master(natsUrl, "mod_signal_gen");
    master.run();
    return 0;
}
