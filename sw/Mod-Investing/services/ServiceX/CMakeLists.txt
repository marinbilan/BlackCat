
project(ServiceX)

add_library(ServiceX STATIC)

target_include_directories(ServiceX PUBLIC if)
target_include_directories(ServiceX PUBLIC inc)

target_sources(ServiceX PRIVATE
    if/ServiceIf.h

    inc/ServiceX.h

    src/ServiceX.cpp
)

# Add unit test
target_sources(UnitTests PRIVATE unitTest/unitTestSrvX.cpp)
target_link_libraries(UnitTests PRIVATE ServiceX)
