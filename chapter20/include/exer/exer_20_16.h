#ifndef EXER_20_16_H
#define EXER_20_16_H

#include <vector>

template <typename T>
class ovector : private std::vector<T*> {
private:
    using Base = std::vector<T*>;

public:
    using Base::begin;
    using Base::empty;
    using Base::end;
    using Base::push_back;
    using Base::size;
    using Base::reserve;

    ovector() = default;
    ovector(const ovector&) = delete;
    ovector& operator=(const ovector&) = delete;

    ovector(ovector&& other) noexcept : Base(std::move(other)) {}
    ovector& operator=(ovector&& other) noexcept {
        Base::operator=(std::move(other));
        return *this;
    }
    ~ovector(){
        for (auto p : *this)
            delete p;
    }
    T& operator[](std::size_t n) { return *Base::operator[](n); }
    T& operator[](std::size_t n) const { return *Base::operator[](n); }

    T& operator*() { return *Base::front();}
    T& operator*() const { return *Base::operator*(); }
    

};
#endif // EXER_20_16_H