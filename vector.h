#ifndef TINY_STL_VECTOR_H
#define TINY_STL_VECTOR_H

#include <iostream>

namespace tinystl{
    template<typename T>
    class Vector{
    public:
        Vector() = default;

        template<typename U>
        friend std::ostream& operator<<(std::ostream& out, const Vector<U>& v);

        inline std::size_t Size() const noexcept {return size_;}
    private:
        T* data_;
        std::size_t size_;    
    };
}

template<typename U>
std::ostream& operator<<(std::ostream& out, const tinystl::Vector<U>& v)
{
    if(nullptr ==  v)
    {
        return out;
    }
    for(std::size_t i = 0;i<v.size_ - 1;i++)
    {
        out << v.data_[i] << " ";
    }
    out << v.data_[v.size_ - 1] << "\n";
    return out;
}

#endif