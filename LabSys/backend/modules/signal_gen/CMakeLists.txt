project(mod_signal_gen)

find_package(cnats REQUIRED)

labsys_add_module(mod_signal_gen
    main.cpp
    master/src/SignalGenMaster.cpp
    devices/src/SgBase.cpp
    devices/src/SgVendorA.cpp
)

target_include_directories(mod_signal_gen PRIVATE
    master/if
    master/inc
    devices/if
    devices/inc
)

# NATS C client for control-plane IPC
target_link_libraries(mod_signal_gen PRIVATE cnats::nats_static)
