from pathlib import Path

from conan import ConanFile
from conan.errors import ConanInvalidConfiguration
from conan.tools.cmake import CMake, CMakeToolchain, CMakeDeps, cmake_layout
from conan.tools.build import check_min_cppstd

class Voxium(ConanFile):

    name = "Voxium"
    version = "1.0.0"
    label = "Voxel Game Engine"

    # Optional metadata
    license = "<Put the package license here>"
    author = "<Put your name here> <And your email here>"
    url = "<Package recipe repository url here, for issues about the package>"
    description = "<Description of hello package here>"
    topics = ("<Put some tag here>", "<here>", "<and here>")

    settings = "os", "compiler", "build_type", "arch"

    exports_sources = "CMakeLists.txt", "cmake/*", "src/*", "test/*"

    options = {"shared": [True, False], "fPIC": [True, False]}
    default_options = {
        "shared": True,
        "fPIC": True
    }

    requires = [
        "spdlog/1.11.0",
        "nlohmann_json/3.11.3",
        # "vorbis/1.3.7",
        # "opengl/system",
        # "glew/2.2.0",
        # "glfw/3.4",
        "glm/cci.20230113",
        # "entt/3.14.0",
        # "imgui/1.91.5",
        # "openal-soft/1.23.1",
        # "vulkan-loader/1.3.290.0",
        "kainjow-mustache/4.1",
        # "stb/cci.20240531",
        # "joltphysics/5.2.0",
        # "sol2/3.3.1",
        "zlib/1.3.1"
        # "pngpp/0.2.10",
        # "curlpp/0.8.1.cci.20240530"
    ]

    # tool_requires = [
    #     "cmake/[>=3.27.0]",
    #     "ccache/[>=4.8.3]",
    #     "cppcheck/[>=2.12.1]"
    # ]

    # test_requires = [
    #     "gtest/[>=1.14.0]"
    # ]

    def config_options(self):
        if self.settings.os == "Windows":
            del self.options.fPIC

    def configure(self):
        self.options["spdlog"].header_only = True
        # self.options["sol2"].with_lua = "luajit"

    def layout(self):
        cmake_layout(self)

    def validate(self):
        check_min_cppstd(self, "20")

        if self.settings.compiler == "gcc" and self.settings.compiler.version < "9":
            raise ConanInvalidConfiguration("GCC < 9 is not supported.")

    def generate(self):
        tc = CMakeToolchain(self)
        tc.variables["BUILD_SHARED_LIBS"] = self.options.shared
        tc.generate()
        CMakeDeps(self).generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()

    # def package_info(self):
    #     self.cpp_info.libs = [""]
    #     if self.settings.compiler == "Visual Studio":
    #         self.cpp_info.cxxflags.append("/Zc:__cplusplus")
