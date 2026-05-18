// modules/pid_regulator/main.cpp
// Entry point for the PID Regulator module process.
// Usage: mod_pid [--nats-url nats://host:4222]

#include "PidMaster.h"
#include <string>

int main(int argc, char* argv[])
{
    std::string natsUrl = "nats://localhost:4222";
    for (int i = 1; i < argc - 1; ++i) {
        if (std::string(argv[i]) == "--nats-url")
            natsUrl = argv[i + 1];
    }

    LabSys::Modules::Pid::PidMaster master(natsUrl, "mod_pid");
    master.run();
    return 0;
}
