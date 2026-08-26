# ── Retargeting this port for a new library ─────────────────────
# CMAKE_PROJECT_NAME must match project()'s name in the repo's root
# CMakeLists.txt exactly, casing included.
#
# GITHUB_REPO_NAME must match the actual GitHub repository name.
# ──────────────────────────────────────────────────────────────

set(CMAKE_PROJECT_NAME ProjName)
set(GITHUB_REPO_NAME cpp2_template)

# ---------------------------------------------------------------------
# GitHub authentication
# ---------------------------------------------------------------------

if(DEFINED ENV{GITHUB_TOKEN})
    set(GITHUB_AUTH_TOKEN "$ENV{GITHUB_TOKEN}")
else()
    set(GITHUB_AUTH_TOKEN "")
endif()

# ---------------------------------------------------------------------
# Main source
# ---------------------------------------------------------------------

vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO privateMwb/${GITHUB_REPO_NAME}
    REF 3d051c726b7af9f716b0afdd371682be63479a54
    SHA512 8ed246af33f890ebee9b8bddab533ac0c6ec049703f64aba2d7c194a838191aa849eee936697fe4ade8a73c65abef9826ef9c37b5b555cd844ce7890f685f93d
    AUTHORIZATION_TOKEN ${GITHUB_AUTH_TOKEN}
)

# ---------------------------------------------------------------------
# Optional internal submodules
# ---------------------------------------------------------------------
#
# GitHub source archives do not contain git submodule contents.
#
# This template currently has no submodules, so this list is empty.
# Add:
#
# "RepoName|commit-sha|sha512"
#
# when the project actually gains a submodule.
# ---------------------------------------------------------------------

set(SUBMODULE_SPECS
    # "JsonPro|<commit-sha>|<sha512>"
)

foreach(SPEC ${SUBMODULE_SPECS})

    string(REPLACE "|" ";" SPEC_PARTS "${SPEC}")

    list(GET SPEC_PARTS 0 SUBMODULE_NAME)
    list(GET SPEC_PARTS 1 SUBMODULE_REF)
    list(GET SPEC_PARTS 2 SUBMODULE_SHA512)

    file(
        REMOVE_RECURSE
        "${SOURCE_PATH}/libs/internal/${SUBMODULE_NAME}"
    )

    vcpkg_from_github(
        OUT_SOURCE_PATH SUBMODULE_SOURCE_PATH
        REPO privateMwb/${SUBMODULE_NAME}
        REF ${SUBMODULE_REF}
        SHA512 ${SUBMODULE_SHA512}
        AUTHORIZATION_TOKEN ${GITHUB_AUTH_TOKEN}
    )

    file(
        RENAME
        "${SUBMODULE_SOURCE_PATH}"
        "${SOURCE_PATH}/libs/internal/${SUBMODULE_NAME}"
    )

endforeach()

# ---------------------------------------------------------------------
# Configure
# ---------------------------------------------------------------------

vcpkg_cmake_configure(
    SOURCE_PATH "${SOURCE_PATH}"
    OPTIONS
        -DBUILD_TESTS=OFF
        -DBUILD_BENCHMARKS=OFF
        -DBUILD_REGRESSION=OFF
        -DBUILD_EXAMPLES=OFF
)

# ---------------------------------------------------------------------
# Build + install
# ---------------------------------------------------------------------

vcpkg_cmake_install()

# ---------------------------------------------------------------------
# CMake package configuration
# ---------------------------------------------------------------------
#
# The template can produce either:
#
#   1. STATIC library
#   2. INTERFACE/header-only library
#
# vcpkg_cmake_config_fixup() expects a debug configuration tree.
# For an empty/header-only template there may be no debug config.
#
# Only perform the fixup when the debug configuration actually exists.
# ---------------------------------------------------------------------

set(DEBUG_CMAKE_CONFIG_PATH
    "${CURRENT_PACKAGES_DIR}/debug/lib/cmake/${CMAKE_PROJECT_NAME}"
)

if(EXISTS "${DEBUG_CMAKE_CONFIG_PATH}")

    message(STATUS
        "Debug CMake configuration found; "
        "running vcpkg_cmake_config_fixup()."
    )

    vcpkg_cmake_config_fixup(
        PACKAGE_NAME ${CMAKE_PROJECT_NAME}
        CONFIG_PATH lib/cmake/${CMAKE_PROJECT_NAME}
    )

else()

    message(STATUS
        "No debug CMake configuration found; "
        "skipping vcpkg_cmake_config_fixup()."
    )

endif()

# ---------------------------------------------------------------------
# Headers
# ---------------------------------------------------------------------

# Debug builds do not need another copy of the headers.
file(
    REMOVE_RECURSE
    "${CURRENT_PACKAGES_DIR}/debug/include"
)

# ---------------------------------------------------------------------
# License
# ---------------------------------------------------------------------

vcpkg_install_copyright(
    FILE_LIST "${SOURCE_PATH}/LICENSE"
)