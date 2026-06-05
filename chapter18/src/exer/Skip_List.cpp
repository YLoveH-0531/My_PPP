/**
 * @file Skip_List.cpp
 * @author KaKaRot
 * @brief 
 * @version 0.1
 * @date 2026-06-04
 * 
 * @copyright Copyright (c) 2026
 */
 
#include "Skip_List.h"
#include <random>
#include <utility>
#include <iostream>

Skip_list::Skip_list()
: rng(std::random_device{}()), head(new node{0, MAX_LEVEL , new node*[MAX_LEVEL]{nullptr, nullptr, nullptr, nullptr, nullptr}}), height(0)
{ 
}

Skip_list::Skip_list(Skip_list&& lhs) noexcept : rng(lhs.rng), head(lhs.head), height(lhs.height)
{
    lhs.head = nullptr;
    lhs.height = 0;
}

Skip_list& Skip_list::operator=(Skip_list&& lhs) noexcept
{
    if (this == &lhs) {
        return *this;
    }
    destory();
    rng = lhs.rng;
    head = lhs.head;
    height = lhs.height;
    lhs.head = nullptr;
    lhs.height = 0;
    return *this;
}

Skip_list::~Skip_list() 
{
    destory();
}

void Skip_list::destory()
{
    if(!head) { return; }
    height = 0;
    node* p = head->succ[height];
    node* q = p;
    while (p) {
        q = p;
        p = p->succ[height];
        delete[] q->succ;
        delete q;
    }
    delete[] head->succ;
    delete head; 
}

node* Skip_list::insert(int value)
{
    node* update[MAX_LEVEL];
    std::pair<bool, node*> match= find(value, update);
    if (match.first) {
        return match.second;     // if found, do nothing
    }

    int level = createLevel();
    node** add_succ = new node*[level];
    node* add = new node{value, level, add_succ};
    for (int i = level -1; i >= 0; i--){
            add->succ[i] = update[i]->succ[i];
            update[i]->succ[i] = add;
    }
    return add;
}

std::pair<bool, node*> Skip_list::find(int value, node** update)
{
    // int current_level = height;
    // node* pc = head->succ[current_level];
    // node* prev = pc;
    // while (current_level >= 0) {  
    //     if(pc->data == value){
    //         return std::make_pair(true, pc);
    //     }
    //     else if (pc->data < value) {
    //         if (!pc->succ[current_level] && (--current_level < 0)) {
    //             break;
    //         }                
    //         prev = pc;
    //         pc = pc->succ[current_level];
    //     }
    //     else {
    //         if(--current_level >= 0){
    //             if(pc == prev){
    //                 pc = prev = head->succ[current_level];
    //             }else {
    //                 pc = prev->succ[current_level];
    //             }
    //         }else {
    //             break;
    //         }
    //     }
    // }
    // return std::make_pair(false, prev);
    int current_level = MAX_LEVEL;
    node* pc = head;
    while (--current_level >= 0) {
        while (pc->succ[current_level] != nullptr && pc->succ[current_level]->data < value ) {
            pc = pc->succ[current_level];
        }
        update[current_level] = pc;
    }
    node* candidate= pc->succ[0];
    if (candidate != nullptr && candidate->data == value) {
        return std::make_pair(true, candidate);
    }
    return std::make_pair(false, candidate);
}

int Skip_list::createLevel()
{
    int level = 1;
    std::bernoulli_distribution dis(0.5);
    while (level < MAX_LEVEL && dis(rng)) {
        level++;
    }
    return level;
}

node* Skip_list::at(int value){
    node* update[MAX_LEVEL];
    std::pair<bool, node*> match= find(value, update);
    if (match.first) {
        return match.second;
    }else {
        return nullptr;
    }
}

node* Skip_list::erase(int value)
{
    node* update[MAX_LEVEL];
    std::pair<bool, node*> match= find(value, update);
    if (!match.first) {
        return  nullptr;
    }

    // ============================================================
    // TODO(review by Claude): BUG —— heap-buffer-overflow（ASan 实测确认，本函数第 ~154 行）
    //
    // 问题：循环上界用 MAX_LEVEL-1（=4）。但要删的节点 match.second 的 succ 数组
    //       只有 match.second->height 个元素（它只存在于 level 0 ~ height-1）。
    //       当 i >= match.second->height 时：
    //         1) match.second->succ[i] 越界读（数组没那么大）→ ASan 报错点
    //         2) 那些层目标根本不存在，update[i]->succ[i] 也不指向它，乱改会破坏链表
    //       if(update[i]) 没用：update[i] 永远非空（至少是 head）。
    //       真正的条件是"目标在不在第 i 层" = "i < match.second->height"。
    //
    // 修法：把循环上界改成目标自己的高度，并去掉 if 判断：
    //       for (int i = match.second->height - 1; i >= 0; i--) {
    //           update[i]->succ[i] = match.second->succ[i];
    //       }
    // ============================================================
    for (int i = MAX_LEVEL - 1; i >= 0; i--) {
        if (update[i]) {
            update[i]->succ[i] = match.second->succ[i];
        }
    }
    delete[] match.second->succ;
    delete match.second;
    return update[0];
}

void Skip_list::print()
{
    // ============================================================
    // TODO(review by Claude): BUG —— 每层没把 p 重置回 head
    //
    // 问题：打完一层后，p 已经走到该层最右边的节点；进入下一层时 p 没回到 head，
    //       于是低层从"上一层结束的位置"接着往后走 → 低层几乎打不出东西。
    //       结果只有最高层能完整打印。
    //
    // 修法：在 while(--level >= 0) 循环体的开头，加一行把 p 拨回 head：
    //       while (--level >= 0) {
    //           p = head;                       // ← 每层从头开始
    //           std::cout << " level " << level + 1 << ": ";
    //           while (p->succ[level]) { ... }
    //       }
    // ============================================================
    int level = MAX_LEVEL;
    node* p = head;
    std::cout << "Skip list: \n";
    while (--level >= 0) {
        std::cout << " level " << level + 1 << ": ";
        while (p->succ[level]) {
            std::cout << p->succ[level]->data << " ";
            p = p->succ[level];
        }
        std::cout << '\n';
    }
}


  先用 ASan 跑（强烈建议，能抓内存 bug）：
  cd ~/personal/CPlusPlus/My_PPP/chapter18
  g++ -std=c++17 -g -fsanitize=address,undefined -I include/exer \
      src/exer/exer_18_11.cpp src/exer/Skip_List.cpp -o test_skiplist && ./test_skiplist

  判断成功的标准：
  - ✅ 末尾显示 PASS: XX   FAIL: 0
  - ✅ ASan 全程无任何红色报错
  - ✅ Test 7 打印里，最底层那行升序列出 10 20 30 40 50 全部 5 个（修好 print 的标志）

  如果哪个 FAIL 或 ASan 报错，把输出贴给我，我帮你看。

  ---
  辛苦了，下班路上可以想想那两处为什么这么改（尤其 erase 的"用目标高度而非 MAX_LEVEL"）。回家改完发我结果 💪 回头见。
