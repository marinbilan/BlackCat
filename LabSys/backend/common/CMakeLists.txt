project(LabSysCommon)

# Collect all implementation sources under common/
# NOTE: registry/src/ is intentionally excluded — the Channel Registry and
# Device Registry are now owned by the Runtime Manager and exposed cross-process
# via NATS (labsys.registry.*).  Device modules must NOT use the old in-process
# ChannelRegistry singleton; they register/look up channels via NATS requests.
file(GLOB_RECURSE COMMON_SOURCES
    channel/src/*.cpp
    factory/src/*.cpp
    log/src/*.cpp
)

add_library(LabSysCommon STATIC ${COMMON_SOURCES})

# Public headers: every module that links LabSysCommon gets these include paths
target_include_directories(LabSysCommon PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/channel/if
    ${CMAKE_CURRENT_SOURCE_DIR}/channel/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/factory/if
    ${CMAKE_CURRENT_SOURCE_DIR}/factory/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/log/inc
)

# External deps (resolved by Conan CMakeDeps generator)
find_package(nlohmann_json REQUIRED)

target_link_libraries(LabSysCommon PUBLIC
    nlohmann_json::nlohmann_json
)
