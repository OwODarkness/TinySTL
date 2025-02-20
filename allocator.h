#ifndef TINYSTL_ALLOCATOR_H
#define TINYSTL_ALLOCATOR_H

#include <iostream>

namespace tinystl
{
    template <typename T>
    class Allocator
    {
    public:
        using value_type = T;
        using pointer = T *;
        using const_pointer = const T*;
        using size_type = size_t;

        static pointer allocate(size_type n = 1);

        static void deallocate(pointer p, size_type n = 1);

        static void construct(pointer p);
        static void construct(pointer p, const T&q);
        static void construct(pointer p, T&& q);
        template<class... Args> static void construct(pointer p, Args&&... args);

        static void destroy(pointer p);
    };

    template<typename T>
    Allocator<T>::pointer Allocator<T>::allocate(size_type n)
    {
        return n<=0 ? nullptr : reinterpret_cast<pointer>(::operator new(n * sizeof(value_type)));
    }

    template<typename T>
    void Allocator<T>::deallocate(pointer p, size_type n)
    {
        if(p != nullptr)
        {
            ::operator delete(p);
        }
    }

    template<typename T>
    void Allocator<T>::construct(pointer p)
    {
        new(p) T();
    }

    template<typename T>
    void Allocator<T>::construct(pointer p, const T&q)
    {
        new(p) T(q);
    }

    template<typename T>
    void Allocator<T>::construct(pointer p, T&& q)
    {
        new(p) T(q);
    }

    template<typename T>
    template<class... Args>
    void Allocator<T>::construct(pointer p, Args&&... args)
    {
        new(p) T(std::forward(args)...);
    }

    template<typename T>
    void Allocator<T>::destroy(pointer p)
    {
        if(p != nullptr)
        {
            p->~T();
        }
    }
};

#endif // TINYSTL_ALLOCATOR_H