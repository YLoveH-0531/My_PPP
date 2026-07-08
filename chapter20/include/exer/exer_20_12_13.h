#ifndef EXER_20_12_13_H
#define EXER_20_12_13_H

template<typename Elem>
struct Link {
    Link* prev;
    Link* succ;
    Elem val;
};

template<typename Elem>
class list {
    Link<Elem> link;   // 哨兵节点：link.succ → 首节点，link.prev → 尾节点
public:

    // ── iterator ──────────────────────────────────────────────────────────
    // 先于所有使用它的方法定义，避免不完整类型问题
    class iterator {
        Link<Elem>* curr;
        friend class list<Elem>;   // list 需要访问 curr
    public:
        iterator(Link<Elem>* p) : curr{p} {}
        iterator& operator++() { curr = curr->succ; return *this; }
        iterator& operator--() { curr = curr->prev; return *this; }
        Elem& operator*() { return curr->val; }
        bool operator==(const iterator& b) const { return curr == b.curr; }
        bool operator!=(const iterator& b) const { return curr != b.curr; }
    };

    // ── 构造 / 析构 ────────────────────────────────────────────────────────
    list() { link.prev = &link; link.succ = &link; }

    ~list() {
        Link<Elem>* curr = link.succ;
        while (curr != &link) {
            Link<Elem>* next = curr->succ;
            delete curr;
            curr = next;
        }
    }

    // ── 迭代器 ─────────────────────────────────────────────────────────────
    iterator begin() { return iterator(link.succ); }
    iterator end()   { return iterator(&link); }

    // ── 插入 / 删除 ────────────────────────────────────────────────────────
    // 在 p 所指节点之前插入 v，返回新节点迭代器
    iterator insert(iterator p, const Elem& v) {
        Link<Elem>* n = new Link<Elem>{p.curr->prev, p.curr, v};
        p.curr->prev->succ = n;
        p.curr->prev = n;
        return iterator(n);
    }

    // 删除 p 所指节点，返回下一节点迭代器
    iterator erase(iterator p) {
        p.curr->prev->succ = p.curr->succ;
        p.curr->succ->prev = p.curr->prev;
        iterator next(p.curr->succ);
        delete p.curr;
        return next;
    }

    void push_back(const Elem& v)  { insert(end(), v); }
    void push_front(const Elem& v) { insert(begin(), v); }
    void pop_front() { erase(begin()); }
    void pop_back()  { erase(--end()); }

    // ── 访问 ───────────────────────────────────────────────────────────────
    Elem& front() { return *begin(); }
    Elem& back()  { return *(--end()); }
};

// ── 通用算法：返回 [first, last) 中最大元素的迭代器 ──────────────────────
// 原版用了未定义的 Iterator，修正为 Iter；变量名改为 h 避免与函数名遮蔽
template<typename Iter>
Iter high(Iter first, Iter last)
{
    Iter h = first;
    for (Iter p = first; p != last; ++p)
        if (*h < *p)
            h = p;
    return h;
}

#endif //EXER_20_12_13_H
