C++ 把“调用某个东西”也抽象成了一种运算，并用 () 表示这种运算；因此只要一个对象定义了 operator()，它就可以获得“像函数一样被调用”的能力。
# 函数调用、Function Object 与静态/动态多态——整体理解

## 一、起点：`f(x)` 首先是一种“调用操作”

C++ 中：

```cpp
f(x, y)
```

可以把它理解成一种 **function call expression（函数调用表达式）**。

对于 function object：

```cpp
struct Compare {
    bool operator()(int a, int b) const {
        return a < b;
    }
};

Compare cmp;
cmp(1, 2);
```

这里的：

```cpp
cmp(1, 2)
```

对应：

```cpp
cmp.operator()(1, 2);
```

因此，`operator()` 允许一个普通对象获得“像函数一样调用”的能力。

这一步带来的真正意义是：

> **不同种类的可调用对象，可以统一使用 `f(args...)` 这种调用语法。**

例如：

```cpp
普通函数
函数指针
function object
lambda
std::function
```

都可以写成：

```cpp
f(x);
```

但这只解决了**“怎么调用”**的问题。

---

# 二、真正的分水岭：编译器知道不知道 `f` 的具体类型？

这是理解后面所有内容的关键。

例如：

```cpp
template<class Compare>
void algo(Compare comp)
{
    comp(1, 2);
}
```

当我们：

```cpp
Compare cmp;
algo(cmp);
```

模板实例化之后，`Compare` 已经不再是一个抽象的占位符，而变成了一个具体类型：

```cpp
void algo(Compare comp)
{
    comp(1, 2);
}
```

假设实际类型是：

```cpp
Cmp
```

那么编译器看到的实际上近似于：

```cpp
void algo(Cmp comp)
{
    comp(1, 2);
}
```

这意味着编译器知道：

> `comp` 到底是什么类型。

于是它也知道：

> `comp(1, 2)` 最终要调用哪个 `operator()`。

这就是**静态绑定（static binding）**。

所以：

```text
模板实例化
    ↓
具体类型确定
    ↓
operator() 的目标确定
    ↓
静态绑定
```

这里最重要的不是：

> “对象有没有传地址”

而是：

> **调用目标是否在编译期已经确定。**

---

# 三、调用目标确定以后，优化才有了基础

一旦编译器知道：

```cpp
comp(1, 2)
```

一定是在调用：

```cpp
Cmp::operator()(int, int)
```

它就有很多优化空间。

最典型的就是：

### 1. 内联

把：

```cpp
comp(1, 2);
```

直接展开成 `operator()` 的函数体。

例如：

```cpp
struct Add {
    int operator()(int a, int b) const {
        return a + b;
    }
};

Add add;
int x = add(1, 2);
```

可能直接被优化成：

```cpp
int x = 1 + 2;
```

甚至最终变成：

```cpp
int x = 3;
```

于是“函数调用”这个抽象动作完全消失。

### 2. 即使不内联，也通常可以直接调用

如果编译器选择不内联，那么它仍然知道：

```text
调用目标是谁
```

因此通常可以生成一个**直接调用**。

也就是说，它不需要运行时先问：

```text
“这个对象到底应该调用哪个函数？”
```

目标已经由编译/链接阶段确定了。

所以真正的因果关系是：

```text
具体类型已知
    ↓
调用目标可静态确定
    ↓
静态绑定
    ↓
可以直接调用
    ↓
进一步可以内联和优化
```

注意：

> “可以内联”不是语言保证，而是编译器优化的结果。

---

# 四、为什么虚函数是另一条路线？

考虑：

```cpp
struct Base {
    virtual void f();
};

struct Derived : Base {
    void f() override;
};

Base* p = ...;
p->f();
```

此时 `p` 的**静态类型**是：

```cpp
Base*
```

但运行时它可能指向：

```cpp
Derived
```

也可能指向其他继承自 `Base` 的类型。

因此编译器在一般情况下不能仅根据：

```cpp
Base* p
```

就确定：

```cpp
p->f()
```

到底调用哪一个 `f()`。

于是需要运行时根据对象的动态类型选择目标。

典型实现就是通过：

```text
对象
 ↓
vptr
 ↓
vtable
 ↓
目标函数地址
```

因此这里是：

```text
编译期不知道最终调用目标
        ↓
运行时决定
        ↓
动态绑定
        ↓
通常需要间接调用
```

所以，模板和虚函数真正的区别可以概括成：

```text
模板：
“我在编译期就知道你是什么。”

虚函数：
“我现在只知道你是 Base，
具体是谁，运行时再决定。”
```

---

# 五、因此，“静态多态”和“动态多态”的本质区别出现了

### 模板 + Function Object

```cpp
template<class F>
void algo(F f)
{
    f(x);
}
```

这里：

```text
F
↓
实例化时变成具体类型
↓
调用目标编译期确定
↓
静态多态
```

不同类型可以提供不同的：

```cpp
operator()
```

但不需要继承共同的基类。

这就是 C++ 泛型编程非常重要的一点：

> **只要你的类型满足所需要的接口，就可以参与算法。**

---

### 虚函数

```cpp
void algo(Base& obj)
{
    obj.f();
}
```

这里：

```text
Base&
↓
运行时可能绑定到不同派生类
↓
调用目标运行时决定
↓
动态多态
```

它的优势是：

> **程序运行过程中，可以根据实际对象类型改变行为。**

代价就是：

> **失去了部分编译期类型信息，因此通常需要运行时分派。**

---

# 六、`std::function` 又是什么？

`std::function` 和模板其实是另一种非常重要的对比。

例如：

```cpp
std::function<int(int)> f;
```

它可以装：

```text
普通函数
函数指针
lambda
function object
```

所以它故意把原来的具体类型隐藏起来。

例如：

```cpp
auto lambda = [](int x) {
    return x * 2;
};
```

lambda 本身有一个具体的、编译器生成的匿名类型。

而：

```cpp
std::function<int(int)> f = lambda;
```

相当于说：

> “我不在乎你原来到底是什么类型，只要你能够按照 `int(int)` 这个接口被调用就行。”

于是：

```text
原来的具体类型
      ↓
类型擦除
      ↓
std::function
      ↓
运行时保存如何调用它的信息
```

所以和模板相比，它牺牲了一部分编译期信息，换来了：

> **运行时统一保存和替换不同类型的可调用对象。**

因此这里的对比应该是：

```text
模板
→ 保留具体类型
→ 静态绑定
→ 编译器优化空间大

类型擦除 / 虚函数
→ 隐藏具体类型
→ 动态绑定
→ 获得运行时灵活性
```

---

# 七、还有一个完全独立的问题：对象的“状态”放在哪里？

这一点不要和“调用哪个函数”混在一起。

例如：

```cpp
struct GreaterThan {
    int threshold;

    bool operator()(int x) const {
        return x > threshold;
    }
};
```

对象：

```cpp
GreaterThan cmp{100};
```

这里有两个完全不同的问题：

### 问题 1：调用谁？

```cpp
cmp(x)
```

调用的是：

```cpp
GreaterThan::operator()(int)
```

因为类型是：

```cpp
GreaterThan
```

这是**调用机制**。

### 问题 2：`100` 放在哪里？

这是普通的**对象存储问题**。

它可能：

```text
栈上
寄存器
某块内存
被优化成常量
甚至被完全消除
```

这和“调用哪个函数”不是一个维度。

所以一定不要形成：

> “function object 有状态，所以调用就必须通过地址。”

这是错误的。

**状态的存储方式**和**函数调用目标的确定方式**是两件独立的事情。

---

# 八、最终把整个逻辑串起来

真正完整的因果链应该是：

```text
① `f(x)` 是函数调用表达式
        ↓
② `operator()` 允许普通对象获得函数调用能力
        ↓
③ 模板可以接受任意满足调用要求的类型
        ↓
④ 模板实例化时，具体类型被确定
        ↓
⑤ 因此调用目标可以在编译期确定
        ↓
⑥ 形成静态绑定
        ↓
⑦ 编译器可以直接调用 / 内联 / 进一步优化
```

另一条路线是：

```text
① 隐藏具体类型
        ↓
② 运行时才知道实际类型/目标
        ↓
③ 动态绑定
        ↓
④ 运行时进行分派
        ↓
⑤ 通常需要间接调用
```

于是最终形成：

```text
                    可调用对象
                        │
                使用统一调用语法
                    f(args...)
                        │
             ┌──────────┴──────────┐
             │                     │
        保留具体类型             隐藏具体类型
             │                     │
           模板                 虚函数/类型擦除
             │                     │
          编译期知道             运行时决定
             │                     │
          静态绑定              动态绑定
             │                     │
      直接调用 / 内联          间接调用 / 分派
             │                     │
         编译期优化              运行时灵活性
```

# 一句话真正应该记住的

> **C++ 的核心问题不是“对象传的是地址还是值”，而是“调用目标什么时候才能确定”。**
>
> 如果具体类型在编译期就确定了，调用目标通常也能静态确定，于是编译器拥有很大的优化空间；如果具体类型被隐藏，需要运行时才能确定目标，就必须通过动态分派获得灵活性。
>
> 而 function object 的 `operator()`，只是把“可调用行为”统一成了一种普通的 C++ 类型机制，使模板能够把“行为”作为类型的一部分传递给算法。

① C++ 允许对象重载 operator()
        ↓
② 对象因此可以表现得像函数
        ↓
③ 模板可以接受这种具体的 function object
        ↓
④ 模板实例化后，具体类型被保留下来
        ↓
⑤ 编译器知道具体的 operator()
        ↓
⑥ 调用目标可以静态确定
        ↓
⑦ 小而简单的 operator() 非常容易内联
        ↓
⑧ 函数调用的运行时开销被消除
        ↓
⑨ 最终只剩真正的业务操作
        ↓
⑩ 因此代码可能比“传一个函数/函数指针”
   更小、更快


lambda 并不是凭空出现的一种“特殊函数”，它其实是 C++ 用很漂亮的语法，让你方便地创建一个带行为、甚至带状态的对象。
lambda expression
    ↓
编译器生成一个匿名类类型
    ↓
创建这个类型的 closure object
    ↓
这个对象有 operator()
    ↓
所以它是一个 function object

---

## STL 总纲：Algorithm 只认 Iterator 的"能力"，不认它的"来历"

> 这是对第一版总纲的修正版。第一版最大的问题：把 `Category`（能力）和 `Adapter`（来历/实现）画成了 Iterator 下面并列的两个分支，容易让人以为"来历"也是一套需要穷举的分类体系。真正的关键区别是——**能力（Category）是一个封闭、有限的集合，算法只认这个；来历（谁提供这个能力）是一个开放、无限的集合，算法根本不关心，你也永远列不完。**

### 一、骨架不变，但要看清哪一层是"契约"

```
                Algorithm
                   │
          只声明需要哪种 Iterator 能力
                   │
                   ▼
                Iterator  ←────  这是 Algorithm 与 Data Source 之间唯一的契约
                   │
          只要满足这份契约，谁来实现都行
                   │
                   ▼
               Data Source
```

Algorithm 与 Iterator 之间的关系，本质上是"提需求"（我需要一个支持 `++`、`*`、`==` 的东西，最好还支持 `--`、`+n`……）；Iterator 与 Data Source 之间的关系，才是"谁来满足这个需求"——这一层，Algorithm 完全看不见，也不需要看见。

### 二、唯一需要死记的分类：Category（封闭集合）

```
Input → Output → Forward → Bidirectional → RandomAccess → Contiguous(C++20)
```

这六个是**算法真正检查的东西**。`std::sort` 要求 RandomAccess，`std::find` 要求 Input，`std::advance` 会按类别走不同分支——它检查的永远是"你满足哪个 Category"，不会问"你是谁写的、你包了什么"。

### 三、不需要死记的东西：Provenance（开放集合，算法不关心）

"这个能力从哪来"不是一个像 Category 那样的固定分类表，而是一个**永远列不完的开放集合**。常见的几种只是这个集合里的几个例子，不是全部：

| 能力提供者 | 举例 | 备注 |
|---|---|---|
| 容器原生迭代器 | `vector::iterator`、`list::iterator` | 最常见，但不是唯一 |
| 包装另一个迭代器 | `reverse_iterator`、`move_iterator`、`counted_iterator` | 官方叫 "iterator adaptor" |
| 包装一个容器的写接口 | `back_insert_iterator`、`insert_iterator` | 官方也归进 "iterator adaptor"，但包的是容器不是迭代器 |
| 包装一个流 | `istream_iterator`、`istreambuf_iterator` | 官方单独归为 "stream iterator"，跟上面两类分开列 |
| **不包装任何东西，纯生成** | 你自己写的、`operator*` 里现算一个值返回（比如递增计数器、斐波那契数列生成器、随机数流） | **没有"数据源"这个概念，数据是算出来的，不是读出来的** |
| 其他一切 | 网络流、传感器读数、数据库游标、惰性 range 的中间结果…… | 只要满足 Category，全部合法 |

最后一行"纯生成"是最能说明问题的例子：

```cpp
struct FibIterator {
    long a = 0, b = 1;
    long operator*() const { return a; }
    FibIterator& operator++() { long t = a + b; a = b; b = t; return *this; }
    bool operator==(const FibIterator&) const { return false; } // 无限序列
};
```

这个迭代器背后**没有任何容器、没有任何存储**，`*it` 每次都是现场计算出来的。它照样满足 Input Iterator（甚至 Forward，如果你保证多趟遍历结果一致）的全部语法要求，塞进 `std::copy_n(FibIterator{}, 10, std::ostream_iterator<long>(std::cout, " "))` 一样能跑。**这才是"数据源不重要"这句话的极限情况——数据源可以根本不存在。**

（cppreference 自己也是这么组织 `<iterator>` 页面的：`iterator adaptor`——包装另一个迭代器或容器——和 `stream iterator`——包装流——是分开列的两个小节，说明你之前"流迭代器好像不太一样"的直觉是对的；但这只是标准库文档一个方便查阅的分组，不是说这两类之外就没有别的可能了。）

### 四、Adapter 是"来历"集合里的一种常见手法，不是并列分类

上一版把 Adapter 单独列成一个跟 Category 平级的分支，这里做个更准的定位：**Adapter 是一种设计手法（复用已有实现、只换外部接口），它出现在 Provenance 这个开放集合里，而且不止出现在 Iterator 一处**：

```
Adapter 手法出现的三个地方：
├── 容器适配器  stack / queue / priority_queue   → 包一层 Container
├── 迭代器适配器 reverse / move / insert / counted → 包一层 Iterator 或 Container
└── 仿函数适配器 std::bind / std::not_fn           → 包一层 Function Object
```

它是"实现来源"里被验证好用、被反复使用的一类具体做法，跟"生成式迭代器""流迭代器""容器原生迭代器"是并列的几个例子之一，不是凌驾于它们之上的另一套分类体系。

### 五、C++20 Concepts：把"契约"从君子协定变成部分可机械验证

Category 这份契约其实拆成两半：

| 部分 | 能否被编译器验证 | 举例 |
|---|---|---|
| **语法结构**（有没有这个操作符、返回类型对不对） | ✅ C++20 concepts 可以机械检查 | `it++`、`*it`、`it == it2` 是否存在且类型正确 |
| **语义约定**（行为是否真的符合承诺） | ❌ 永远无法被编译器验证，是你自己的承诺 | Forward 要求"多趟遍历结果一致"（multipass guarantee）；`RandomAccess` 要求 `it+n` 花费常数时间——这些编译器测不出来 |

在 C++20 之前，你靠**手写声明**一个 tag（`using iterator_category = std::forward_iterator_tag;`）来"自称"某个类别，算法直接相信这个自称，不会验证。如果你谎报（比如声明了 `forward_iterator_tag` 但其实不支持多趟遍历），编译能过，但跑起来是未定义行为。C++20 的 `std::forward_iterator` 等 concept 至少把"语法结构"这部分从"自称"升级成"编译器检查"，但"语义约定"那部分，无论新旧标准，都只能算是文档层面的契约——这也是为什么 `std::ranges` 的很多 concept 命名后面还专门写了 "semantic requirements" 这一节，因为编译器管不到。

### 六、以后遇到 STL 新东西，只问两句话

1. **它满足哪个 Category？**（这是算法唯一关心的问题——决定它能被哪些算法使用、以什么效率）
2. **它的能力是从哪来的？**（这只是帮你自己理解它"长什么样"，算法完全不关心——可能是容器原生、包装了另一个迭代器/容器/流，也可能是纯生成、什么都不包）

不用再问"它属于哪个容器"，也不用把"来源"整理成一张跟 Category 平级的分类表——那张表永远不完整，因为你随时可以自己再发明一种。

### 七、附录：常见 Provenance 速查表（不是分类体系，只是举例）

**迭代器**

| 名称 | 提供者类型 | Category | 说明 |
|---|---|---|---|
| `vector/deque::iterator` | 容器原生 | Random Access（`vector` 可到 Contiguous） | 直接遍历容器 |
| `list::iterator` | 容器原生 | Bidirectional | |
| `forward_list::iterator` | 容器原生 | Forward | |
| `reverse_iterator<It>` | 包装另一迭代器 | 继承自 `It` | 反转 `++`/`--` |
| `move_iterator<It>` | 包装另一迭代器 | 继承自 `It` | 解引用返回右值引用 |
| `counted_iterator`（C++20） | 包装另一迭代器 | 继承 | 走 N 步就停 |
| `back_insert_iterator`/`front_insert_iterator`/`insert_iterator` | 包装一个容器 | 固定为 Output | 赋值即 push_back/push_front/insert |
| `istream_iterator`/`ostream_iterator` | 包装一个流 | 固定为 Input / Output | |
| `istreambuf_iterator`/`ostreambuf_iterator` | 包装流缓冲区 | Input / Output | 字符级，更底层 |
| 自定义生成式迭代器（如上面 `FibIterator`） | **不包装任何东西** | 你自己声明满足哪个 | 数据现算，没有存储 |

**容器适配器**（Adapter 手法用在 Container 上的例子）

| 名称 | 底层默认容器 | 说明 |
|---|---|---|
| `stack` | `deque` | LIFO，只留 push/pop/top |
| `queue` | `deque` | FIFO |
| `priority_queue` | `vector` | 堆 |
| `flat_map`/`flat_set`（C++23） | 有序 `vector` | 连续内存伪装关联容器接口 |

**仿函数适配器**（Adapter 手法用在 Function Object 上的例子）

| 名称 | 说明 |
|---|---|
| `std::bind`（C++11） | 固定/重排参数 |
| `std::mem_fn`（C++11） | 把成员函数指针包成可调用对象 |
| `std::not_fn`（C++17） | 对谓词取反 |

### 八、一句话总纲

> **STL 不要求 Algorithm 认识具体的 Data Source，它只要求 Data Source 通过 Iterator 提供一个满足 Category 的访问接口；至于这个接口背后是容器、是流、是另一个迭代器的包装，还是完全没有存储、临时算出来的一个值——Algorithm 不关心，也不应该关心。你能自己写一个满足条件的迭代器去接算法，正是这套设计"故意"允许、甚至鼓励的用法，而不是什么取巧或例外。**