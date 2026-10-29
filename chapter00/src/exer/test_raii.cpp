/**
 * @file test_raii.cpp
 * @author KaKaRot
 * @brief Example for cpp-knowledge/03-memory-resource/RAII_summary.md: construction failure,
 *        stack unwinding, ownership transfer, lock_guard
 * @version 0.1
 * @date 2026-10-29
 *
 * @copyright Copyright (c) 2026
 */

#include <cstdio>
#include <mutex>
#include <stdexcept>
#include <thread>
#include <utility>

class File {
    std::FILE* f_ = nullptr;
public:
    File(const char* path, const char* mode) : f_(std::fopen(path, mode)) {
        if (!f_) throw std::runtime_error("open failed");   // failure: no object
        std::puts("  File opened");
    }
    ~File() {
        if (f_) { std::fclose(f_); std::puts("  File closed"); }
    }
    File(const File&) = delete;                             // sole owner
    File(File&& o) noexcept : f_(o.f_) { o.f_ = nullptr; }
    File& operator=(File&&) = delete;
    std::FILE* get() const { return f_; }
};

struct TwoFiles {                                           // no special members: members are RAII
    File a_;
    File b_;
    TwoFiles(const char* pa, const char* pb) : a_(pa, "r"), b_(pb, "r") {}
};

void work_that_throws(const File&) { throw std::runtime_error("work failed"); }

std::mutex m;
bool is_locked() {
    bool locked = true;
    // try_lock by the owning thread is undefined, so ask another thread
    std::thread([&] { if (m.try_lock()) { m.unlock(); locked = false; } }).join();
    return locked;
}

int main() {
    std::puts("1) open fails:");
    try { File f("/no/such/file", "r"); }
    catch (const std::exception& e) { std::printf("  caught: %s\n", e.what()); }

    std::puts("2) exception after open:");
    try { File f("/etc/hostname", "r"); work_that_throws(f); }
    catch (const std::exception& e) { std::printf("  caught: %s\n", e.what()); }

    std::puts("3) second member fails:");
    try { TwoFiles t("/etc/hostname", "/no/such/file"); }
    catch (const std::exception& e) { std::printf("  caught: %s\n", e.what()); }

    std::puts("4) move:");
    {
        File a("/etc/hostname", "r");
        File b(std::move(a));
        std::printf("  a.get() == nullptr: %d\n", a.get() == nullptr);
    }

    std::puts("5) lock_guard:");
    try {
        std::lock_guard<std::mutex> g(m);
        std::printf("  inside: locked=%d\n", is_locked());
        throw std::runtime_error("work failed");
    } catch (const std::exception& e) {
        std::printf("  caught: %s, locked=%d\n", e.what(), is_locked());
    }
}

/*
Expected output:

1) open fails:
  caught: open failed
2) exception after open:
  File opened
  File closed
  caught: work failed
3) second member fails:
  File opened
  File closed
  caught: open failed
4) move:
  File opened
  a.get() == nullptr: 1
  File closed
5) lock_guard:
  inside: locked=1
  caught: work failed, locked=0
*/
