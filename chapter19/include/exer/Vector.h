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
public:
    // constructors and copy-control
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
        if (capacity() < rhs.size()) { reserve(rhs.capacity());}
        for (T* p = start; p < finish; p++) {
            alloc.destroy(p);
        }
        std::uninitialized_copy(rhs.begin(), rhs.end(), start);
        finish = start + rhs.size();
        alloc = rhs.alloc;
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
        for (T* p = start; p < finish; p++) { alloc.destroy(p); }
        alloc.deallocate(start, capacity());
        start = finish = cap = nullptr; 
        swap(rhs);
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
    T& front() { return *start; }
    const T& front() const { return *start; }
    T& back() { return *(finish - 1); }
    const T& back() const { return *(finish - 1); }
    T* data() { return start; }
    const T* data() const { return start; }

    //Modifiers
    void push_back(const T& value)
    {
        reserve(size() + 1);
        alloc.construct(finish++, value);
    }
    void push_back(T&& value)
    {
        reserve(size() + 1);
        alloc.construct(finish++, value);
    }
    template<typename... Args>
    void emplace_back(Args&&... args) 
    {
        if (size() == capacity()) {
            reserve(capacity() ? 2 *capacity() : 8);        
        }
        alloc.construct(finish++, std::forward<Args>(args)...);
    }
    template<typename... Args>
    T* emplace(T* pos, Args... args)
    {
        if (pos < start || pos >= finish) {
            throw std::out_of_range("out of range!");
        }
        if (size() == capacity()) {
            reserve(capacity() ? 2 *capacity() : 8);                
        }
        T* p = finish;
        for (int i = size(); i > pos; i--) { p[i] = p[i -1]; }
        alloc.allocate(pos, std::forward<Args>(args)...);
        return pos;
    }
    void pop_back()
    {
        if (size() > 0) {
            alloc.destroy(finish--, size());        
        }
    }
    void swap(vector& rhs)
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
        for (T* p = start; p < finish; p++) { alloc.destroy(p, size()); }
        finish = start;
    }

    //Capacity
    std::size_t size() const { return finish - start; }
    std::size_t capacity() const { return cap - start; }
    bool empty() const { return size() == 0; }
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

    // iterator
    T* begin() { return start; }
    const T* begin() const { return start; }
    T* end() { return finish; }
    const T* end() const { return finish; }
    const T* cbegin() const { return start; }
    const T* cend() const { return finish; } 
    T* rbegin() { return finish - 1; }
    const T* rbegin() const { return finish - 1; }
    T* rend() { return start - 1; }
    const T* rend() const { return start - 1; }
    const T* crbegin() const { return finish - 1; }
    const T* crend() const { return start - 1; }

private:
    A  alloc;
    T* start;
    T* finish;
    T* cap;
};

}
#endif //VECTOR_H