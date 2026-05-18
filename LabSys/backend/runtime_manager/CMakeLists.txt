project(runtime_manager)

find_package(cnats REQUIRED)

labsys_add_module(runtime_manager
    main.cpp
    src/RuntimeManager.cpp
)

# NATS C client for control-plane IPC
target_link_libraries(runtime_manager PRIVATE cnats::nats_static)
