# Chapter 20 — C++ STL 萃取（Traits）与编译期类型反射

## 练习目录

1. drill
2. exer
3. vector

---

# C++ STL 萃取（Traits）详解

## 一、什么是萃取？

**Traits** 本意为"特征"。在 C++ 中被译为**"萃取"**，可以理解为一台**"类型榨汁机"**：把任意类型丢进去，编译期就能把它的内部特征（值类型、迭代器类别、引用类型……）提取出来。

**核心定义：** 萃取是利用**类模板**和**模板偏特化**，在**编译期**提取、查询和处理类型信息的泛型编程设计模式。

---

## 二、为什么需要萃取？

### 痛点：泛型算法需要知道内部类型

对于自定义类迭代器，可以通过嵌套 `using` 声明内部类型：

```cpp
template <typename T>
class MyIterator {
public:
    using value_type = T;
};

template <class Iterator>
void my_algorithm(Iterator it) {
    typename Iterator::value_type temp = *it;  // OK
}
```

### 危机：原生指针不支持嵌套定义

```cpp
int arr[5] = {1, 2, 3, 4, 5};
my_algorithm(arr);  // 传入 int*，编译报错！
```

`int*` 不是类，内部不存在 `value_type`，编译器遇到 `int*::value_type` 直接崩溃。

### 解法：`iterator_traits` 中间层

```cpp
// 通用版：转发给自定义迭代器的内嵌类型
template <class Iterator>
struct iterator_traits {
    using value_type = typename Iterator::value_type;
};

// 偏特化：专门处理原生指针 T*
template <class T>
struct iterator_traits<T*> {
    using value_type = T;
};

// 偏特化：专门处理 const T*
template <class T>
struct iterator_traits<const T*> {
    using value_type = T;
};
```

算法统一通过萃取机查询，类和指针一视同仁：

```cpp
template <class Iterator>
void my_algorithm(Iterator it) {
    typename std::iterator_traits<Iterator>::value_type temp = *it;  // 类或指针均 OK
}
```

---

## 三、本质：编译期的类型反射

Java/Python 的反射发生在**运行期**，代价昂贵。C++ 追求"零成本抽象"，用模板元编程在**编译期**实现了同等能力，即萃取的本质。

`<type_traits>` 展示了编译期反射的三大能力：

### 1. 类型查询（"你是谁？"）

```cpp
std::is_pointer_v<T>        // T 是指针吗？
std::is_class_v<T>          // T 是类吗？
std::is_floating_point_v<T> // T 是浮点数吗？
```

### 2. 类型变换（"改变形态"）

```cpp
std::remove_reference_t<T>  // int& → int
std::add_const_t<T>         // T   → const T
std::decay_t<T>             // 数组/函数 → 对应指针
```

### 3. 类型路由（"看客下菜碟"）

C++17 的 `if constexpr` 让编译期分发极为简洁：

```cpp
#include <type_traits>

template <typename T>
void printValue(T val) {
    if constexpr (std::is_pointer_v<T>) {
        std::cout << "指针指向的值: " << *val << '\n';
    } else {
        std::cout << "直接值: " << val << '\n';
    }
}

int main() {
    int a = 42;
    int* p = &a;
    printValue(a);  // 编译产物只含 else 分支
    printValue(p);  // 编译产物只含 if 分支
}
```

每条分支在编译期已确定，运行时**零额外开销**。

---

## 四、现代 C++ 的演进

每一代改的不是萃取的**本质**，而是**怎么写得更像人话**。

### C++11/14 — `_t` / `_v`：纯粹的语法糖

萃取结果有两种形式：提取出一个**类型**，或得到一个 **true/false**。  
C++11 之前写法极为啰嗦：

```cpp
typename std::remove_reference<T>::type   // 取类型：要写 typename + ::type
std::is_pointer<T>::value                 // 取值：  要写 ::value
```

C++11/14 用别名模板和变量模板把尾巴藏起来（标准库已内置，不需要自己写）：

```cpp
// 库内部定义
template<typename T> using remove_reference_t = typename remove_reference<T>::type;
template<typename T> inline constexpr bool is_pointer_v = is_pointer<T>::value;

// 使用时
remove_reference_t<int&>   // → int    （省掉 typename 和 ::type）
is_pointer_v<int*>         // → true   （省掉 ::value）
```

---

### C++17 — `if constexpr`：逻辑从三个函数收进一个

**旧方式（Tag Dispatching）**：根据类型选不同实现，必须拆成多个重载函数，用 tag 类型区分：

```cpp
// 两个实现
template<typename Iter>
void advance_impl(Iter& it, int n, random_access_iterator_tag) { it += n; }

template<typename Iter>
void advance_impl(Iter& it, int n, input_iterator_tag) { while (n--) ++it; }

// 对外接口：萃取 tag，靠类型选重载
template<typename Iter>
void advance(Iter& it, int n) {
    using Tag = typename iterator_traits<Iter>::iterator_category;
    advance_impl(it, n, Tag{});
}
```

逻辑被拆散在三个函数里，读代码要跳来跳去。

**新方式（`if constexpr`）**：分支直接写进函数体，编译期选定，另一分支不参与编译：

```cpp
template<typename Iter>
void advance(Iter& it, int n) {
    using Tag = typename iterator_traits<Iter>::iterator_category;
    if constexpr (std::is_same_v<Tag, random_access_iterator_tag>) {
        it += n;           // 随机访问迭代器编译这行
    } else {
        while (n--) ++it;  // 其他迭代器编译这行
    }
}
```

---

### C++20 — Concepts：约束从魔法咒语变成人话

**旧方式（`enable_if` / SFINAE）**：限制模板只接受整型，写法晦涩，报错是一页模板展开垃圾：

```cpp
template<typename T, std::enable_if_t<std::is_integral_v<T>, int> = 0>
void print(T val) { std::cout << val << '\n'; }
```

**新方式（Concepts）**：先定义类型契约，再直接使用：

```cpp
// 定义概念
template<typename T>
concept Integral = std::is_integral_v<T>;

// 使用（两种等价写法）
template<typename T> requires Integral<T>
void print(T val) { std::cout << val << '\n'; }

void print(Integral auto val) { std::cout << val << '\n'; }  // 更简洁
```

传错类型时报错直接说明原因：

```
error: the concept 'Integral<double>' was not satisfied
```

---

### 三代对比

```
C++11/14   remove_reference<T>::type  →  remove_reference_t<T>   少打字
           is_pointer<T>::value       →  is_pointer_v<T>

C++17      三函数 + tag 类型           →  一函数 + if constexpr    逻辑集中

C++20      enable_if<...> 魔法咒语    →  concept + requires       可读 + 友好报错
```

---

## 五、一句话总结

> 萃取是 **C++ 编译期的类型反射机制**。泛型编程需要根据类型的不同特征进行算法优化或类型推导，而 Traits 利用模板偏特化，让原生类型和自定义类型能被**一视同仁**地处理，且所有开销在编译期消化，运行时归零。
