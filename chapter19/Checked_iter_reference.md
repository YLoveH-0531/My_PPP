# Range-checked 随机访问迭代器 —— 参考实现

> 仅供参考对照。设计选择:
> - 状态用**三指针** `cur / beg / end`(自包含,不耦合容器;reserve 后失效,与 std 一致)
> - const 版走**方案 A**:同一模板 `Checked_iter<T>`,用 `const T` 实例化得到 const 迭代器
> - 越界一律 `throw`(release 也生效)

---

## 1. 完整实现

```cpp
#include <cstddef>      // std::ptrdiff_t
#include <iterator>     // std::random_access_iterator_tag
#include <stdexcept>    // std::out_of_range / std::invalid_argument
#include <type_traits>  // std::remove_cv_t / std::is_same / std::enable_if_t

namespace KaKaRot {

template <typename T>
class Checked_iter {
    // const 版与非 const 版是不同的实例化,互设友元,转换构造才能读对方私有成员
    template <typename U> friend class Checked_iter;

public:
    // ── (1) 五个 traits:std 算法靠 iterator_traits 查它们来选策略 ──
    using iterator_category = std::random_access_iterator_tag;
    using value_type        = std::remove_cv_t<T>;   // 注意去掉 cv:const 迭代器的 value_type 也应是非 const
    using difference_type   = std::ptrdiff_t;
    using pointer           = T*;
    using reference         = T&;

    // ── (2) 构造 ──
    Checked_iter() noexcept : cur(nullptr), beg(nullptr), end(nullptr) {}
    Checked_iter(T* p, T* b, T* e) noexcept : cur(p), beg(b), end(e) {}

    // 非 const → const 的隐式转换。仅当 "T 是 const 且 U 是它去掉 const" 时启用:
    //   T = const int, U = int  →  const U == T  →  开启
    //   T = int(非const)        →  const U != T  →  SFINAE 关闭(禁止 const→非const)
    template <typename U,
              typename = std::enable_if_t<std::is_same<const U, T>::value>>
    Checked_iter(const Checked_iter<U>& o) noexcept
        : cur(o.cur), beg(o.beg), end(o.end) {}

    // ── (3) 解引用(带检查)──
    reference operator*()  const { check_deref();    return *cur; }
    pointer   operator->() const { check_deref();    return  cur; }
    reference operator[](difference_type n) const { check_index(n); return cur[n]; }

    // ── (4) 步进 ──
    Checked_iter& operator++()    { check_move(1);  ++cur; return *this; }
    Checked_iter& operator--()    { check_move(-1); --cur; return *this; }
    Checked_iter  operator++(int) { Checked_iter t = *this; ++*this; return t; }
    Checked_iter  operator--(int) { Checked_iter t = *this; --*this; return t; }

    // ── (5) 跳跃 ──
    Checked_iter& operator+=(difference_type n) { check_move(n);  cur += n; return *this; }
    Checked_iter& operator-=(difference_type n) { check_move(-n); cur -= n; return *this; }
    Checked_iter  operator+(difference_type n) const { Checked_iter t = *this; return t += n; }
    Checked_iter  operator-(difference_type n) const { Checked_iter t = *this; return t -= n; }
    // n + it 必须是非成员(左操作数是整数);写成 friend 最省事
    friend Checked_iter operator+(difference_type n, const Checked_iter& it) { return it + n; }

    // ── (6) 两迭代器求距离 ──
    difference_type operator-(const Checked_iter& o) const { check_same(o); return cur - o.cur; }

    // ── (7) 比较(> <= >= 全部用 < 和 == 拼出来)──
    bool operator==(const Checked_iter& o) const { check_same(o); return cur == o.cur; }
    bool operator!=(const Checked_iter& o) const { return !(*this == o); }
    bool operator< (const Checked_iter& o) const { check_same(o); return cur <  o.cur; }
    bool operator> (const Checked_iter& o) const { return   o <  *this;  }
    bool operator<=(const Checked_iter& o) const { return !(o <  *this); }
    bool operator>=(const Checked_iter& o) const { return !(*this <  o);  }

    // 取回裸指针:给 vector::emplace 之类需要 T* 的接口用
    pointer base() const noexcept { return cur; }

private:
    T* cur;   // 当前
    T* beg;   // 下界(= 容器 start)
    T* end;   // 上界 / 尾后(= 容器 finish)

    // 可解引用要求:beg <= cur < end
    void check_deref() const {
        if (cur < beg || cur >= end)
            throw std::out_of_range("Checked_iter: dereference out of range");
    }
    // 下标要求:beg <= cur+n < end。用差值比较,不去形成 cur+n(严格无 UB)
    void check_index(difference_type n) const {
        if (n < beg - cur || n >= end - cur)
            throw std::out_of_range("Checked_iter: index out of range");
    }
    // 移动后位置允许落在 [beg, end](end 是合法尾后位置,可停不可解引用)
    void check_move(difference_type n) const {
        if (n < beg - cur || n > end - cur)
            throw std::out_of_range("Checked_iter: iterator moved out of range");
    }
    // 可选:跨容器的比较/相减无意义,直接拦掉
    void check_same(const Checked_iter& o) const {
        if (beg != o.beg || end != o.end)
            throw std::invalid_argument("Checked_iter: iterators from different ranges");
    }
};

} // namespace KaKaRot
```

---

## 2. 接进 vector

别名切过去 + `begin/end` 家族返回检查版:

```cpp
// 别名
using iterator               = Checked_iter<T>;
using const_iterator         = Checked_iter<const T>;
using reverse_iterator       = std::reverse_iterator<iterator>;        // 自动包住检查版,反向遍历也带检查
using const_reverse_iterator = std::reverse_iterator<const_iterator>;

// 正向:cur 分别取 start / finish,边界都是 [start, finish)
iterator       begin()  noexcept       { return iterator(start, start, finish); }
const_iterator begin()  const noexcept { return const_iterator(start, start, finish); }
iterator       end()    noexcept       { return iterator(finish, start, finish); }
const_iterator end()    const noexcept { return const_iterator(finish, start, finish); }
const_iterator cbegin() const noexcept { return const_iterator(start, start, finish); }
const_iterator cend()   const noexcept { return const_iterator(finish, start, finish); }

// 反向:用别名 + 检查版迭代器构造(rbegin 存 end、rend 存 begin —— reverse_iterator 的错位约定)
reverse_iterator       rbegin()        { return reverse_iterator(end()); }
const_reverse_iterator rbegin() const  { return const_reverse_iterator(end()); }
reverse_iterator       rend()          { return reverse_iterator(begin()); }
const_reverse_iterator rend()   const  { return const_reverse_iterator(begin()); }
const_reverse_iterator crbegin() const { return const_reverse_iterator(cend()); }
const_reverse_iterator crend()   const { return const_reverse_iterator(cbegin()); }
```

> `begin() const` 里把 `start`(`T*`)传给 `Checked_iter<const T>`(收 `const T*`),靠 `T* → const T*` 隐式转换成立。

**内部实现基本不受影响**:`reserve`/`emplace` 用的是成员裸指针 `start/finish`,不是 `begin()/end()`;`operator<<` 的 range-for 正常遍历不会解引用 `end`,不会误抛。

---

## 3. emplace 的处理(方案 A:改收 iterator)

原来 `emplace(T* pos, ...)` 收裸指针,`v.emplace(v.begin(), x)` 就传不进去了。改成收 `iterator`,进函数先 `.base()` 取裸指针,返回也用 `iterator` 包回去:

```cpp
template <typename... Args>
iterator emplace(iterator pos, Args&&... args)
{
    T* p = pos.base();                                  // ← 取回裸指针,后续逻辑照旧
    if (p < start || p > finish) throw std::out_of_range("out of range!");
    std::size_t offset = p - start;
    // ...（原有的固化 tmp、扩容、移位逻辑不变，都用裸指针 start/finish）...
    return iterator(start + offset, start, finish);     // ← 用最新边界重新构造返回
}
```

其余收裸指针的接口(将来的 `insert`/`erase`)同理:签名收 `iterator`,内部 `.base()`。

---

## 4. 自测片段(验证"正常不误伤 + 越界会抛 + 能喂给 std 算法")

```cpp
#include <cassert>
#include <algorithm>
#include <stdexcept>

void test() {
    KaKaRot::vector<int> v;
    for (int i = 0; i < 5; ++i) v.push_back(i);      // 0 1 2 3 4

    // (a) 正常遍历不误伤
    int sum = 0;
    for (auto it = v.begin(); it != v.end(); ++it) sum += *it;
    assert(sum == 10);

    // (b) 随机访问
    assert(v.begin()[3] == 3);
    assert(*(v.end() - 1) == 4);
    assert((v.end() - v.begin()) == 5);

    // (c) 解引用 end() 必须抛
    bool threw = false;
    try { (void)*v.end(); } catch (const std::out_of_range&) { threw = true; }
    assert(threw);

    // (d) 越界移动必须抛
    threw = false;
    try { auto it = v.begin(); it += 1000; } catch (const std::out_of_range&) { threw = true; }
    assert(threw);

    // (e) 下标越界必须抛
    threw = false;
    try { (void)v.begin()[500]; } catch (const std::out_of_range&) { threw = true; }
    assert(threw);

    // (f) 是合格的随机访问迭代器 —— 能喂给 std 算法
    std::sort(v.begin(), v.end(), std::greater<int>{});   // 4 3 2 1 0
    assert(v.begin()[0] == 4);
    assert(std::find(v.begin(), v.end(), 2) != v.end());
}
```

---

## 5. 关键点备忘

1. **为什么原来 `iterator = T*` 不用写 traits**:标准库有 `std::iterator_traits<T*>` 的指针偏特化,免费给全 5 个 traits。换成 class 就没这待遇,必须自己写(否则 `std::sort` 等查 `iterator_category` 时编译失败)。
2. **检查为什么用差值 `n < beg - cur`**:直接写 `cur + n` 再判断,`n` 过大时 `cur + n` 本身就越界 → 指针算术 UB,还没 throw 就已经 UB。`beg - cur` / `end - cur` 是同数组内的合法指针差,拿它和 `n` 比是纯整数比较,无 UB。(PPP 范围内不写这层也能跑,但这是"严格正确"的写法。)
3. **`value_type` 用 `remove_cv_t<T>`**:const 迭代器的 `value_type` 也应是非 const(否则某些算法的 `value_type tmp;` 会出问题)。这是对你原计划里 `value_type = T` 的一处修正。
4. **`n + it` 必须非成员**:左操作数是整数,成员函数形式匹配不上。写成 friend 最简单。
5. **`base()` 是胶水**:让检查版迭代器能和收裸指针的旧接口(emplace 等)互通。
6. **reverse_iterator 免费带检查**:因为 `std::reverse_iterator<iterator>` 现在包的是 `Checked_iter`,底层每次访问都会走检查。
