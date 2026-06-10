#ifndef UNIQUE_PTR_H
#define UNIQUE_PTR_H

#include <utility>

namespace KaKaRot {

template <typename T>
class unique_ptr
{
public:
    unique_ptr() : p(nullptr) {}
    explicit unique_ptr(T* ptr) : p(ptr) {}

    // 禁止拷贝（unique 语义）
    unique_ptr(const unique_ptr&)            = delete;
    unique_ptr& operator=(const unique_ptr&) = delete;

    // 移动构造
    unique_ptr(unique_ptr&& rhs) noexcept : p(rhs.p) { rhs.p = nullptr; }

    // 移动赋值
    unique_ptr& operator=(unique_ptr&& rhs) noexcept
    {
        if (this == &rhs) return *this;
        delete p;
        p = rhs.p;
        rhs.p = nullptr;
        return *this;
    }

    ~unique_ptr() { delete p; }

    // release：放弃所有权，返回原始指针（调用方负责 delete）
    T* release()
    {
        T* ptr = p;
        p = nullptr;
        return ptr; 
    }

    // reset：释放当前资源，接管新指针（默认 nullptr = 只释放）
    void reset(T* ptr = nullptr)
    {
        delete p;
        p = ptr;
    }

    T& operator*()        { return *p; }
    const T& operator*()  const { return *p; }

    T* operator->()       { return p; }
    const T* operator->() const { return p; }

    T* get()              { return p; }
    const T* get()        const { return p; }

    explicit operator bool() const { return p != nullptr; }

private:
    T* p;
};

// make_unique（C++14 标准库有，这里简单实现单对象版）
template<typename T, typename... Args>
unique_ptr<T> make_unique(Args&&... args)
{
    return unique_ptr<T>(new T(std::forward<Args>(args)...));
}

} // namespace KaKaRot

#endif // UNIQUE_PTR_H
