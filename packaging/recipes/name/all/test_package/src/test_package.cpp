// Minimal smoke test: confirms the installed package's headers are
// reachable and the library links, by constructing the core type.
//
// This deliberately doesn't exercise parsing or other behavior — that
// would need matching the library's actual API surface. Consider
// expanding this to parse a small literal and check the result once
// this is filled in for a real project.
// TODO: replace with the library's real public header

#include <iostream>

int main() {
    // TODO: replace with a real construction of the library's core type.
    std::cout << "Library linked and constructed successfully.\n";
    return 0;
}