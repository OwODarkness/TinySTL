#ifndef TINYSTL_DELETER_H
#define TINYSTL_DELETER_H

namespace tinystl{
    template<typename T>
    struct default_delete{
        default_delete() = default;
        void operator()(T* p)
        {
            delete p;
        }
    };

    template<typename T>
    struct default_delete<T[]>{
        void operator()(T* p)
        {
            delete[] p;
        }
    };

}

#endif //TINYSTL_DELETER_H