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
# This template currently has no submodules.
# Add entries here when the project gains submodules:
#
# "RepoName|commit-sha|sha512"
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
# Upstream installs:
#
#   lib/cmake/ProjName/
#   debug/lib/cmake/ProjName/
#
# vcpkg requires:
#
#   share/projname/
#
# vcpkg_cmake_config_fixup() merges the Debug/Release configuration
# files and moves them to the correct vcpkg location.
# ---------------------------------------------------------------------

vcpkg_cmake_config_fixup(
    PACKAGE_NAME ${CMAKE_PROJECT_NAME}
    CONFIG_PATH lib/cmake/${CMAKE_PROJECT_NAME}
)

# ---------------------------------------------------------------------
# Headers
# ---------------------------------------------------------------------

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