project(mod_pid)

find_package(cnats REQUIRED)

labsys_add_module(mod_pid
    main.cpp
    master/src/PidMaster.cpp
    devices/src/PidBase.cpp
)

target_include_directories(mod_pid PRIVATE
    master/if
    master/inc
    devices/if
    devices/inc
)

# NATS C client for control-plane IPC
target_link_libraries(mod_pid PRIVATE cnats::nats_static)
