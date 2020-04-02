from conans import ConanFile, CMake


class JumpyThievesWasm(ConanFile):
    settings = {"os": ["Emscripten"]}
    requires = ["entt/3.3.2",
                "glm/0.9.8.5@bincrafters/stable",
                "gsl_microsoft/2.0.0@bincrafters/stable",
                "spdlog/1.5.0",
                "stb/20180214@conan/stable",
                "imgui/1.75"]
    generators = ["cmake"]

    def _configure_cmake(self):
        cmake = CMake(self)
        cmake.configure()
        return cmake

    def build(self):
        cmake = self._configure_cmake()
        cmake.build()
