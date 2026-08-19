// clang-format off
#include <gtest/gtest.h>

#include <atomic>                 // std::atomic
#include <thread>                 // std::thread
#include <vector>                 // std::vector
// clang-format on

// Concurrent reads and writes from multiple threads, and correctness
// under simultaneous access.
// TODO: replace with a real shared instance exercised from multiple
// threads, then assert the final state is what's expected. This
// placeholder uses std::atomic to stay well-defined — don't swap in a
// plain int once real logic goes here without adding real
// synchronization.
TEST(Concurrency, ConcurrentAccess) {
    std::vector<std::thread> threads;
    std::atomic<int> counter = 0;

    for (int i = 0; i < 4; ++i)
        threads.emplace_back([&counter]() { ++counter; });

    for (auto& t : threads)
        t.join();

    EXPECT_EQ(counter, 4);
}
