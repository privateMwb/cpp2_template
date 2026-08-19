// clang-format off
#include "support/framework.h"  // REGISTER_TEST_SUITE(), RUN(), CHK()

#include <thread>                 // std::thread
#include <atomic>                 // std::atomic
#include <vector>                 // std::vector
// clang-format on

namespace {

// Concurrent reads and writes from multiple threads, and correctness
// under simultaneous access.
void concurrent_access() {
    // TODO: replace with a real shared instance exercised from multiple
    // threads, then assert the final state is what's expected. This
    // placeholder uses std::atomic to stay well-defined — don't swap in
    // a plain int once real logic goes here without adding real
    // synchronization.
    std::vector<std::thread> threads;
    std::atomic<int> counter = 0;

    for (int i = 0; i < 4; ++i)
        threads.emplace_back([&counter]() { ++counter; });

    for (auto& t : threads)
        t.join();

    CHK(counter == 4);
}

void run_tests() {
    RUN(concurrent_access);
}

}  // namespace

REGISTER_TEST_SUITE()
