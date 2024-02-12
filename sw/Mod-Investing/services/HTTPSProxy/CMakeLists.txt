
project(HTTPSProxySrv)

add_library(HTTPSProxySrv STATIC)

target_include_directories(HTTPSProxySrv PUBLIC if)
target_include_directories(HTTPSProxySrv PUBLIC inc)

target_sources(HTTPSProxySrv PRIVATE
    if/HTTPSProxySrvIf.h

    inc/HTTPSProxySrv.h

    src/HTTPSProxySrv.cpp
)

target_link_libraries(HTTPSProxySrv PUBLIC DevSrv)
target_link_libraries(HTTPSProxySrv PRIVATE common Factory Networking)
