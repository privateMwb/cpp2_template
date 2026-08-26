# ── Retargeting this port for a new library ─────────────────────
# CMAKE_PROJECT_NAME must match project()'s name in the repo's root
# CMakeLists.txt exactly, casing included (e.g. "FalconHTTP") -- NOT
# ${PORT}, which vcpkg forces to lowercase-with-hyphens (e.g.
# "falconhttp") and is a different string. GITHUB_REPO_NAME must match
# the real GitHub repo name, casing included -- also not necessarily
# the same as ${PORT}.
set(CMAKE_PROJECT_NAME ProjName)
set(GITHUB_REPO_NAME cpp2_template)
# ──────────────────────────────────────────────────────────────

# Reads GITHUB_TOKEN from the environment if present (set by
# packaging.yml's vcpkg job) so these repos, which are private, can be
# fetched -- vcpkg_from_github's own HTTPS downloader has no other way
# to authenticate. Empty string is fine for public consumers/local
# testing: vcpkg treats an empty AUTHORIZATION_TOKEN as "no auth."
if(DEFINED ENV{GITHUB_TOKEN})
    set(GITHUB_AUTH_TOKEN "$ENV{GITHUB_TOKEN}")
else()
    set(GITHUB_AUTH_TOKEN "")
endif()

vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO privateMwb/${GITHUB_REPO_NAME}
    REF 3d051c726b7af9f716b0afdd371682be63479a54
    SHA512 8ed246af33f890ebee9b8bddab533ac0c6ec049703f64aba2d7c194a838191aa849eee936697fe4ade8a73c65abef9826ef9c37b5b555cd844ce7890f685f93d
    AUTHORIZATION_TOKEN ${GITHUB_AUTH_TOKEN}
)

# GitHub archive tarballs never include submodule content, so this
# project's internal libraries under libs/internal/ are fetched
# separately here, each pinned to the exact commit the submodule
# points at, then copied into place.
#
# Table of name|ref|sha512 -- add a submodule by adding one line here,
# not by copy-pasting a whole vcpkg_from_github() block. The line
# below is a placeholder example; replace with your actual submodules.
set(SUBMODULE_SPECS
    # "SubmoduleName|<commit-sha>|<sha512>"
)

foreach(SPEC ${SUBMODULE_SPECS})
    string(REPLACE "|" ";" SPEC_PARTS "${SPEC}")
    list(GET SPEC_PARTS 0 SUBMODULE_NAME)
    list(GET SPEC_PARTS 1 SUBMODULE_REF)
    list(GET SPEC_PARTS 2 SUBMODULE_SHA512)

    file(REMOVE_RECURSE "${SOURCE_PATH}/libs/internal/${SUBMODULE_NAME}")

    vcpkg_from_github(
        OUT_SOURCE_PATH SUBMODULE_SOURCE_PATH
        REPO privateMwb/${SUBMODULE_NAME}
        REF ${SUBMODULE_REF}
        SHA512 ${SUBMODULE_SHA512}
        AUTHORIZATION_TOKEN ${GITHUB_AUTH_TOKEN}
    )

    file(RENAME "${SUBMODULE_SOURCE_PATH}" "${SOURCE_PATH}/libs/internal/${SUBMODULE_NAME}")
endforeach()

# Consumers only need the library itself, not the tests, benchmarks,
# regression tools, or examples.
vcpkg_cmake_configure(
    SOURCE_PATH "${SOURCE_PATH}"
    OPTIONS
        -DBUILD_TESTS=OFF
        -DBUILD_BENCHMARKS=OFF
        -DBUILD_REGRESSION=OFF
        -DBUILD_EXAMPLES=OFF
)

vcpkg_cmake_install()

# PACKAGE_NAME/CONFIG_PATH must match CMAKE_PROJECT_NAME above, not
# ${PORT}: this points at wherever the library's own root
# CMakeLists.txt called install(EXPORT ... DESTINATION
# lib/cmake/${PROJECT_NAME}), and that PROJECT_NAME keeps whatever
# casing project() used -- vcpkg's forced-lowercase ${PORT} will not
# match it on a case-sensitive filesystem.
vcpkg_cmake_config_fixup(
    PACKAGE_NAME ${CMAKE_PROJECT_NAME}
    CONFIG_PATH lib/cmake/${CMAKE_PROJECT_NAME}
)

# This library is compiled (not header-only), so debug binaries are
# real and must be kept -- only the duplicate debug/include headers
# are removed.
file(
    REMOVE_RECURSE
    "${CURRENT_PACKAGES_DIR}/debug/include"
)

vcpkg_install_copyright(
    FILE_LIST "${SOURCE_PATH}/LICENSE"
)