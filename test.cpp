#include <iostream>
#include "functional.h"
#include "memory.h"

tinystl::weak_ptr<int> gw;

void observe()
{
    std::cout << "use count = " << gw.use_count() << "; ";
    if (tinystl::shared_ptr<int> spt = gw.lock())
    {
        std::cout << "*spt = " << *spt;
    }
    else
    {
        std::cout << "gw is expired";
    }
    std::cout << "\n";
}

int main(int argc, char **argv)
{
    {

        tinystl::shared_ptr<int> sp = tinystl::make_shared<int>(42);
        gw = sp;
        observe();
    }
    observe();
    return 0;
}
