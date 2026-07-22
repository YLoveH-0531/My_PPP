/**
 * @file exer_20_16.cpp
 * @author KaKaRot
 * @brief 验证 ovector：访问语义（[] / *）、emplace_back 完美转发、移动语义、无泄漏/无 double free
 * @version 0.3
 * @date 2026-07-10
 *
 * @copyright Copyright (c) 2026
 */

#include "exer_20_16.h"

#include <cassert>
#include <iostream>
#include <string>
#include <utility>

// 带计数的探针类型：统计存活对象数，用来检测泄漏 / double free。
struct Tracked {
    int value;
    std::string tag;
    static int alive;
    static int copies; // 记录拷贝构造次数，用于验证 emplace 的完美转发未产生多余拷贝

    explicit Tracked(int v, std::string t = "") : value(v), tag(std::move(t)) { ++alive; }
    Tracked(const Tracked& o) : value(o.value), tag(o.tag) { ++alive; ++copies; }
    ~Tracked() { --alive; }

    Tracked& operator=(const Tracked&) = delete;
};

int Tracked::alive = 0;
int Tracked::copies = 0;

int main() {
    // ---- 1. push_back + [] 读写 ----
    {
        ovector<Tracked> ov;
        ov.push_back(new Tracked(10));
        ov.push_back(new Tracked(20));
        ov.push_back(new Tracked(30));

        assert(ov.size() == 3);
        assert(!ov.empty());
        assert(Tracked::alive == 3);

        assert(ov[0].value == 10);
        ov[1].value = 99;            // [] 返回引用 → 可写
        assert(ov[1].value == 99);
    }
    assert(Tracked::alive == 0);     // 作用域结束，全部释放

    // ---- 2. operator*（首元素引用）读写 + const 访问 ----
    {
        ovector<Tracked> ov;
        ov.push_back(new Tracked(1));
        ov.push_back(new Tracked(2));

        assert((*ov).value == 1);
        (*ov).value = 100;
        assert(ov[0].value == 100);

        const ovector<Tracked>& cov = ov;   // 浅 const：对象仍可读
        assert(cov[1].value == 2);
        assert((*cov).value == 100);
    }
    assert(Tracked::alive == 0);

    // ---- 3. emplace_back：单参 / 多参 / 完美转发右值 ----
    {
        ovector<Tracked> ov;
        ov.emplace_back(7);                          // 单参 → new Tracked(7)
        std::string s = "moved";
        ov.emplace_back(8, std::move(s));            // 多参 + 右值转发
        ov.emplace_back(9, "literal");               // const char* → std::string

        assert(ov.size() == 3);
        assert(Tracked::alive == 3);
        assert(ov[0].value == 7 && ov[0].tag == "");
        assert(ov[1].value == 8 && ov[1].tag == "moved");
        assert(ov[2].value == 9 && ov[2].tag == "literal");
        assert(s.empty());                           // 被 move 走，源被掏空
        assert(Tracked::copies == 0);                // 全程无 Tracked 拷贝构造
    }
    assert(Tracked::alive == 0);

    // ---- 4. 移动构造：所有权转移，不新增/销毁对象 ----
    {
        ovector<Tracked> src;
        src.emplace_back(11);
        src.emplace_back(22);
        assert(Tracked::alive == 2);

        ovector<Tracked> dst = std::move(src);
        assert(dst.size() == 2);
        assert(src.empty());                         // 源退化为空
        assert(Tracked::alive == 2);                 // 没有额外构造/析构
        assert(dst[1].value == 22);
    }
    assert(Tracked::alive == 0);

    // ---- 5. 移动赋值：目标原有对象必须先释放（回归测试原 bug）----
    {
        ovector<Tracked> a;
        a.emplace_back(1);
        a.emplace_back(2);

        ovector<Tracked> b;
        b.emplace_back(3);
        b.emplace_back(4);
        b.emplace_back(5);
        assert(Tracked::alive == 5);

        a = std::move(b);                            // a 原有的 1,2 应被 delete
        assert(Tracked::alive == 3);                 // 若漏 delete 则为 5（泄漏）
        assert(a.size() == 3);
        assert(a[0].value == 3);
        assert(b.empty());
    }
    assert(Tracked::alive == 0);

    // ---- 6. 移动自赋值：安全（不销毁自身内容）----
    {
        ovector<Tracked> a;
        a.emplace_back(42);
        ovector<Tracked>& self = a;                  // 经引用别名，绕开 -Wself-move
        a = std::move(self);                         // this == &other 分支
        assert(Tracked::alive == 1);
        assert(a.size() == 1);
        assert(a[0].value == 42);
    }
    assert(Tracked::alive == 0);

    // ---- 7. 迭代器遍历（注意：解引用得到 T*，与 [] 的 T& 不同）----
    {
        ovector<Tracked> ov;
        ov.emplace_back(1);
        ov.emplace_back(2);
        ov.emplace_back(3);

        int sum = 0;
        for (Tracked* p : ov)                        // begin/end 透出的是 vector<T*> 迭代器
            sum += p->value;
        assert(sum == 6);
    }
    assert(Tracked::alive == 0);

    std::cout << "all checks passed; alive=" << Tracked::alive
              << " copies=" << Tracked::copies << '\n';
}
