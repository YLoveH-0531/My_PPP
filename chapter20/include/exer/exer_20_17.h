#ifndef EXER_20_17_H
#define EXER_20_17_H

#include <cstddef>
#include <utility>
#include <vector>

// 20.17: ownership_vector —— 像 pvector 一样持有对象指针，
// 但由用户对每个元素单独决定是否由容器拥有（析构时是否 delete）。
template <typename T>
class ownership_vector {
public:
    using size_type = std::size_t;

    ownership_vector() = default;

    // 持有裸指针 + 析构按需 delete ⇒ rule of five。
    // 禁止拷贝：逐值复制会让两个容器都以为自己拥有同一指针 → double free。
    ownership_vector(const ownership_vector&) = delete;
    ownership_vector& operator=(const ownership_vector&) = delete;

    // 移动构造：整体接管 other 的指针数组与所有权标志，other 退化为空。
    ownership_vector(ownership_vector&& other) noexcept
        : data(std::move(other.data)), own(std::move(other.own)) {}

    // 移动赋值：先释放自己拥有的对象，再接管 other 的。
    ownership_vector& operator=(ownership_vector&& other) noexcept {
        if (this != &other) {
            release();
            data = std::move(other.data);
            own = std::move(other.own);
        }
        return *this;
    }

    ~ownership_vector() { release(); }

    // 用户在这里决定该元素是否被容器拥有。
    void add(T* p, bool owned) {
        data.push_back(p);
        own.push_back(owned);
    }

    // 像 pvector 一样返回指针本身（浅 const）。
    T* operator[](size_type n)       { return data[n]; }
    T* operator[](size_type n) const { return data[n]; }

    size_type size() const { return data.size(); }
    bool empty() const { return data.empty(); }

    // 查询某元素是否由容器拥有。
    bool owns(size_type n) const { return own[n]; }

private:
    std::vector<T*> data;
    std::vector<bool> own;

    // 只删除标记为“拥有”的元素。
    void release() {
        for (size_type i = 0; i < data.size(); ++i)
            if (own[i])
                delete data[i];
    }
};

#endif // EXER_20_17_H
