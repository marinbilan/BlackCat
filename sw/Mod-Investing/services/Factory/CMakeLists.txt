
project(Factory)

add_library(Factory INTERFACE)

target_include_directories(Factory INTERFACE if)
target_include_directories(Factory INTERFACE inc)

target_sources(Factory INTERFACE
    if/FactoryIf.h

    inc/Factory.h
    inc/FactoryImpl.h
)

target_link_libraries(Factory INTERFACE common Container MasterSrv DevSrv ClientServerSrv HTTPSProxySrv ServiceX)
