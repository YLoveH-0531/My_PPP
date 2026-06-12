# Chapter 19 — 模板、泛型编程与资源管理

---

## 1. 多态（Polymorphism）

多态（希腊语"多种形态"）= 通过**统一接口操作多种不同类型**。C++ 里有两种：

| 类型 | 机制 | 解析时机 |
|------|------|---------|
| **泛型编程** | 模板 | 编译期（compile-time） |
| **面向对象** | 类继承 + 虚函数 | 运行期（run-time） |

---

## 2. 概念（Concepts）

> 对模板参数的一组**要求**称为 concept。模板参数必须满足所应用模板的 concept。

```cpp
// 例：inner_product 对 T 的隐式 concept：
//   T 支持 += 和 *
// Number<T> 只有实现了这两个运算符，才能被 inner_product 使用
```

**注意**：`C<D>` 是 `C<B>` 的子类型 **不成立**，即使 `D` 是 `B` 的子类：

```cpp
// D 继承自 B，但：
// vector<D> 不是 vector<B> 的子类型
// 两者是完全独立的实例化
```

---

## 3. RAII（Resource Acquisition Is Initialization）

**名字只强调"获取"那一半**——因为"释放"是自动推论出来的，不需要特别强调。

> 把"获取资源"绑定到"对象诞生"，"释放资源"就自动等于"对象死亡"——而对象死亡由语言（作用域/析构函数）保证。

```cpp
// 没有 RAII：获取和释放是两个分离的动作，容易漏
FILE* fp = fopen("a.txt", "r");
// ... 多个 return 路径，容易忘记 fclose
fclose(fp);   // 可能根本跑不到

// 有 RAII：ifstream 在构造时 open，析构时自动 close
{
    std::ifstream file("a.txt");
    // ... 任意 return，file 析构都会自动关闭
}
```

适用于所有资源：文件句柄、数据库锁、socket、内存、I/O 缓冲区等。

---

## 4. allocator：把"分配内存"和"构造对象"拆开

`new T[n]` 把**分配内存**和**构造对象**捆死了。`allocator` 把两件事分开：

```cpp
allocator<T> a;
T* p = a.allocate(n);       // 只拿原始内存，不构造任何对象
a.construct(p, args...);    // 在指定位置构造对象（placement new）
a.destroy(p);               // 只析构，不释放内存
a.deallocate(p, n);         // 只释放内存
```

**为什么 vector 必须用 allocator**：

```
[已构造][已构造][已构造][ 空 ][ 空 ]   ← capacity=5, size=3
 <────── size ──────>
 <──────────── capacity ──────────>
```

- `new T[capacity]` 会**强制构造全部 capacity 个对象**——浪费，且要求 T 能默认构造
- allocator 只构造前 `size` 个，后面留着原始内存

**为什么不能在未初始化内存上直接赋值**：

```cpp
// string 的 operator= 第一步是 delete[] 旧数据
// 未初始化内存里是垃圾指针，delete 垃圾地址 → 崩溃
// 必须用 placement new（构造）而不是赋值
new(p) T(src);   // ✅ 从零构造，不假设原有状态
*p = src;        // ❌ 假设 *p 已是合法对象
```

---

## 5. 接口设计原则：快的可以变慢，慢的无法变快

```cpp
// 无检查（快）→ 在它之上叠加有检查（慢）：✅ 可以
T& at(size_t i) {
    if (i >= size_) throw out_of_range{};
    return (*this)[i];   // 复用无检查的 operator[]
}

// 有检查（慢）→ 想造出无检查（快）：❌ 做不到
// at() 内部的 if 永远在跑，去不掉
```

> 设计原则：**把最精简、最快的版本做底层，把额外功能叠在上层**。

---

## 6. 模板两阶段名称查找（Two-Phase Name Lookup）

> **这是 C++ 模板里最容易踩的坑之一**，直接影响跨编译器的可移植性。

### 6.1 基础：普通函数的名称查找

普通函数调用一个名字，编译器同时做两件事：

**① 普通非限定查找**：从调用点向外逐层扫描——函数内 → 类 → 命名空间 → 全局

**② ADL（参数依赖查找）**：把每个参数类型所在的命名空间也加入查找范围

```cpp
namespace Foo {
    struct Bar {};
    void f(Bar);
}

void test() {
    Foo::Bar b;
    f(b);   // ✅ ADL：b 的类型在 Foo，所以 Foo 被加入查找范围
}
```

**ADL 的意义**：`std::cout << x` 能找到 `std::operator<<`，就是因为 `cout` 的类型在 `std`，ADL 把 `std` 加进来了。

### 6.2 依赖名称 vs 非依赖名称

```cpp
template<typename T>
void func() {
    int x = global_val;   // 非依赖名称：不含 T，第一阶段确定
    is >> val;            // 依赖名称：val 的类型依赖 T，两阶段处理
}
```

### 6.3 两阶段规则

| 阶段 | 时机 | 查找方式 | 查什么 |
|------|------|---------|--------|
| **第一阶段** | 模板**定义时** | 普通非限定查找（不含ADL） | 定义点可见的所有名称 |
| **第二阶段** | 模板**实例化时** | **只做 ADL** | 实参类型命名空间里的名称 |

**关键**：第二阶段**只做 ADL，不再做普通查找**。

### 6.4 ADL 查哪些命名空间

对类型 `T`，ADL 加入：
- `T` 直接所在的命名空间
- `T` 的所有基类的命名空间
- `T` 是类模板实例（如 `vector<int>`）时：模板本身的命名空间（`std`）+ 每个模板参数的命名空间
- T 是指针/引用/数组时，递归地处理指向的类型
- **内置类型（int/double 等）不贡献任何命名空间**

```cpp
// std::vector<int> 的 ADL 只加入 std
// int 是内置类型，不贡献任何命名空间
// → 全局命名空间的 operator>> 不会被 ADL 找到
```

### 6.5 实际踩坑案例（本章 drill_19_1.h）

```cpp
// ❌ 原来的顺序（错误）：
template<typename T>
struct S {
    void read_val(std::istream& is);   // 第10行：定义时 operator>> 还没声明
};

template<typename T>
void S<T>::read_val(std::istream& is) {
    is >> val;   // 第71行：依赖名称，两阶段查找
}

template<typename T>
std::istream& operator>>(std::istream& is, std::vector<T>& v) {
    ...          // 第92行：定义在 read_val 之后！
}
```

查找过程：
- **第一阶段（第71行定义时）**：向上普通查找 `operator>>`，第92行还没到 → **找不到**
- **第二阶段（实例化时）**：只做 ADL，`istream`/`vector<int>` 都在 `std` → 查 `std`，自定义 `operator>>` 在全局命名空间 → **找不到**
- **结果**：Clang 报错，GCC 宽松实现下碰巧能跑

```cpp
// ✅ 修正后的顺序：把 operator>> 移到 struct S 之前
template<typename T>
std::istream& operator>>(std::istream& is, std::vector<T>& v) { ... }

// 现在 read_val 定义时，第一阶段普通查找能找到 operator>> ✅
template<typename T>
struct S { void read_val(std::istream& is); };
```

### 6.6 GCC vs Clang 的行为差异

| 编译器 | 行为 | 是否符合标准 |
|--------|------|------------|
| **GCC** | 实例化时除 ADL 外还做完整普通查找（宽松） | ❌ 超出标准 |
| **Clang** | 实例化时严格只做 ADL | ✅ 符合标准 |

> **结论**：只在 GCC 上能跑的模板代码，不等于正确。养成习惯：**模板里调用的全局命名空间函数，必须在模板定义之前声明**。

---

## 7. 运算符设计规范

从本章练习总结的高频错误和规范：

### operator= 必须 return *this
```cpp
T& operator=(const T& rhs) {
    if (this == &rhs) return *this;
    // ... 赋值逻辑
    return *this;   // ← 必须！漏掉是 UB（函数无返回值）
}
```

### operator+ vs operator+=
```cpp
// + ：不修改自身，返回新对象，加 const
Number operator+(const Number& rhs) const {
    return Number(val + rhs.val);
}

// += ：修改自身，返回自身引用，不加 const
Number& operator+=(const Number& rhs) {
    val += rhs.val;
    return *this;
}
```

### 比较运算符加 const
```cpp
bool operator==(const T& rhs) const { ... }  // ✅ const：不修改对象
bool operator==(const T& rhs) { ... }         // ❌ const T 对象无法调用
```

### 移动构造要用 std::move
```cpp
Number(T&& t) : val(std::move(t)) {}   // ✅ 真正移动
Number(T&& t) : val(t) {}             // ❌ t 是右值引用但 val(t) 是拷贝
```

---

## 8. 模板的鸭子类型（Duck Typing）

模板不关心类型的名字，只关心它**支持哪些操作**：

```cpp
// inner_product 隐式要求 T 支持 * 和 +=
// 只要满足这个 concept，任何类型都能用：
inner_product<int>(v1, v2);            // ✅
inner_product<double>(v1, v2);         // ✅
inner_product<Number<int>>(v1, v2);    // ✅ 自定义类型同样可以
```

这是泛型编程的核心威力：**写一次，适用于所有满足 concept 的类型**。

---

## 9. ADL 的历史背景与本质

### 9.1 ADL 为什么存在——工程问题，不是哲学

命名空间是 **C++98** 才加入的。加进来后立刻暴露了一个兼容性问题：

```cpp
// 命名空间加进来之前，这个写法好用：
cout << x;

// 标准库全进了 std 之后，理论上要这样写：
std::operator<<(std::cout, x);   // 极丑，链式调用根本没法写
```

ADL（Koenig Lookup，以发明者 Andrew Koenig 命名）**就是为了修这个兼容性问题**而引入的——让原本能工作的运算符语法在加了命名空间之后继续工作。

### 9.2 ADL 解决的是调用侧的问题

**"定义在一起"不等于"调用时能找到"**——这是关键。

```cpp
namespace Geo {
    struct Point { double x, y; };
    std::ostream& operator<<(std::ostream& os, const Point& p) { ... }
}

int main() {
    Geo::Point p{1.0, 2.0};
    std::cout << p;   // ← 调用点在全局命名空间
                      // 普通查找：main 内部 → 全局，不会进 Geo
                      // ADL：p 的类型 Geo::Point 在 Geo → 自动查 Geo → 找到
}
```

- **定义侧**：`Point` 和 `operator<<` 写在同一个命名空间里——这是事实
- **调用侧**：调用者在命名空间外面，普通查找进不去——这是问题
- **ADL**：看到参数类型 `Geo::Point`，自动把 `Geo` 加进查找范围——这是解法

### 9.3 ADL 的实质

和引用是"好看的指针"一样，ADL 是一个**编译期名称查找规则的扩展**：

> 编译器在调用点看到参数的类型，自动把参数类型所在的命名空间加进查找范围，等价于帮你**自动补命名空间前缀**。

```cpp
// 你写的：
std::cout << p;

// ADL 帮你做的事等价于：
Geo::operator<<(std::cout, p);
```

**没有运行时开销，没有虚表，纯粹是编译期的名字解析规则**——和引用一样，是让代码更自然的语法层机制。

### 9.4 使用时形成的惯例

ADL 引入后，顺带确立了一个 C++ 惯例：

> **把类型和它的配套函数（尤其是运算符、`swap`、`begin`/`end`）放在同一命名空间里**，调用者就能自然地不加前缀地使用它们。

```cpp
// 标准库算法的定制点：
namespace MyLib {
    struct BigData { std::vector<int> data; };
    void swap(BigData& a, BigData& b) { a.data.swap(b.data); }  // O(1)
}

std::sort(v.begin(), v.end());
// sort 内部调用 swap(a, b)
// ADL：a/b 是 MyLib::BigData → 查 MyLib → 找到高效的 MyLib::swap
// 自动用你的定制版，不是 std::swap 的通用拷贝版
```

### 9.5 ADL 的代价：隐式 = 意外

ADL 是隐式的，有时会意外找到你不想要的函数：

```cpp
namespace Evil {
    struct Hijack {};
    template<typename T>
    void swap(T& a, T& b) { /* 干坏事 */ }
}

std::vector<Evil::Hijack> v;
swap(v[0], v[1]);   // 你以为是 std::swap
                    // 但 ADL 找到了 Evil::swap！
```

防御写法：

```cpp
using std::swap;
swap(a, b);   // using 让 std::swap 参与重载决议，明确优先级
```

### 9.6 一句话

> **ADL 是 C++98 加入命名空间时打的向后兼容补丁，核心动机是让 `cout << x` 继续好用。本质是编译期自动把参数类型的命名空间加进查找范围，等价于帮你补命名空间前缀——无运行时开销，纯语法层规则。"类型和配套函数放同一命名空间"是使用 ADL 后形成的惯用法，不是它的哲学出发点。**

---

## 10. 完美转发（Perfect Forwarding）

### 10.1 问题：普通传参会丢失右值性

```cpp
void target(int& x)  { std::cout << "左值\n"; }
void target(int&& x) { std::cout << "右值\n"; }

void wrapper(int&& x) {
    target(x);   // ← x 有名字，在函数体内是左值！
                 // 调用的是 target(int&)，右值性丢了
}

wrapper(42);   // 传了右值，但 target 收到的是左值
```

**根本原因**：一个值一旦有了名字（变量名），在函数体内就是**左值**，不管声明类型是不是 `&&`。

### 10.2 解法：转发引用 + std::forward

```cpp
template<typename T>
void wrapper(T&& x) {             // T&& 是"转发引用"（万能引用）
    target(std::forward<T>(x));   // 完美转发：保留原来的左值/右值性
}

int a = 10;
wrapper(a);    // a 是左值 → T=int& → forward 保持左值 → 调 target(int&)
wrapper(42);   // 42 是右值 → T=int  → forward 保持右值 → 调 target(int&&)
```

**转发引用（`T&&`）**：只在模板参数推导上下文里才是万能引用，能绑左值也能绑右值。

| 传入 | T 推导为 | T&& 实际是 |
|------|---------|-----------|
| 左值 `int a` | `int&` | `int& && = int&` |
| 右值 `42` | `int` | `int&&` |

**`std::forward<T>`**：根据 T 决定是否恢复右值——本质是一个有条件的 `static_cast<T&&>`。

### 10.3 forward vs move

```cpp
std::move(x)          // 无条件把 x 变成右值，不管 x 原来是什么
std::forward<T>(x)    // 有条件：T 是右值引用类型时变右值，否则保持左值
```

---

## 11. 可变参数模板（Variadic Templates）

### 11.1 参数包是什么

**参数包就是"一组类型/值的集合"**，大小在编译期由调用者决定。

```cpp
template<typename... Args>   // Args：类型包
void f(Args&&... args) {     // args：值包
    // 调用 f(1, 3.14, "hi") 时：
    // Args = { int,  double,  const char* }
    // args = {  1,   3.14,      "hi"     }
    //           ↕      ↕           ↕  一一对应
}
```

### 11.2 展开：`...` 的作用

包**不能直接用**，必须展开。`...` 放在含有包的表达式后面，意思是：

> **把这个表达式对包里每个元素重复一遍，用逗号隔开。**

```cpp
// 模式：std::forward<Args>(args)
std::forward<Args>(args)...

// 编译器展开成（假设3个参数）：
std::forward<int>(a1), std::forward<double>(a2), std::forward<const char*>(a3)
// 和你手写 3 个参数的效果完全一样
```

`...` 放在哪，那个整体表达式就作为模式被展开：

```cpp
args...            // → a1, a2, a3
sizeof(Args)...    // → sizeof(T1), sizeof(T2), sizeof(T3)
std::forward<Args>(args)...  // → forward<T1>(a1), forward<T2>(a2), ...
```

### 11.3 多参数完美转发

```cpp
template<typename... Args>
void wrapper(Args&&... args) {
    target(std::forward<Args>(args)...);   // 所有参数完整转发
}
```

### 11.4 实战：emplace_back

```cpp
// push_back：先构造临时对象，再移动进去（多一步）
v.push_back(std::string("hello"));

// emplace_back：把构造参数直接转发到 vector 内存上，就地构造，零拷贝
v.emplace_back("hello");

// emplace_back 的实现原理：
template<typename... Args>
void emplace_back(Args&&... args) {
    new(end_ptr) T(std::forward<Args>(args)...);  // 就地构造
}
```

### 11.5 实用工具

**`sizeof...`**：获取参数包大小

```cpp
template<typename... Args>
void f(Args&&... args) {
    std::cout << sizeof...(args) << "\n";   // 参数个数
}
f(1, 2.0, "hi");   // 输出 3
```

**折叠表达式（C++17）**：一行展开所有参数，不用递归

```cpp
// 依次打印所有参数
template<typename... Args>
void print(Args&&... args) {
    ((std::cout << args << " "), ...);
}

// 求和
template<typename... Args>
auto sum(Args&&... args) {
    return (args + ...);   // 展开成 a1 + a2 + a3 + ...
}
```

### 11.6 一句话

> **参数包是"一组类型/值的集合"，`...` 放在含包的表达式后面就是展开——把那个表达式对每个元素重复一遍用逗号隔开，和手写 N 个参数等价，只是让编译器替你重复这件事。完美转发 = 转发引用（`T&&`）+ `std::forward<T>`，把参数的左值/右值性原封不动地传给下一个函数。**


一句话总结:你对「异常传播路径」的理解完全正确,但漏了「构造失败的对象其析构函数不被调用」这条规则;正因为这条规则,结果是资源泄漏,而不是残次品对象
  在析构时出问题

---

## 12. 手写 `vector` 的异常安全与资源管理（来自 `Vector.h` 练习）

围绕自己实现的 `KaKaRot::vector` 反复打磨出来的三组核心要点。

### 12.1 分配器层：`construct`/`destroy` 与 RAII 回滚守卫

#### `std::allocator::construct/destroy` 的标准变迁

| 标准 | 状态 | 替代方案 |
|------|------|---------|
| C++11/14 | 可用 | — |
| C++17 | **弃用（deprecated）** | `std::allocator_traits<A>::construct/destroy` |
| C++20 | **删除（removed）** | 同上，否则编译失败 |

```cpp
// 旧（仅 C++17 及以前）
alloc.construct(p, args...);
alloc.destroy(p);

// 规范写法（任意标准都可，C++20 必须）
std::allocator_traits<A>::construct(alloc, p, args...);
std::allocator_traits<A>::destroy(alloc, p);
```

> `allocator_traits` 还顺带统一处理「分配器传播（`propagate_on_*`）」和「分配器没有 `construct` 成员」的情况。

#### RAII 回滚守卫：批量构造的异常安全骨架

裸的「`allocate` + 循环 `construct`」一旦中途抛异常，已分配的内存 + 已构造的元素全部泄漏。把「缓冲区 + 已构造计数」绑到一个**局部对象**上，靠析构兜底：

```cpp
struct AllocGuard {
    A& a; T* buf; size_t cap_n; size_t built; bool committed;
    AllocGuard(A& a_, size_t n)
        : a(a_), buf(a_.allocate(n)), cap_n(n), built(0), committed(false) {}
    void add()     { ++built; }              // 每 construct 成功一个就"记账"
    void release() { committed = true; }     // 全部成功后"提交"，转交所有权
    ~AllocGuard() {
        if (committed) return;               // 已提交 → 不回滚
        for (size_t i = 0; i < built; i++) a.destroy(buf + i);  // 逆向清理
        a.deallocate(buf, cap_n);
    }
};
```

使用范式（计数构造 / `reserve` / 拷贝构造 / il 构造都用这一套）：

```cpp
AllocGuard g(alloc, n);
for (size_t i = 0; i < n; i++) { alloc.construct(g.buf + i, ...); g.add(); }
g.release();                                 // 成功才提交
start = g.buf; finish = g.buf + n; cap = g.buf + n;
```

> **`add()` 记账，`~AllocGuard()` 在异常时按账本清理，C++ 的栈展开保证析构一定执行。** 这就是把裸指针"升级"成能自动清理的 RAII 对象。

### 12.2 构造函数的异常语义（关键认知）

> **构造函数抛异常 ⇒ 该对象被视为「从未构造成功」⇒ 它自己的析构函数绝不被调用；但已构造完成的成员/基类会被正常析构。**

```cpp
struct Widget {
    Member m;                 // 进入函数体前已构造完成
    Widget() { throw ...; }   // 函数体抛异常
    ~Widget() { ... }         // ← 不会被调用！
};
// 实测输出：Member 构造 → Member 析构（成员被清理）→ 捕获
//          但没有 "Widget 析构"
```

对手写 `vector` 的含义：

- 成员 `start/finish/cap` 是**裸指针**，其"析构"是空操作 → 不会释放指向的内存。
- 构造途中抛异常 + `~vector()` 不执行 → **纯内存泄漏**（不是"残次品对象析构时出错"）。
- 标准为何这样设计：若强行调 `~vector()`，会对「只构造了一半、却被 `finish` 标记为全满」的内存做析构 → **对未初始化内存析构 = UB**。两害相权，标准选「不调析构」，代价是泄漏——而泄漏正好用 **12.1 的 RAII 守卫** 来堵。

常见误区纠正：

| 误以为 | 实际 |
|--------|------|
| 异常上抛时对象已构造成"残次品" | 对象从未诞生，调用方那个变量不存在 |
| 析构残次品时会出问题 | `~vector()` 根本不跑，谈不上析构出错 |
| 成员也不管了 | 已构造完成的成员**会**被析构（只是裸指针清理不了内存） |

### 12.3 拷贝控制：copy-and-swap

#### 「先毁后建」的二次析构 UB

```cpp
// 反面教材：
for (T* p = start; p < finish; p++) alloc.destroy(p);   // ① 先毁旧元素
std::uninitialized_copy(rhs.begin(), rhs.end(), start);  // ② 可能抛异常
finish = start + rhs.size();
// 若 ② 抛异常：[start,finish) 已是裸内存，但 finish 仍指旧尾
// → ~vector 再次 destroy 这段 → double destroy（UB）
```

#### copy-and-swap 修法（强异常保证）

```cpp
vector& operator=(const vector& rhs) {
    vector tmp(rhs);    // 拷贝失败发生在改动 *this 之前
    swap(tmp);          // swap 是 noexcept 的 O(1) 指针交换
    return *this;
}                       // tmp 析构自动清理旧资源
```

- **强保证**：拷贝 `rhs` 出错时 `*this` 完全不变。
- 无"先毁后建" → 无二次析构 UB。
- 自赋值天然安全（`tmp` 是独立副本），仅多一次拷贝。

#### 为什么不用「按值统一赋值」`operator=(vector rhs)`

```cpp
vector& operator=(vector rhs);        // 按值：左值→拷贝、右值→移动，本可统一两者
vector& operator=(vector&& rhs);      // 但与它共存会"重载二义"（实测编译报错）
```

- 按值版很优雅，但**和单独的 `noexcept` 移动赋值并存会二义**。
- 对容器而言 **`noexcept` 移动赋值很关键**：`std::vector<vector<T>>` 扩容时靠 `move_if_noexcept` 决定走移动还是拷贝。
- **结论**：采用「`const&` 版 copy-and-swap」+「专门的 `noexcept` 移动赋值」两个重载——既无二义，又保住 noexcept。

> 一句话：copy-and-swap 用"先建副本、再交换、旧的随副本析构"的顺序，把异常风险挡在改动自身之前，天然获得强保证；对容器要单独保留 `noexcept` 移动赋值。