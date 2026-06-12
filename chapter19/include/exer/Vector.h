#ifndef VECTOR_H
#define VECTOR_H

#include <cstddef>
#include <initializer_list>
#include <memory>
#include <algorithm>
#include <iterator>
#include <stdexcept>
#include <utility>

namespace KaKaRot{

template <typename T, typename A = std::allocator<T>>
class vector
{
    // ───────────────────────────────────────────────────────────────────────
    // [#1 标准兼容性标注] 本类全程使用 alloc.construct(...) / alloc.destroy(...)，
    //   这两个成员函数在 C++17 起被【弃用】、在 C++20 中已被【删除】。
    //   若要用 -std=c++20 及以上编译，需将所有此类调用统一替换为：
    //       std::allocator_traits<A>::construct(alloc, p, args...)
    //       std::allocator_traits<A>::destroy(alloc, p)
    //   当前实现仅适用于 C++11/14/17。下文每个 construct/destroy 调用均受此影响。
    // ───────────────────────────────────────────────────────────────────────

    // [#2 RAII 回滚守卫] 持有一块裸缓冲区，并记录已成功构造的元素个数。
    //   若批量构造途中抛出异常而未调用 release()，析构时会自动析构已构造的
    //   元素并释放缓冲区，从而保证不泄漏（强异常安全）。构造全部成功后调用
    //   release() 提交所有权，析构便不再回滚。
    struct AllocGuard
    {
        A&          a;
        T*          buf;
        std::size_t cap_n;
        std::size_t built;
        bool        committed;
        AllocGuard(A& a_, std::size_t n)
            : a(a_), buf(a_.allocate(n)), cap_n(n), built(0), committed(false) {}
        void add()     { ++built; }
        void release() { committed = true; }
        ~AllocGuard()
        {
            if (committed) { return; }
            for (std::size_t i = 0; i < built; i++) { a.destroy(buf + i); }
            a.deallocate(buf, cap_n);
        }
        AllocGuard(const AllocGuard&)            = delete;
        AllocGuard& operator=(const AllocGuard&) = delete;
    };

public:
    // constructors and copy-control
    vector() : start(nullptr), finish(nullptr), cap(nullptr) {}
    explicit vector(std::size_t count, const T& value = T{}) : start(nullptr), finish(nullptr), cap(nullptr)
    {
        if (count == 0) { return; }
        AllocGuard g(alloc, count);                                 // 构造途中抛异常会自动回滚
        for (std::size_t i = 0; i < count; i++) { alloc.construct(g.buf + i, value); g.add(); }
        g.release();                                                // 全部成功，提交所有权
        start  = g.buf;
        finish = g.buf + count;
        cap    = g.buf + count;
    }
    vector(std::initializer_list<T> il) : start(il.size() ? alloc.allocate(il.size()) : nullptr),
                                          finish(start ? start + il.size() : nullptr),
                                          cap(start ? start + il.size() : nullptr) 
                                          { std::uninitialized_copy(il.begin(), il.end(), start); }
    vector(const vector& rhs) : alloc(rhs.alloc), 
                                start(rhs.start ? alloc.allocate(rhs.capacity()) : nullptr), 
                                finish(start ? start + rhs.size() : nullptr), 
                                cap(start ? start + rhs.capacity() : nullptr) 
                                { std::uninitialized_copy(rhs.begin(), rhs.end(), start); }
    vector& operator=(const vector& rhs)
    {
        if (this == &rhs) { return *this; }
        if (capacity() < rhs.size()) { reserve(rhs.capacity());}
        for (T* p = start; p < finish; p++) {
            alloc.destroy(p);
        }
        std::uninitialized_copy(rhs.begin(), rhs.end(), start);
        finish = start + rhs.size();
        return *this;   
    }
    vector(vector&& rhs) noexcept : alloc(std::move(rhs.alloc)), start(rhs.start), 
    finish(rhs.finish), cap(rhs.cap) 
    { 
        rhs.start = nullptr;  
        rhs.finish = nullptr;  
        rhs.cap = nullptr;  
    }
    vector& operator=(vector&& rhs) noexcept
    {
        if (this == &rhs) { return *this; }
        for (T* p = start; p < finish; p++) { alloc.destroy(p); }
        if (start) { alloc.deallocate(start, capacity()); }
        start = finish = cap = nullptr; 
        swap(rhs);
        return *this;
    }
    ~vector()
    {
        if (!start) { return; }
        for (T* p = start; p < finish; p++) { alloc.destroy(p); }
        alloc.deallocate(start, capacity());
    }

    //Element access
    T& operator[](std::size_t n) { return *(start + n); }
    const T& operator[](std::size_t n) const { return *(start + n); }
    T& at(std::size_t n) 
    {
        if (n >= size()) { throw std::out_of_range("out of range!"); }
        return (*this)[n];
    }
    const T& at(std::size_t n) const 
    {
        if (n >= size()) { throw std::out_of_range("out of range!"); }
        return (*this)[n];
    }
    // [#4 前置条件] front()/back() 要求容器【非空】。对空容器调用是未定义行为
    //   （与 std::vector 一致，出于性能不做检查；调用方需自行保证 !empty()）。
    T& front() { return *start; }
    const T& front() const { return *start; }
    T& back() { return *(finish - 1); }
    const T& back() const { return *(finish - 1); }
    T* data() noexcept { return start; }
    const T* data() const noexcept { return start; }

    //Modifiers
    void push_back(const T& value)
    {
        if (size() == capacity()) {
            T value_copy = value;
            reserve(capacity() ? 2 * capacity() : 8);
            alloc.construct(finish++, std::move(value_copy));
            return ;
        }
        alloc.construct(finish++, value);
    }
    void push_back(T&& value)
    {
        if (size() == capacity()) { reserve(capacity() ? 2 * capacity() : 8); }
        alloc.construct(finish++, std::move(value));
    }
    template<typename... Args>
    void emplace_back(Args&&... args) 
    {
        if (size() == capacity()) { reserve(capacity() ? 2 *capacity() : 8); }
        alloc.construct(finish++, std::forward<Args>(args)...);
    }
    template<typename... Args>
    T* emplace(T* pos, Args&&... args)
    {
        std::size_t offset = pos - start;
        if (pos < start || pos > finish) { throw std::out_of_range("out of range!");}
        if (size() == capacity()) { reserve(capacity() ? 2 *capacity() : 8); }
        pos = start + offset;
        if (pos == finish) {
            alloc.construct(finish, std::forward<Args>(args)...);
        }else{
            alloc.construct(start + size(), std::move(*(start + size() -1)));
            for (std::size_t i = size() - 1; i > offset; i--) { start[i] = std::move(start[i - 1]); }
            *pos = T(std::forward<Args>(args)...);
        }
        finish++;
        return pos;
    }
    void pop_back()
    {
        if (size() > 0) {
            alloc.destroy(--finish);        
        }
    }
    void swap(vector& rhs) noexcept
    {
        using std::swap;
        swap(alloc, rhs.alloc);
        swap(start, rhs.start);
        swap(finish, rhs.finish);
        swap(cap, rhs.cap);
    }
    void resize(std::size_t n, const T& value = T{})
    {
        reserve(n);
        for (std::size_t i = size(); i < n; i++) { alloc.construct(start + i, value); }
        for (std::size_t i = n; i < size(); i++) { alloc.destroy(start + i); }
        finish  = start + n;
    }
    void clear()
    {
        for (T* p = start; p < finish; p++) { alloc.destroy(p); }
        finish = start;
    }

    //Capacity
    std::size_t size() const noexcept { return finish - start; }
    std::size_t capacity() const noexcept { return cap - start; }
    bool empty() const noexcept { return size() == 0; }
    void reserve(std::size_t count)
    {
        if (count <= capacity()) { return; }
        // [#7] 先把旧状态存入局部变量，后续指针切换不再依赖 "此刻 start/cap 仍是旧值" 的隐式时序
        T* const          old_start = start;
        const std::size_t old_size  = size();
        const std::size_t old_cap   = capacity();

        // [#2] 搬移途中若某次 construct 抛异常，g 析构时自动回滚已构造元素并释放 new_s
        AllocGuard g(alloc, count);
        for (std::size_t i = 0; i < old_size; i++) {
            alloc.construct(g.buf + i, std::move_if_noexcept(old_start[i]));
            g.add();
        }
        g.release();                                            // 搬移全部成功，提交所有权

        for (std::size_t i = 0; i < old_size; i++) { alloc.destroy(old_start + i); }
        if (old_start) { alloc.deallocate(old_start, old_cap); }
        start  = g.buf;
        finish = g.buf + old_size;
        cap    = g.buf + count;
    }

    // iterator
    T* begin() noexcept { return start; }
    const T* begin() const noexcept { return start; }
    T* end() noexcept { return finish; }
    const T* end() const noexcept { return finish; }
    const T* cbegin() const noexcept { return start; }
    const T* cend() const noexcept { return finish; } 
    std::reverse_iterator<T*>       rbegin()        { return  std::reverse_iterator<T*>(finish); }
    std::reverse_iterator<const T*> rbegin() const  { return  std::reverse_iterator<const T*>(finish); }
    std::reverse_iterator<T*>       rend()          { return std::reverse_iterator<T*>(start); }
    std::reverse_iterator<const T*> rend() const    { return std::reverse_iterator<const T*>(start); }
    std::reverse_iterator<const T*> crbegin() const { return  std::reverse_iterator<const T*>(finish); }
    std::reverse_iterator<const T*> crend() const   { return std::reverse_iterator<const T*>(start); }

private:
    A  alloc;
    T* start;
    T* finish;
    T* cap;
};

}
#endif //VECTOR_H