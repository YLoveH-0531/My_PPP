#ifndef EXER_20_15_H
#define EXER_20_15_H

#include <vector>

template <typename T>
class Pvector {
 public:
  Pvector();
  ~Pvector() {
    for (auto p : v) delete p;
  }

 private:
  std::vector<T*> v;
};

#endif  // EXER_20_15_H