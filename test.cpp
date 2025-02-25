#include <iostream>
#include "functional.h"

void test(int a, int b)
{
    std::cout << a << " " << b << "\n";
}

int main(int argc, char** argv){
    tinystl::function_v2<int(int, int)> f = [](int a, int b){return a + b;};

    //tinystl::function<void(int, int)> f = test;
    
    //tinystl::function_v2<void(int, int)> f = test;
    std::cout << f(2, 3);
    return 0;
}
