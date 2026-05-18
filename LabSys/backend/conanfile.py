from conan import ConanFile


class LabSys(ConanFile):
    name = "labsys"
    version = "0.1.0"
    settings = "os", "compiler", "build_type", "arch"
    # CMakeDeps  → generates Find<Pkg>.cmake / <Pkg>Config.cmake files
    # CMakeToolchain → generates conan_toolchain.cmake consumed by root CMake
    generators = "CMakeDeps", "CMakeToolchain"

    def requirements(self):
        # NATS C client — control plane IPC (lifecycle events, registry requests)
        self.requires("cnats/3.8.2")
        # Modern JSON — config parsing
        self.requires("nlohmann_json/3.11.3")
        # Unit testing
        self.requires("gtest/1.14.0")
