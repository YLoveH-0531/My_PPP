#ifndef COUNTED_PTR_H
#define COUNTED_PTR_H

#include <cstddef>
#include <utility>

template <typename T>
class counted_ptr
{
public:
    counted_ptr() : p(nullptr), count(nullptr) {}
    counted_ptr(std::nullptr_t) : counted_ptr() {}
    explicit counted_ptr(T* ptr) : p(ptr), count(ptr ? new size_t(1) : nullptr) {}
    counted_ptr(const counted_ptr& rhs) : p(rhs.p) , count(rhs.count) 
    {
        if (count) { ++*count; } 
    }
    counted_ptr& operator=(const counted_ptr& rhs)
    {
        if (this == &rhs) { return *this; }
        Free();
        p = rhs.p;
        count = rhs.count;
        if (count) { ++*count; }
        return *this;
    }
    counted_ptr(counted_ptr&& rhs) noexcept : p(rhs.p), count(rhs.count)
    {
        rhs.p = nullptr;
        rhs.count = nullptr;
    }
    counted_ptr& operator=(counted_ptr&& rhs) noexcept
    {
        if (this == &rhs) { return *this; }
        Free();
        p = rhs.p;
        count = rhs.count;
        rhs.p = nullptr;
        rhs.count = nullptr;
        return *this;
    }
    ~counted_ptr()
    {
        Free();
    }
    explicit operator bool() const { return p != nullptr; }
    T& operator*() { return *p; }
    const T& operator*() const { return *p; }
    T* operator->() { return p; }
    const T* operator->() const { return p; }
    T* get() { return p; }
    const T* get() const { return p; }
    void reset(T* ptr = nullptr)
    {
        Free();
        p = ptr;
        count = ptr ? new size_t(1) : nullptr;

    }
    size_t use_count() const { return count ? *count : 0; }
    bool operator==(const counted_ptr& rhs) const { return p == rhs.p; }
    bool operator!=(const counted_ptr& rhs) const { return !(*this == rhs); }
    bool operator==(std::nullptr_t) const { return p == nullptr; }
    bool operator!=(std::nullptr_t) const { return p != nullptr; }
private:
     T*      p;
size_t*  count;
    void Free()
    {
        if (count && --*count == 0) {
            delete p;
            delete count;
        }
        p = nullptr;
        count = nullptr;
    }
};


template <typename T, typename... Args>
counted_ptr<T> make_counted(Args&&... args)
{
    return counted_ptr<T>(new T(std::forward<Args>(args)...));
}
#endif //COUNTED_PTR_H
