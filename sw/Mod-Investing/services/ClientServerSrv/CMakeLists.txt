
project(ClientServerSrv)

add_library(ClientServerSrv STATIC)

target_include_directories(ClientServerSrv PUBLIC if)
target_include_directories(ClientServerSrv PUBLIC inc)

target_sources(ClientServerSrv PRIVATE
    if/ClientServerSrvIf.h

    inc/ClientServerSrv.h

    src/ClientServerSrv.cpp
)

target_link_libraries(ClientServerSrv PRIVATE Factory)
