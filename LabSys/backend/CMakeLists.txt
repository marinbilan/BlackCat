cmake_minimum_required(VERSION 3.22)
project(LabSys VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Conan-generated toolchain is loaded via cmake preset or -DCMAKE_TOOLCHAIN_FILE
# Run: conan install . --output-folder=build --build=missing
#      cmake -B build -DCMAKE_TOOLCHAIN_FILE=build/conan_toolchain.cmake

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(LabSysModule)

# Output all binaries to bins/
set(BINS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/bins")

# ── Shared infrastructure ──────────────────────────────────────────────────────
add_subdirectory(common)

# ── Runtime Manager (orchestrator process) ────────────────────────────────────
add_subdirectory(runtime_manager)

# ── Device Modules (one binary per device type) ───────────────────────────────
add_subdirectory(modules/signal_gen)
add_subdirectory(modules/pid_regulator)
