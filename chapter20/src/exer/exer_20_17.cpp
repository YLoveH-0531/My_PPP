/**
 * @file exer_20_17.cpp
 * @author KaKaRot
 * @brief 验证 ownership_vector：按元素决定所有权（析构只删 owned）、访问语义、
 *        移动语义、无泄漏 / 无 double free / 不误删 not-owned。
 * @version 0.1
 * @date 2026-07-22
 *
 * @copyright Copyright (c) 2026
 */

#include "exer_20_17.h"

#include <cassert>
#include <iostream>
#include <utility>

// 带计数的探针类型：统计存活对象数，用来检测泄漏 / double free / 误删。
struct Tracked {
    int value;
    static int alive;

    explicit Tracked(int v) : value(v) { ++alive; }
    ~Tracked() { --alive; }

    Tracked(const Tracked&) = delete;
    Tracked& operator=(const Tracked&) = delete;
};

int Tracked::alive = 0;

int main() {
    // ---- 1. add + 访问接口 + 全部 owned ----
    {
        ownership_vector<Tracked> ov;
        ov.add(new Tracked(10), true);
        ov.add(new Tracked(20), true);

        assert(ov.size() == 2);
        assert(!ov.empty());
        assert(Tracked::alive == 2);

        assert(ov[0]->value == 10);   // [] 返回指针
        ov[1]->value = 99;
        assert(ov[1]->value == 99);
        assert(ov.owns(0) && ov.owns(1));
    }
    assert(Tracked::alive == 0);      // 全 owned → 析构全删

    // ---- 2. 核心：析构只删 owned，not-owned 不动 ----
    {
        Tracked external(100);        // 栈对象，由其自身作用域管理
        assert(Tracked::alive == 1);
        {
            ownership_vector<Tracked> ov;
            ov.add(new Tracked(1), true);   // owned  → 会被删
            ov.add(&external, false);       // not-owned → 不删

            assert(ov.size() == 2);
            assert(Tracked::alive == 2);    // external + heap(1)
            assert(ov.owns(0) == true);
            assert(ov.owns(1) == false);
        }
        // ov 析构：只 delete 了 owned 的 heap(1)
        assert(Tracked::alive == 1);        // external 仍存活
        assert(external.value == 100);      // 未被破坏
    }
    assert(Tracked::alive == 0);            // external 随其作用域析构

    // ---- 3. const 访问（浅 const）----
    {
        ownership_vector<Tracked> ov;
        ov.add(new Tracked(7), true);
        const ownership_vector<Tracked>& cov = ov;
        assert(cov.size() == 1);
        assert(cov[0]->value == 7);
        assert(cov.owns(0));
    }
    assert(Tracked::alive == 0);

    // ---- 4. 移动构造：所有权（含标志）整体转移，无额外构造/析构 ----
    {
        ownership_vector<Tracked> src;
        src.add(new Tracked(11), true);
        src.add(new Tracked(22), true);
        assert(Tracked::alive == 2);

        ownership_vector<Tracked> dst = std::move(src);
        assert(dst.size() == 2);
        assert(src.empty());                // 源退化为空
        assert(Tracked::alive == 2);        // 无额外构造/析构
        assert(dst[1]->value == 22);
        assert(dst.owns(0) && dst.owns(1)); // 所有权标志随之转移
    }
    assert(Tracked::alive == 0);

    // ---- 5. 移动赋值：目标原有 owned 对象必须先释放（回归 double-free/泄漏）----
    {
        ownership_vector<Tracked> a;
        a.add(new Tracked(1), true);
        a.add(new Tracked(2), true);

        ownership_vector<Tracked> b;
        b.add(new Tracked(3), true);
        assert(Tracked::alive == 3);

        a = std::move(b);                   // a 原有 1,2 应被 delete
        assert(Tracked::alive == 1);        // 若漏删则为 3（泄漏）
        assert(a.size() == 1);
        assert(a[0]->value == 3);
        assert(b.empty());
    }
    assert(Tracked::alive == 0);

    // ---- 6. 移动赋值时目标持有 not-owned 元素：不得误删 ----
    {
        Tracked external(77);
        {
            ownership_vector<Tracked> a;
            a.add(&external, false);        // not-owned
            a.add(new Tracked(9), true);    // owned

            ownership_vector<Tracked> b;
            b.add(new Tracked(5), true);
            assert(Tracked::alive == 3);    // external + heap9 + heap5

            a = std::move(b);               // a.release() 只删 heap9，external 不动
            assert(Tracked::alive == 2);    // external + heap5
            assert(external.value == 77);
            assert(a.size() == 1);
            assert(a[0]->value == 5);
        }
        assert(Tracked::alive == 1);        // external 还在
    }
    assert(Tracked::alive == 0);

    // ---- 7. 移动自赋值：安全（不销毁自身内容）----
    {
        ownership_vector<Tracked> a;
        a.add(new Tracked(42), true);
        ownership_vector<Tracked>& self = a;  // 经引用别名，绕开 -Wself-move
        a = std::move(self);                   // this == &other 分支
        assert(Tracked::alive == 1);
        assert(a.size() == 1);
        assert(a[0]->value == 42);
    }
    assert(Tracked::alive == 0);

    std::cout << "all checks passed; alive=" << Tracked::alive << '\n';
}
