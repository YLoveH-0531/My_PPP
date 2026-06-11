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

/* =============================================================================
 * REVIEW NOTES (by Claude, 2026-06-11) —— 上一版已修：构造拷贝元素 / finish 更新 /
 * cap=new_s+count / deallocate 用 capacity / destroy 拼写 / 命名空间。剩下如下：
 *
 * 🔴 必修 BUG
 *  1) at(n)：判断写成 `n > size()`，应为 `n >= size()`。
 *     合法下标是 0..size()-1；ASan 实测 at(size()) 不抛异常反而越界读。
 *     （两个重载都要改）
 *  2) at 抛 std::invalid_argument，std::vector 的约定是 std::out_of_range。
 *
 * 🟡 可移植性 / 进阶
 *  3) 有状态 allocator 的顺序错（copy=、move=）：现在先 `alloc = rhs.alloc` 再
 *     deallocate 旧内存 —— 旧内存是“旧 alloc”分配的，却用“新 alloc”释放。
 *     stateless 的 std::allocator 没事，但有状态分配器会错。
 *     正确：先用旧 alloc 释放旧内存，再赋值 alloc。
 *  4) std::allocator::construct/destroy 在 C++17 已废弃、C++20 移除。
 *     现在 -std=c++17 能过，换 C++20 编译失败。
 *     可移植写法：std::allocator_traits<A>::construct/destroy(alloc, p, ...)
 *     （或直接用你自己的 KaKaRot::Allocator）。
 *  5) reserve 无异常安全：uninitialized_copy 途中若元素拷贝/移动抛异常，
 *     new_s 泄漏、旧数据已被搬走。进阶可用 move_if_noexcept + try/catch。
 *
 * 🟢 设计提升
 *  6) move= 过度复杂（destroy+deallocate 后又 swap 又置 null，swap 是多余的）。
 *     最简写法：直接和 rhs 交换全部成员，让 rhs 的析构去清理旧数据 —— 一行 swap 即可，
 *     不用手动 destroy/deallocate。
 *  7) emplace_back 扩容条件：特判 `size()==0` 略别扭，真正条件是 `capacity()==0`。
 *     建议合并：if (size()==capacity()) reserve(capacity() ? 2*capacity() : 8);
 *  8) 构造方式不一致：emplace_back 用 placement new，resize 用 alloc.construct。
 *     统一成一种（建议都走 allocator_traits / alloc.construct）。
 *  9) 缺常用成员：push_back / pop_back / clear / empty() / front() / back() /
 *     data() / cbegin()/cend()。
 * 10) 析构早退条件 `if(!start||!finish||!cap)` 过严，三者同生同灭，`if(!start) return;` 即可。
 * 11) 拷贝构造分配的是 rhs.capacity()（保留容量）；std::vector 通常只分配 size（收缩）。
 *     设计取舍，非 bug。
 *
 * 优先级：先 1（必崩）→ 4（换 C++20 就挂）→ 6/7（简化）。
 * ===========================================================================*/

template <typename T, typename A = std::allocator<T>>
class vector
{
public:
    vector() : start(nullptr), finish(nullptr), cap(nullptr) {}
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
        alloc = rhs.alloc;
        if (capacity() < rhs.size()) { reserve(rhs.capacity());}
        for (T* p = start; p < finish; p++) {
            alloc.destroy(p);
        }
        std::uninitialized_copy(rhs.begin(), rhs.end(), start);
        finish = start + rhs.size();
        return *this;   
    }
    vector(vector&& rhs) noexcept : alloc(rhs.alloc), start(rhs.start), 
    finish(rhs.finish), cap(rhs.cap) 
    { 
        rhs.start = nullptr;  
        rhs.finish = nullptr;  
        rhs.cap = nullptr;  
    }
    vector& operator=(vector&& rhs) noexcept
    {
        if (this == &rhs) { return *this; }
        alloc = rhs.alloc;
        for (T* p = start; p < finish; p++) { alloc.destroy(p); }
        alloc.deallocate(start, capacity());
        std::swap(start, rhs.start);
        std::swap(finish, rhs.finish);
        std::swap(cap, rhs.cap);
        rhs.start = nullptr;  
        rhs.finish = nullptr;  
        rhs.cap = nullptr; 
        return *this;
    }
    ~vector()
    {
        if (!start || !finish || !cap) { return; }
        for (T* p = start; p < finish; p++) { alloc.destroy(p); }
        alloc.deallocate(start, capacity());
        start = nullptr;  
        finish = nullptr;  
        cap = nullptr; 
    }
    void reserve(std::size_t count)
    {
        if(count <= capacity()) return;
        T* new_s = alloc.allocate(count);
        std::uninitialized_copy(std::make_move_iterator(begin()), std::make_move_iterator(end()), new_s);
        for (T* p = start; p < finish; p++) { alloc.destroy(p); }
        finish = new_s + size();
        cap = new_s + count;
        std::swap(new_s, start);
        alloc.deallocate(new_s, capacity());
    }
    void resize(std::size_t n, const T& value = T{})
    {
        reserve(n);
        for (std::size_t i = size(); i < n; i++) { alloc.construct(start + i, value); }
        for (std::size_t i = n; i < size(); i++) { alloc.destroy(start + i); }
        finish  = start + n;
    }
    T& operator[](std::size_t n) { return *(start + n); }
    const T& operator[](std::size_t n) const { return *(start + n); }
    T& at(std::size_t n) 
    {
        if (n > size()) { throw std::invalid_argument("out of range!"); }
        return (*this)[n];
    }
    const T& at(std::size_t n) const 
    {
        if (n > size()) { throw std::invalid_argument("out of range!"); }
        return (*this)[n];
    }
    template<typename... Args>
    void emplace_back(Args&&... args) 
    {
        if (size() == 0) {
            reserve(8);        
        }else if (size() >= capacity()) {
            reserve(2 * capacity());
        }
        new(finish) T(std::forward<Args>(args)...);
        finish++;
    }
    std::size_t size() const { return finish - start; }
    std::size_t capacity() const { return cap - start; }
    T* begin() { return start; }
    const T* begin() const { return start; }
    T* end() { return finish; }
    const T* end() const { return finish; }
private:
    A  alloc;
    T* start;
    T* finish;
    T* cap;
};


}
#endif //VECTOR_H