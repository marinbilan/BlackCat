
project(DevSrv)

add_library(DevSrv STATIC)

target_include_directories(DevSrv PUBLIC if)
target_include_directories(DevSrv PUBLIC inc)

target_sources(DevSrv PRIVATE
    if/InvDevIf.h

    inc/InvDev0.h

    src/InvDev0.cpp
)

#target_link_libraries(DevSrv PUBLIC common)
target_link_libraries(DevSrv PRIVATE Factory HTTPSProxySrv)
