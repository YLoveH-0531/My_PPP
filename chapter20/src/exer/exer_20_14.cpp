/**
 * @file exer_20_14.cpp
 * @author KaKaRot
 * @brief
 * @version 0.1
 * @date 2026-07-08
 *
 * @copyright Copyright (c) 2026
 */

#include "exer_20_14.h"
#include <cassert>
#include <iostream>
#include <string>

template<typename Elem>
void print(const std::string& label, slist<Elem>& l)
{
    std::cout << "  " << label << ": [";
    for (auto it = l.begin(); it != l.end(); ++it) {
        std::cout << *it;
        auto next = it; ++next;
        if (next != l.end()) std::cout << ", ";
    }
    std::cout << "]\n";
}

// ══════════════════════════════════════════════════════════════
//  push_front / push_back / 迭代器顺序
// ══════════════════════════════════════════════════════════════
static void test_push_and_iterate()
{
    std::cout << "── push / iterate ──\n";

    slist<int> l;
    l.push_back(1); l.push_back(2); l.push_back(3);
    print("push_back 1,2,3", l);

    int exp[] = {1, 2, 3};
    int i = 0;
    for (auto it = l.begin(); it != l.end(); ++it, ++i)
        assert(*it == exp[i]);
    std::cout << "  [PASS] push_back 顺序正确\n";

    slist<int> l2;
    l2.push_front(3); l2.push_front(2); l2.push_front(1);
    print("push_front 3,2,1", l2);

    i = 0;
    for (auto it = l2.begin(); it != l2.end(); ++it, ++i)
        assert(*it == exp[i]);
    std::cout << "  [PASS] push_front 顺序正确\n";
}

// ══════════════════════════════════════════════════════════════
//  front / back
// ══════════════════════════════════════════════════════════════
static void test_front_back()
{
    std::cout << "── front / back ──\n";

    slist<int> l;
    l.push_back(10); l.push_back(20); l.push_back(30);

    assert(l.front() == 10);
    assert(l.back()  == 30);
    std::cout << "  [PASS] front=10, back=30\n";

    l.front() = 100;
    l.back()  = 300;
    assert(l.front() == 100 && l.back() == 300);
    std::cout << "  [PASS] front/back 可写\n";
}

// ══════════════════════════════════════════════════════════════
//  pop_front / pop_back
// ══════════════════════════════════════════════════════════════
static void test_pop()
{
    std::cout << "── pop_front / pop_back ──\n";

    slist<int> l;
    l.push_back(1); l.push_back(2); l.push_back(3);

    l.pop_front();
    assert(l.front() == 2);
    std::cout << "  [PASS] pop_front → front 变为 2\n";

    l.pop_back();
    assert(l.back() == 2);
    std::cout << "  [PASS] pop_back  → back  变为 2\n";

    l.pop_front();
    assert(l.begin() == l.end());
    std::cout << "  [PASS] 弹完后 begin()==end()\n";
}

// ══════════════════════════════════════════════════════════════
//  insert_after / erase_after
// ══════════════════════════════════════════════════════════════
static void test_insert_erase()
{
    std::cout << "── insert_after / erase_after ──\n";

    slist<int> l;
    l.push_back(1); l.push_back(3);

    auto it = l.begin();           // 指向 1
    l.insert_after(it, 2);         // 在 1 后插入 2 → 1,2,3
    print("insert_after(1, 2)", l);

    int exp[] = {1, 2, 3};
    int i = 0;
    for (auto p = l.begin(); p != l.end(); ++p, ++i)
        assert(*p == exp[i]);
    std::cout << "  [PASS] insert_after 后顺序 1,2,3\n";

    it = l.begin();                // 指向 1
    l.erase_after(it);             // 删 1 后的节点(2) → 1,3
    print("erase_after(1)", l);
    assert(l.front() == 1 && l.back() == 3);
    std::cout << "  [PASS] erase_after 后剩 1,3\n";
}

// ══════════════════════════════════════════════════════════════
//  单元素边界
// ══════════════════════════════════════════════════════════════
static void test_single_element()
{
    std::cout << "── 单元素边界 ──\n";

    slist<int> l;
    l.push_back(42);
    assert(l.front() == 42 && l.back() == 42);
    std::cout << "  [PASS] 单元素 front==back==42\n";

    l.pop_back();
    assert(l.begin() == l.end());
    std::cout << "  [PASS] pop_back 后列表为空\n";
}

// ══════════════════════════════════════════════════════════════
//  string 类型
// ══════════════════════════════════════════════════════════════
static void test_string()
{
    std::cout << "── string 类型 ──\n";

    slist<std::string> l;
    l.push_back("hello");
    l.push_back("world");
    assert(l.front() == "hello" && l.back() == "world");
    std::cout << "  [PASS] string push_back/front/back\n";
}

int main()
{
    test_push_and_iterate();
    std::cout << '\n';
    test_front_back();
    std::cout << '\n';
    test_pop();
    std::cout << '\n';
    test_insert_erase();
    std::cout << '\n';
    test_single_element();
    std::cout << '\n';
    test_string();
    std::cout << "\n所有测试通过！\n";
    return 0;
}
