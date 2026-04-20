#include <iostream>

void swap_v(int a, int b)
{
    int temp = a;
    a = b;
    b = temp;
}

void swap_r(int& a, int& b)
{
    int temp = a;
    a = b;
    b = temp;
}

void swap_cr(const int&, const int&)
{
    // This function cannot swap the values of a and b because they are const references.
    // The following code would cause a compilation error:
    // int temp = a;
    // a = b;
    // b = temp;
}

int main()
{
    // test one
    int x = 7;
    int y = 9;
    swap_v(x, y);
    std::cout << "swap_v: x = " << x << ", y = " << y << std::endl; // x and y remain unchanged 
    swap_r(x, y);
    std::cout << "swap_r: x = " << x << ", y = " << y << std::endl; // x and y are swapped
    swap_cr(x, y);
    std::cout << "swap_cr: x = " << x << ", y = " << y << std::endl; // x and y remain unchanged
    
    swap_v(7, 9);    // This will compile but will not swap the values of 7 and 9 because they are passed by value
    // swap_r(7, 9); // This will not compile for the same reason as above
    swap_cr(7, 9);   // This will compile because we can bind const references to rvalues, but it will not swap the values of 7 and 9

    // test two
    const int cx = 7;
    const int cy = 9;
    swap_v(cx, cy); // This will compile but will not swap the values of cx and cy because they are passed by value
    // swap_r(cx, cy); // This will not compile because we cannot bind non-const references to const variables
    swap_cr(cx, cy); // This will compile because we can bind const references to const variables, but it will not swap the values of cx and cy

    // test three
    double dx = 7.7;
    double dy = 9.9;
    swap_v(dx, dy); // This will compile but will not swap the values of dx and dy because they are passed by value
    //swap_r(dx, dy); // int& 不能绑定到 double 类型的变量，因为绑定时需要隐式转换，转换会产生临时对象，而非 const
                      // 的左值引用不能绑定到临时对象。
    swap_cr(dx, dy); // This will compile because we can bind const references to non-const variables, but it will not swap the values of dx and dy
    
    swap_v(7.7, 9.9); // This will compile but will not swap the values of 7.7 and 9.9 because they are passed by value
    // swap_r(7.7, 9.9); // This will not compile because we cannot bind non-const references to rvalues
    swap_cr(7.7, 9.9); // This will compile because we can bind const references to rvalues, but it will not swap the values of 7.7 and 9.9



    return 0;
}