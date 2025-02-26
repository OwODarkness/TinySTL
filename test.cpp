#include <iostream>
#include "functional.h"
#include "memory.h"

int main(int argc, char** argv){
    tinystl::shared_ptr<int> p(new int(3));

    return 0;
}
