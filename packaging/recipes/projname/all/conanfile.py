from conan import ConanFile
from conan.tools.build import check_min_cppstd
from conan.tools.cmake import CMake, CMakeDeps, CMakeToolchain, cmake_layout
from conan.tools.files import collect_libs, copy
from conan.tools.scm import Git
import os

class Conan(ConanFile):
    # ── Retargeting this recipe for a new library ───────────────────
    # Edit these fields (and the module-level dnspro above) — everything
    # below derives from them. Version is handled by a separate script,
    # not edited here.
    name = "projname"
    cmake_name = "ProjName"  # matches project()'s dnspro in the top-level CMakeLists.txt
    version = "1.0.0"

    url = "https://github.com/privateMwb/cpp2_template"
    description = "TODO: one-line description of what this library does."
    topics = (
        "TODO",
        "replace-with-real-topics",
    )
    # ──────────────────────────────────────────────────────────────

    # Compiled library: Parser.cpp/Builder.cpp/ZoneStore.cpp/Resolver.cpp
    # under src/ produce real object code, so this is a static or shared
    # library depending on the "shared" option below, not an INTERFACE
    # target -- package_id() and cpp_info.libs must account for that.
    package_type = "library"

    license = "MIT"
    author = "privateMwb"

    settings = "os", "compiler", "build_type", "arch"

    options = {
        "shared": [True, False],
        "fPIC": [True, False],
    }

    default_options = {
        "shared": False,
        "fPIC": True,
    }

    exports_sources = (
        "CMakeLists.txt",
        "cmake/*",
        "include/*",
        "src/*",
    )

    def config_options(self):
        if self.settings.os == "Windows":
            self.options.rm_safe("fPIC")

    def configure(self):
        if self.options.shared:
            self.options.rm_safe("fPIC")

    def layout(self):
        cmake_layout(self)

    def validate(self):
        check_min_cppstd(self, 23)

    def source(self):
        sources = self.conan_data["sources"][self.version]
        git = Git(self)
        git.run("init .")
        git.run(f"remote add origin {sources['url']}")
        git.run(f"fetch --depth 1 origin {sources['commit']}")
        git.run("checkout FETCH_HEAD")
        git.run("submodule sync --recursive")
        git.run("submodule update --init --recursive")

    def generate(self):
        deps = CMakeDeps(self)
        deps.generate()

        tc = CMakeToolchain(self)
        tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure(
            variables={
                "BUILD_TESTS": "OFF",
                "BUILD_BENCHMARKS": "OFF",
                "BUILD_REGRESSION": "OFF",
                "BUILD_EXAMPLES": "OFF",
            }
        )
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()

        copy(
            self,
            "LICENSE",
            src=self.source_folder,
            dst=os.path.join(self.package_folder, "licenses"),
        )

    def package_info(self):
        self.cpp_info.set_property("cmake_file_name", self.cmake_name)
        self.cpp_info.set_property("cmake_target_name", f"{self.cmake_name}::{self.cmake_name}")
        self.cpp_info.libs = collect_libs(self)