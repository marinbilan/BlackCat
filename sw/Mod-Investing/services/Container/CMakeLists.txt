
project(Container)

add_library(Container STATIC)

target_include_directories(Container PUBLIC if)
target_include_directories(Container PUBLIC inc)

target_sources(Container PRIVATE
    if/ContainerIf.h

    inc/ObjectsManager.h

    src/ObjectsManager.cpp
)

target_link_libraries(Container PUBLIC DevSrv ServiceX)
target_link_libraries(Container PRIVATE Factory)
