#ifndef VECTOR_H
#define VECTOR_H

#include <cstddef>
#include <iterator>
#include <initializer_list>
#include <memory>
#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <utility>
#include <type_traits>

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
    // ── range-checked 随机访问迭代器 ──────────────────────────────────────────
    //   嵌套类模板：U = T 得到非 const 迭代器，U = const T 得到 const 迭代器（一套代码）。
    //   状态是三指针 cur/beg/last(尾后)；每次访问都验证边界，越界抛异常。
    //   注：reserve/搬移后 beg/last 变旧值 → 迭代器失效，与 std::vector 语义一致。
    template <typename U>
    class Checked_iter {
        template <typename V> friend class Checked_iter;   // const/非const 互为友元(转换构造要读私有成员)
    public:
        using iterator_category = std::random_access_iterator_tag;
        using value_type        = typename std::remove_cv<U>::type;  // 去 cv：const 迭代器的 value_type 也应非 const
        using difference_type   = std::ptrdiff_t;
        using pointer           = U*;
        using reference         = U&;

        Checked_iter() noexcept : cur(nullptr), beg(nullptr), last(nullptr) {}
        Checked_iter(U* p, U* b, U* e) noexcept : cur(p), beg(b), last(e) {}

        // 非 const → const 的隐式转换：仅当 U 是 const 且 V 是它去掉 const 时启用
        template <typename V, typename = typename std::enable_if<std::is_same<const V, U>::value>::type>
        Checked_iter(const Checked_iter<V>& o) noexcept : cur(o.cur), beg(o.beg), last(o.last) {}

        reference operator*()  const { check_deref();    return *cur; }
        pointer   operator->() const { check_deref();    return  cur; }
        reference operator[](difference_type n) const { check_index(n); return cur[n]; }

        Checked_iter& operator++()    { check_move(1);  ++cur; return *this; }
        Checked_iter& operator--()    { check_move(-1); --cur; return *this; }
        Checked_iter  operator++(int) { Checked_iter t = *this; ++*this; return t; }
        Checked_iter  operator--(int) { Checked_iter t = *this; --*this; return t; }

        Checked_iter& operator+=(difference_type n) { check_move(n);  cur += n; return *this; }
        Checked_iter& operator-=(difference_type n) { check_move(-n); cur -= n; return *this; }
        Checked_iter  operator+(difference_type n) const { Checked_iter t = *this; return t += n; }
        Checked_iter  operator-(difference_type n) const { Checked_iter t = *this; return t -= n; }
        friend Checked_iter operator+(difference_type n, const Checked_iter& it) { return it + n; }

        difference_type operator-(const Checked_iter& o) const { check_same(o); return cur - o.cur; }

        bool operator==(const Checked_iter& o) const { check_same(o); return cur == o.cur; }
        bool operator!=(const Checked_iter& o) const { return !(*this == o); }
        bool operator< (const Checked_iter& o) const { check_same(o); return cur <  o.cur; }
        bool operator> (const Checked_iter& o) const { return   o <  *this;  }
        bool operator<=(const Checked_iter& o) const { return !(o <  *this); }
        bool operator>=(const Checked_iter& o) const { return !(*this <  o);  }

        pointer base() const noexcept { return cur; }     // 取回裸指针(给 emplace 等用)

    private:
        U* cur;    // 当前
        U* beg;    // 下界(= 容器 start)
        U* last;   // 尾后(= 容器 finish)

        // 可解引用要求：beg <= cur < last
        void check_deref() const {
            if (cur < beg || cur >= last)
                throw std::out_of_range("Checked_iter: dereference out of range");
        }
        // 下标要求：beg <= cur+n < last。用差值比较，避免形成越界指针(严格无 UB)
        void check_index(difference_type n) const {
            if (n < beg - cur || n >= last - cur)
                throw std::out_of_range("Checked_iter: index out of range");
        }
        // 移动后位置允许落在 [beg, last](last 是合法尾后位置，可停不可解引用)
        void check_move(difference_type n) const {
            if (n < beg - cur || n > last - cur)
                throw std::out_of_range("Checked_iter: iterator moved out of range");
        }
        // 跨容器的比较/相减无意义，直接拦掉
        void check_same(const Checked_iter& o) const {
            if (beg != o.beg || last != o.last)
                throw std::invalid_argument("Checked_iter: iterators from different ranges");
        }
    };

    using iterator               = Checked_iter<T>;
    using const_iterator         = Checked_iter<const T>;
    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;
    using size_type              = std::size_t;
    using difference_type        = std::ptrdiff_t;
    using value_type             = T;
    using reference              = T&;
    using const_reference        = const T&;
    using pointer                = T*;
    using const_pointer          = const T*;

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
    vector(std::initializer_list<T> il) : start(nullptr), finish(nullptr), cap(nullptr)
    {
        if (il.size() == 0) { return; }
        AllocGuard g(alloc, il.size());                 // 拷贝途中抛异常自动回滚并释放缓冲区
        for (const T* p = il.begin(); p != il.end(); ++p) { alloc.construct(g.buf + g.built, *p); g.add(); }
        g.release();
        start  = g.buf;
        finish = g.buf + il.size();
        cap    = g.buf + il.size();
    }
    vector(const vector& rhs) : alloc(rhs.alloc), start(nullptr), finish(nullptr), cap(nullptr)
    {
        if (rhs.capacity() == 0) { return; }            // 保留原行为：按 rhs 的 capacity 分配
        AllocGuard g(alloc, rhs.capacity());            // 拷贝途中抛异常自动回滚
        for (std::size_t i = 0; i < rhs.size(); i++) { alloc.construct(g.buf + i, rhs[i]); g.add(); }
        g.release();
        start  = g.buf;
        finish = g.buf + rhs.size();
        cap    = g.buf + rhs.capacity();
    }
    // copy-and-swap: 先拷贝构造 tmp，再与 *this 交换。
    //   - 拷贝 rhs 途中抛异常时 *this 完全不受影响（强异常保证）；
    //   - 不再有"先 destroy 旧元素、再 copy"导致的二次析构 UB；
    //   - 自赋值天然安全（tmp 是独立副本，仅多一次拷贝）。
    //   注：用 const& 而非按值传参，是为了和下面 noexcept 的移动赋值共存而不产生二义。
    vector& operator=(const vector& rhs)
    {
        vector tmp(rhs);
        swap(tmp);
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
        if (size() == capacity()) {
            T tmp(std::move(value));                    // 先固化：value 可能引用容器内元素，reserve 搬移后会失效
            reserve(capacity() ? 2 * capacity() : 8);
            alloc.construct(finish++, std::move(tmp));
            return;
        }
        alloc.construct(finish++, std::move(value));
    }
    template<typename... Args>
    void emplace_back(Args&&... args)
    {
        if (size() == capacity()) {
            T tmp(std::forward<Args>(args)...);         // 先固化：args 可能引用容器内元素，reserve 搬移后会失效
            reserve(capacity() ? 2 * capacity() : 8);
            alloc.construct(finish++, std::move(tmp));
            return;
        }
        alloc.construct(finish++, std::forward<Args>(args)...);
    }
    template<typename... Args>
    iterator emplace(iterator pos, Args&&... args)
    {
        T* p = pos.base();                              // 取回裸指针，内部逻辑照旧用裸指针
        if (p < start || p > finish) { throw std::out_of_range("out of range!"); }
        std::size_t offset = p - start;                 // 先查后减，避免野指针相减 UB
        // 在任何扩容/移位之前先把新值固化到 tmp：
        //   1) args 可能引用容器内元素，reserve/移位会令其失效 —— 先固化即可规避（别名安全）；
        //   2) tmp 构造若抛异常，此刻容器尚未改动，*this 完好（强异常保证）。
        T tmp(std::forward<Args>(args)...);
        if (size() == capacity()) { reserve(capacity() ? 2 *capacity() : 8); }
        T* q = start + offset;                          // reserve 可能搬移缓冲区，用 offset 重新定位
        if (q == finish) {
            alloc.construct(finish, std::move(tmp));
        }else{
            alloc.construct(start + size(), std::move(*(start + size() -1)));
            for (std::size_t i = size() - 1; i > offset; i--) { start[i] = std::move(start[i - 1]); }
            *q = std::move(tmp);
        }
        finish++;
        return iterator(start + offset, start, finish); // 用最新边界重新构造检查版迭代器返回
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
        if (n > size()) {
            reserve(n);
            // 每构造成功一个就同步 finish：途中抛异常时 [start,finish) 始终与实际已构造元素一致，
            // ~vector 能正确清理，不会泄漏（基本异常保证）。
            while (size() < n) { alloc.construct(finish, value); ++finish; }
        } else {
            while (size() > n) { alloc.destroy(--finish); }
        }
    }
    void clear()
    {
        for (T* p = start; p < finish; p++) { alloc.destroy(p); }
        finish = start;
    }

    // ─── TODO: 待补充的接口（以后再写） ───────────────────────────────────
    //   iterator insert(T* pos, const T& value);            // 单元素插入（参考 emplace 的移位逻辑）
    //   iterator insert(T* pos, T&& value);
    //   iterator insert(T* pos, std::size_t n, const T& v); // 区间插入：先腾出 n 个空位再填充
    //   template<class It> iterator insert(T* pos, It first, It last);
    //   iterator erase(T* pos);                             // 删除单元素：pos 之后的元素整体前移一格
    //   iterator erase(T* first, T* last);                  // 删除区间
    //   void assign(std::size_t n, const T& value);         // 清空后重填 n 个 value
    //   template<class It> void assign(It first, It last);
    //   void assign(std::initializer_list<T> il);
    //   实现要点：复用 reserve() 扩容；移位用 std::move；插入前先把实参固化到临时对象
    //            （规避自引用/别名，参考 emplace）；erase 用赋值前移 + 末尾 destroy。
    // ──────────────────────────────────────────────────────────────────────

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

    // iterator —— 边界统一是 [start, finish)，cur 分别取 start / finish
    iterator       begin()  noexcept       { return iterator(start, start, finish); }
    const_iterator begin()  const noexcept { return const_iterator(start, start, finish); }
    iterator       end()    noexcept       { return iterator(finish, start, finish); }
    const_iterator end()    const noexcept { return const_iterator(finish, start, finish); }
    const_iterator cbegin() const noexcept { return const_iterator(start, start, finish); }
    const_iterator cend()   const noexcept { return const_iterator(finish, start, finish); }
    // 反向：用别名 + 检查版迭代器构造(rbegin 存 end、rend 存 begin —— reverse_iterator 的错位约定)
    reverse_iterator       rbegin()        { return reverse_iterator(end()); }
    const_reverse_iterator rbegin() const  { return const_reverse_iterator(end()); }
    reverse_iterator       rend()          { return reverse_iterator(begin()); }
    const_reverse_iterator rend() const    { return const_reverse_iterator(begin()); }
    const_reverse_iterator crbegin() const { return const_reverse_iterator(cend()); }
    const_reverse_iterator crend()   const { return const_reverse_iterator(cbegin()); }

private:
    A  alloc;
    T* start;
    T* finish;
    T* cap;
};

// 非成员相等比较：先比大小，再逐元素用 T::operator== 比较。
template <typename T, typename A>
bool operator==(const vector<T, A>& lhs, const vector<T, A>& rhs)
{
    if (lhs.size() != rhs.size()) { return false; }
    for (std::size_t i = 0; i < lhs.size(); i++) {
        if (!(lhs[i] == rhs[i])) { return false; }
    }
    return true;
}

template <typename T, typename A>
bool operator!=(const vector<T, A>& lhs, const vector<T, A>& rhs)
{
    return !(lhs == rhs);
}

// 非成员 swap：与类放在同一命名空间，使 `using std::swap; swap(a, b);` 能经 ADL 找到它，
// 从而走 O(1) 的指针交换，而不是退化成标准库基于移动的通用 swap。
template <typename T, typename A>
void swap(vector<T, A>& lhs, vector<T, A>& rhs) noexcept
{
    lhs.swap(rhs);
}

template <typename T, typename A>
std::ostream& operator<<(std::ostream& os, const vector<T,A>& vec)
{
    for (const auto& Elem : vec) {
        os << Elem << '\n';
    }

    return os;
}

template <typename T, typename A>
std::istream& operator>>(std::istream& is, vector<T, A>& vec)
{
    std::istream_iterator<T> in(is), eof;
    for (; in != eof; ++in) {
        vec.push_back(*in);
    }

    return is;
}

}
#endif //VECTOR_H