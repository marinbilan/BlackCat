
project(Mod-Investing)

# Create UnitTests executable target
add_executable(UnitTests)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

target_link_libraries(UnitTests PRIVATE gmock gmock_main)

include(GoogleTest)
gtest_discover_tests(UnitTests)


add_subdirectory(common)
add_subdirectory(services)
