/**
 * @file test_new_delete.cpp
 * @author KaKaRot
 * @brief 
 * @version 0.1
 * @date 2026-10-19
 * 
 * @copyright Copyright (c) 2026
 */

#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <new>

static int g_live = 0;
struct Pool { alignas(std::max_align_t) unsigned char buf[64]; };
struct Tag {};
struct Fail {};
struct Widget { Widget() { throw Fail{}; } };
struct Placed {
    Placed() { throw Fail{}; }
    static void* operator new(std::size_t, void* p) {
        std::printf("  Placed::operator new(size_t, void*)\n");
        return p;
    }
    static void operator delete(void*, void*) noexcept {
        std::printf("  Placed::operator delete(void*, void*)\n");
    }
};

// ordinary version
void* operator new(std::size_t n) {
    void* p = std::malloc(n);
    if (!p) throw std::bad_alloc{};
    ++g_live;
    std::printf("  operator new(%zu)\n", n);
    return p;
}

void operator delete(void* p) noexcept {
    --g_live;
    std::printf("  operator delete(void*)\n");
    std::free(p);
}

void operator delete(void* p, std::size_t n) noexcept {
    --g_live;
    std::printf("  operator delete(void*, %zu)\n", n);
    std::free(p);
}

// nothrow version
void* operator new(std::size_t n, const std::nothrow_t&) noexcept {
    void* p = std::malloc(n);
    if (!p) return nullptr;
    ++g_live;
    std::printf("  operator new(%zu, nothrow)\n", n);
    return p;
}

void operator delete(void* p, const std::nothrow_t&) noexcept {
    --g_live;
    std::printf("  operator delete(void*, nothrow)\n");
    std::free(p);
}

// pool version: no malloc/free, live unchanged
void* operator new(std::size_t n, Pool& pool) {
    std::printf("  operator new(%zu, Pool&)\n", n);
    return pool.buf;
}

void operator delete(void*, Pool&) noexcept {
    std::printf("  operator delete(void*, Pool&)\n");
}

// tag version: deliberately no matching delete
void* operator new(std::size_t n, Tag) {
    void* p = std::malloc(n);
    if (!p) throw std::bad_alloc{};
    ++g_live;
    std::printf("  operator new(%zu, Tag)\n", n);
    return p;
}

template <class F>
void run(const char* label, F f) {
    std::printf("%s\n", label);
    try {
        f();
    } catch (const Fail&) {
        std::printf("  caught Fail\n");
    }
    std::printf("  live blocks: %d\n", g_live);
}

int main() {
    Pool pool;
    alignas(Widget) unsigned char buffer[sizeof(Widget)];

    run("new Widget", [] { new Widget; });
    run("new (std::nothrow) Widget", [] { new (std::nothrow) Widget; });
    run("new (pool) Widget", [&] { new (pool) Widget; });
    run("new (buffer) Widget", [&] { new (buffer) Widget; });
    run("new (buffer) Placed", [&] { new (buffer) Placed; });
    run("new (Tag{}) Widget", [] { new (Tag{}) Widget; });
    return 0;
}
