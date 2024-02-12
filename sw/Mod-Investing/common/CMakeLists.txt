
add_library(common STATIC)

target_include_directories(common PUBLIC inc)

target_sources(common PRIVATE
    inc/CommonTypes.h
    inc/Database.h
    inc/Log.h

    src/Database.cpp
    src/Log.cpp
)

target_link_libraries(common PRIVATE Factory)


add_executable(App-1)

target_sources(App-1 PRIVATE mainMod-App-1.cpp)

target_link_libraries(App-1 PRIVATE common BrokerSrv)

if (NOT WIN32)
    install(
        TARGETS App-1
        RUNTIME
                DESTINATION ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_BINDIR}
                COMPONENT App-1
    )
endif()
