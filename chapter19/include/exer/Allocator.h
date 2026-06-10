#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <cstdlib>
#include <cstddef>
#include <utility>
#include <stdexcept>

namespace KaKaRot {

// -----------------------------------------------------------------------
// Allocator<T>
// 用 malloc/free 实现的简化版 allocator
// 与 std::allocator 的主要差距：
//   1. 缺少 rebind<U>::other
//   2. 缺少 value_type/pointer/size_type 等类型别名
//   3. 缺少 max_size()
//   4. 异常：malloc 失败抛 bad_alloc（标准行为）
// -----------------------------------------------------------------------
template <typename T>
class Allocator
{
public:
    // allocate：只分配 n 个 T 的原始内存，不构造对象
    T* allocate(size_t n)
    {
        T* ptr = static_cast<T*>(malloc(n * sizeof(T)));
        if (!ptr) throw std::bad_alloc{};  // malloc 失败应抛异常，不返回 nullptr
        return ptr;
    }

    // deallocate：只释放内存，不析构对象（析构由 destroy 负责）
    void deallocate(T* p, size_t)   // n 对 free 无意义，直接忽略
    {
        free(p);
    }

    // construct：在已分配的内存 p 上用 placement new 构造对象
    template <typename... Args>
    void construct(T* p, Args&&... args)
    {
        new(p) T(std::forward<Args>(args)...);  // fix: 原来漏了 <Args>
    }

    // destroy：只析构对象，不释放内存
    void destroy(T* p)
    {
        p->~T();
    }
    // Allocator 是无状态的（stateless）：
    // 不持有任何成员变量，不负责追踪分配了哪些内存
    // 那是 vector 自己的职责
};

// -----------------------------------------------------------------------
// vector<T, A>
// PPP §19.4 末尾的简化版 vector，使用上面的 Allocator
// -----------------------------------------------------------------------
template<typename T, typename A = Allocator<T>>
struct vector_base {
    A  alloc;
    T* elem;
    size_t sz;
    size_t space;

    vector_base(const A& a, size_t n)
        : alloc(a), elem(n ? alloc.allocate(n) : nullptr), sz(0), space(n) {}

    // 移动构造：把资源偷过来，把源置空，避免 swap 后 double-free
    vector_base(vector_base&& other) noexcept
        : alloc(other.alloc), elem(other.elem), sz(other.sz), space(other.space)
    {
        other.elem  = nullptr;
        other.sz    = 0;
        other.space = 0;
    }

    vector_base& operator=(vector_base&& other) noexcept
    {
        if (this == &other) return *this;
        if (elem) alloc.deallocate(elem, space);
        alloc = other.alloc;
        elem  = other.elem;
        sz    = other.sz;
        space = other.space;
        other.elem  = nullptr;
        other.sz    = 0;
        other.space = 0;
        return *this;
    }

    ~vector_base()
    {
        if (elem) alloc.deallocate(elem, space);
    }
};

template<typename T, typename A = Allocator<T>>
class vector : private vector_base<T, A> {
    using Base = vector_base<T, A>;
public:
    // 默认构造：空 vector，预留 0 个元素
    explicit vector(size_t n = 0, const T& val = T(), const A& a = A())
        : Base(a, n)
    {
        for (size_t i = 0; i < n; ++i)
            this->alloc.construct(&this->elem[i], val);
        this->sz = n;
    }

    // 拷贝构造
    vector(const vector& v)
        : Base(v.alloc, v.sz)
    {
        for (size_t i = 0; i < v.sz; ++i)
            this->alloc.construct(&this->elem[i], v.elem[i]);
        this->sz = v.sz;
    }

    // 拷贝赋值
    vector& operator=(const vector& v)
    {
        if (this == &v) return *this;
        if (v.sz <= this->space) {
            for (size_t i = 0; i < v.sz; ++i)
                this->alloc.construct(&this->elem[i], v.elem[i]);
            this->sz = v.sz;
            return *this;
        }
        vector_base<T,A> b(v.alloc, v.sz);
        for (size_t i = 0; i < v.sz; ++i)
            b.alloc.construct(&b.elem[i], v.elem[i]);
        // 销毁旧元素
        for (size_t i = 0; i < this->sz; ++i)
            this->alloc.destroy(&this->elem[i]);
        // swap base
        std::swap(static_cast<Base&>(*this), b);
        return *this;
    }

    // 移动构造
    vector(vector&& v) noexcept
        : Base(v.alloc, 0)
    {
        std::swap(static_cast<Base&>(*this), static_cast<Base&>(v));
    }

    // 析构
    ~vector()
    {
        for (size_t i = 0; i < this->sz; ++i)
            this->alloc.destroy(&this->elem[i]);
    }

    // 访问
    T& operator[](size_t i)       { return this->elem[i]; }
    const T& operator[](size_t i) const { return this->elem[i]; }

    size_t size()     const { return this->sz; }
    size_t capacity() const { return this->space; }

    // reserve：扩容（只增不减）
    void reserve(size_t newspace)
    {
        if (newspace <= this->space) return;
        vector_base<T,A> b(this->alloc, newspace);
        for (size_t i = 0; i < this->sz; ++i)
            b.alloc.construct(&b.elem[i], std::move(this->elem[i]));
        for (size_t i = 0; i < this->sz; ++i)
            this->alloc.destroy(&this->elem[i]);
        b.sz = this->sz;
        std::swap(static_cast<Base&>(*this), b);
    }

    // push_back
    void push_back(const T& val)
    {
        if (this->sz == this->space)
            reserve(this->space ? this->space * 2 : 8);
        this->alloc.construct(&this->elem[this->sz], val);
        ++this->sz;
    }

    // 迭代器（简单版）
    T* begin() { return this->elem; }
    T* end()   { return this->elem + this->sz; }
    const T* begin() const { return this->elem; }
    const T* end()   const { return this->elem + this->sz; }
};

} // namespace KaKaRot

#endif // ALLOCATOR_H
