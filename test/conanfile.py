from conan import ConanFile
from conan.tools.cmake import cmake_layout

class Recipe(ConanFile):
    settings   = ["os", "compiler", "build_type", "arch"]
    generators = ["CMakeToolchain", "CMakeDeps"]
    requires   = ["boost-ext-ut/1.1.9", "fmt/11.0.2"]

    def layout(self):
        cmake_layout(self)
