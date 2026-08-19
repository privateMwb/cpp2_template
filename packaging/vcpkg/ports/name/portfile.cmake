vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO privateMwb/cpp-template
    REF v1.0.0
    SHA512 b9fd50669b85bf7dad1363235da6baef22aecef639be8ce222d0aa56abf59ccdae3e55d581c89c23c5286b904923dfac4dd76864e3e213cf7f2938edbc0841d4  # TODO: replace with the real SHA512 of the release tarball above
)

set(VCPKG_PORT_NAME Name)

# Consumers only need the library itself, not the tests, benchmarks,
# regression tools, or examples. regression/ also fetches a third-party
# dependency via FetchContent at configure time, which requires network
# access that vcpkg's build sandbox does not allow.
vcpkg_cmake_configure(
    SOURCE_PATH "${SOURCE_PATH}"
    OPTIONS
        -DBUILD_TESTS=OFF
        -DBUILD_BENCHMARKS=OFF
        -DBUILD_REGRESSION=OFF
        -DBUILD_EXAMPLES=OFF
)

vcpkg_cmake_install()

vcpkg_cmake_config_fixup(
    PACKAGE_NAME ${VCPKG_PORT_NAME}
    CONFIG_PATH lib/cmake/${VCPKG_PORT_NAME}
)

# Header-only: there's nothing in debug/ worth keeping — no compiled
# binaries, just a duplicate copy of the same headers. Remove it
# entirely rather than just debug/include.
# NOTE: if this library ever grows compiled sources, switch this back
# to removing only debug/include and keep the real debug binaries,
# mirroring conanfile.py's own header-library/library toggle.
file(
    REMOVE_RECURSE
    "${CURRENT_PACKAGES_DIR}/debug"
)

vcpkg_install_copyright(
    FILE_LIST "${SOURCE_PATH}/LICENSE"
)