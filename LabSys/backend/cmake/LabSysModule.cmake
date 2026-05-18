# LabSysModule.cmake
# Helper macro: labsys_add_module(target_name source1 source2 ...)
#
# Creates an executable, links LabSysCommon, sets include dirs, and
# places the binary in backend/bins/.

macro(labsys_add_module name)
    add_executable(${name} ${ARGN})

    target_link_libraries(${name} PRIVATE LabSysCommon)

    # Each module auto-gets its own if/ inc/ src/ on the include path
    target_include_directories(${name} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/if
        ${CMAKE_CURRENT_SOURCE_DIR}/inc
        ${CMAKE_CURRENT_SOURCE_DIR}/src
    )

    # All binaries land in backend/bins/
    set_target_properties(${name} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${BINS_DIR}"
    )
endmacro()


# labsys_add_unittest(test_target_name source1 ...)
# Creates a GTest binary linked against LabSysCommon + GTest.
macro(labsys_add_unittest name)
    add_executable(${name} ${ARGN})

    target_link_libraries(${name} PRIVATE
        LabSysCommon
        GTest::gtest_main
        GTest::gmock
    )

    target_include_directories(${name} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../if
        ${CMAKE_CURRENT_SOURCE_DIR}/../inc
    )

    set_target_properties(${name} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${BINS_DIR}/tests"
    )

    # Register with CTest so `ctest` picks it up automatically
    include(GoogleTest)
    gtest_discover_tests(${name})
endmacro()
