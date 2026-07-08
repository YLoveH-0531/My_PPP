#pragma once

template<typename Elem>
struct Link {
    Link* succ;
    Elem  val;
};

template<typename Elem>
class slist {
    Link<Elem> link;   // 哨兵：sentinel.succ → 首节点，尾节点.succ → sentinel
public:

    // ── iterator ──────────────────────────────────────────────────────────
    class iterator {
        Link<Elem>* curr;
        friend class slist<Elem>;
    public:
        iterator(Link<Elem>* p) : curr{p} {}
        iterator& operator++() { curr = curr->succ; return *this; }
        Elem& operator*() { return curr->val; }
        bool operator==(const iterator& b) const { return curr == b.curr; }
        bool operator!=(const iterator& b) const { return curr != b.curr; }
    };

    // ── 构造 / 析构 ────────────────────────────────────────────────────────
    slist() { link.succ = &link; }

    ~slist() {
        Link<Elem>* curr = link.succ;
        while (curr != &link) {          // FIX1: 原来 while(curr) 会 delete 栈上哨兵
            Link<Elem>* next = curr->succ;
            delete curr;
            curr = next;
        }
    }

    // ── 迭代器 ─────────────────────────────────────────────────────────────
    iterator before_begin() { return iterator(&link); }  // 哨兵迭代器，用于插入/删除头节点
    iterator begin()        { return iterator(link.succ); }
    iterator end()          { return iterator(&link); }

    // ── 插入 / 删除 ────────────────────────────────────────────────────────
    // 在 p 之后插入 v
    iterator insert_after(iterator p, const Elem& v) {
        Link<Elem>* n = new Link<Elem>{p.curr->succ, v};  // FIX2: 直接用正确的 succ
        p.curr->succ = n;
        return iterator(n);
    }

    // 删除 p 之后的节点，返回再下一个节点的迭代器
    iterator erase_after(iterator p) {
        Link<Elem>* to_delete = p.curr->succ;  // FIX3: 存指针，不能 delete iterator 对象
        p.curr->succ = to_delete->succ;
        delete to_delete;
        return iterator(p.curr->succ);
    }

    void push_front(const Elem& v) { insert_after(before_begin(), v); }  // FIX4: 插哨兵之后

    void push_back(const Elem& v) {                                        // FIX5: 遍历到尾节点
        auto it = before_begin();
        while (it.curr->succ != &link) ++it;
        insert_after(it, v);
    }

    void pop_front() { erase_after(before_begin()); }                      // FIX4: 同 push_front

    void pop_back() {                                                       // FIX6: 不能用 --end()
        auto it = before_begin();
        while (it.curr->succ->succ != &link) ++it;  // 找倒数第二个节点
        erase_after(it);
    }

    // ── 访问 ───────────────────────────────────────────────────────────────
    Elem& front() { return *begin(); }

    Elem& back() {                                                          // FIX6: 遍历到尾节点
        Link<Elem>* curr = link.succ;
        while (curr->succ != &link) curr = curr->succ;
        return curr->val;
    }
};
