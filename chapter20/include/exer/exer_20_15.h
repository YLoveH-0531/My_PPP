#ifndef EXER_20_15_H
#define EXER_20_15_H

#include <vector>

template <typename T>
class Pvector : private std::vector<T *> {
  private:
    using Base = std::vector<T *>;

  public:
    using Base::begin;
    using Base::empty;
    using Base::end;
    using Base::push_back;
    using Base::size;
    using Base::operator[];
    using Base::reserve;
    Pvector() = default;
    ~Pvector() {
        for (auto p : *this)
            delete p;
    }
};

#endif // EXER_20_15_H