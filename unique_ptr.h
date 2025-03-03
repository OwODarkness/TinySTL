#ifndef TINYSTL_UNIQUE_PTR_H
#define TINYSTL_UNIQUE_PTR_H

#include "deleter.h"

namespace tinystl{
    template<typename T, typename Deleter = default_delete<T>>
    class unique_ptr{
    public:
        using value_type = T;
        using pointer = T*;
        
        explicit unique_ptr();
        explicit unique_ptr(const pointer p);

        //unique_ptr can no be copied
        unique_ptr(const unique_ptr& p) = delete;
        unique_ptr(unique_ptr&& p);
        
        unique_ptr& operator=(const unique_ptr& p) = delete;
        unique_ptr& operator=(unique_ptr&& p);

        inline pointer get() const{return data_;}
        inline Deleter& get_deleter() const noexcept {return deleter_;}

        value_type& operator*(){return *data_;}
        pointer operator->() const{return data_;}
        operator bool() const{return data_ != nullptr;}

        ~unique_ptr();

        template<typename U, typename... VARS>
        friend unique_ptr<U> make_unique(VARS... );
        
    private:
        pointer data_;
        Deleter deleter_;
    };

    template<typename T, typename... VARS>
    unique_ptr<T> make_unique(VARS... vars)
    {
        unique_ptr<T> p;
        p.data_ = new T(vars...);
        return p;
    }

    template<typename T, typename Deleter>
    unique_ptr<T, Deleter>::unique_ptr():data_(nullptr), deleter_(Deleter()){}

    template<typename T, typename Deleter>
    unique_ptr<T, Deleter>::unique_ptr(const typename unique_ptr<T, Deleter>::pointer p):
    data_(p), deleter_(Deleter()){}

    template<typename T, typename Deleter>
    unique_ptr<T, Deleter>::unique_ptr::unique_ptr(unique_ptr&& p )
    {
        this->data_ = p.data_;
        p.data_  = nullptr;
    }

    template<typename T, typename Deleter>
    unique_ptr<T, Deleter>& unique_ptr<T, Deleter>::operator=(unique_ptr&& p)
    {
        deleter_(data_);
        this->data_ = p.data_;
        p.data_ = nullptr;
        return *this;
    }

    template<typename T, typename Deleter>
    unique_ptr<T, Deleter>::unique_ptr::~unique_ptr()
    {
        deleter_(data_);
    }

    
}

#endif