
project(MasterSrv)

add_library(MasterSrv STATIC)

target_include_directories(MasterSrv PUBLIC if)
target_include_directories(MasterSrv PUBLIC inc)

target_sources(MasterSrv PRIVATE
    if/MasterSrvIf.h

    inc/MasterSrv.h

    src/MasterSrv.cpp
)

target_link_libraries(MasterSrv PUBLIC Container DevSrv)
target_link_libraries(MasterSrv PRIVATE Factory)
