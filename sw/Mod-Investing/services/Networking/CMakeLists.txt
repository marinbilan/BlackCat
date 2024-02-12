
project(Networking)

add_library(Networking STATIC)

target_include_directories(Networking PUBLIC if)
target_include_directories(Networking PUBLIC inc)

target_sources(Networking PRIVATE
    if/NetworkingIf.h

    inc/HTTPSClient.h

    src/HTTPSClient.cpp
)

find_package(Boost 1.59 REQUIRED)

# TODO: this is clearly not desirable
target_link_libraries(Networking PUBLIC Boost::boost)

set(OPENSSL_USE_STATIC_LIBS TRUE)
find_package(OpenSSL REQUIRED)

target_link_libraries(Networking PRIVATE OpenSSL::SSL OpenSSL::Crypto)

# Add unit test
target_sources(UnitTests PRIVATE unitTest/unitTestNetworking.cpp)
target_link_libraries(UnitTests PRIVATE Networking)
