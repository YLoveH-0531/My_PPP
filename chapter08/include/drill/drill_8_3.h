#include <iostream>

namespace X{
    int var;
    void print(){
        std::cout << "X::var = " << var << std::endl;
    }
};

namespace Y{
    int var;
    void print()
    {
        std::cout << "Y::var = " << var << std::endl;
    }
};

namespace Z{
    int var;
    void print()
    {
        std::cout << "Z::var = " << var << std::endl;
    }
};
