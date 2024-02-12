
project(BrokerSrv)

add_library(BrokerSrv STATIC)

target_include_directories(BrokerSrv PUBLIC if)
target_include_directories(BrokerSrv PUBLIC inc)

target_sources(BrokerSrv PRIVATE
    if/BrokerSrvIf.h

    inc/BrokerSrv.h

    src/BrokerSrv.cpp
)

target_link_libraries(BrokerSrv PUBLIC MasterSrv ClientServerSrv)
target_link_libraries(BrokerSrv PRIVATE common Factory Container)
