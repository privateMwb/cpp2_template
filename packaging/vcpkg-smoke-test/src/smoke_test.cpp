// Minimal smoke test: confirms the vcpkg-installed package's headers
// are reachable and the library links, by constructing the core type.
//
// Same scope as the Conan test_package smoke test — proves the
// package is installable and linkable, not that its behavior is
// correct. Expand this to parse a small literal and check the result
// once filled in for a real project.
// TODO: replace with the library's real public header

#include <iostream>

int main() {
    // TODO: replace with a real construction of the library's core type.
    std::cout << "Library linked and constructed successfully.\n";
    return 0;
}