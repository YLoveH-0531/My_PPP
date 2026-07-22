#ifndef EXER_20_16_H
#define EXER_20_16_H

#include <utility>
#include <vector>

// 20.16: ovector —— 像 pvector（析构 delete 每个对象），
// 但 [] 和 * 返回所指对象的引用，而不是指针本身。
template <typename T>
class ovector : private std::vector<T*> {
private:
    using Base = std::vector<T*>;

public:
    using size_type = typename Base::size_type;

    // 语义不变、直接透出的接口
    using Base::begin;
    using Base::empty;
    using Base::end;
    using Base::push_back;
    using Base::reserve;
    using Base::size;

    ovector() = default;

    // 持有裸指针 + 析构 delete ⇒ rule of five。禁止拷贝（避免 double free / slicing）。
    ovector(const ovector&) = delete;
    ovector& operator=(const ovector&) = delete;

    // 移动构造：整体接管 other 的指针数组，other 退化为空 vector。
    ovector(ovector&& other) noexcept : Base(std::move(other)) {}

    // 移动赋值：先释放自己已持有的对象，再接管 other 的。
    ovector& operator=(ovector&& other) noexcept {
        if (this != &other) {
            for (T* p : *this)
                delete p;
            Base::operator=(std::move(other));
        }
        return *this;
    }

    ~ovector() {
        for (T* p : *this)
            delete p;
    }

    // [] 返回对象引用（而非指针）。const 版返回 T&：浅 const。
    T& operator[](size_type n)       { return *Base::operator[](n); }
    T& operator[](size_type n) const { return *Base::operator[](n); }

    // * 返回首元素所指对象的引用。
    T& operator*()       { return *Base::front(); }
    T& operator*() const { return *Base::front(); }

    template<typename...Args>
    void emplace_back(Args&&... args){
        Base::push_back(new T(std::forward<Args>(args)...));
    }
};
#endif // EXER_20_16_H