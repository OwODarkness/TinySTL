#ifndef TINYSTL_MEMORY_H
#define TINYSTL_MEMORY_H

#include <utility>

namespace tinystl{

    //shared_ptr
    template<typename T>
    class shared_ptr{
    public:
        using value_type = T;
        using pointer = T*;
        explicit shared_ptr();

        inline size_t use_count() const noexcept{return ref_count_ != nullptr ? *ref_count_ : 0;}

        inline pointer get() noexcept{return data_;}

        template<typename U, typename...VARS> 
        friend shared_ptr<U> make_shared(VARS... vars);

        shared_ptr(const pointer p);

        shared_ptr(const shared_ptr<T>& p);

        shared_ptr(shared_ptr<T>&& p);

        shared_ptr& operator=(const shared_ptr& p);

        shared_ptr& operator=(shared_ptr&& p);

        value_type& operator*() const{return *data_;};
        
        pointer operator->() const{return data_;}

        template<typename U>
        friend std::ostream& operator<<(std::ostream& out, const shared_ptr<U>& p);

        ~shared_ptr();
    private:
        pointer data_;
        size_t* ref_count_;
    };

    template<typename U, typename... VARS>
    shared_ptr<U> make_shared(VARS... vars)
    {
        shared_ptr<U> p;
        p.data_ = new U(vars...);
        p.ref_count_ = new size_t(1);
        return p;
    }


    template<typename T>
    shared_ptr<T>::shared_ptr():data_(nullptr), ref_count_(nullptr){}

    template<typename T>
    shared_ptr<T>::shared_ptr(typename shared_ptr<T>::pointer p):
    data_(p), ref_count_(new size_t(1))
    {

    }

    template<typename T>
    shared_ptr<T>::shared_ptr(const shared_ptr<T>& p)
    {
        this->data_ = p.data_;
        this->ref_count_ = p.ref_count_;
        if(this->ref_count_ != nullptr)
            (*this->ref_count_)++;
    }

    template<typename T>
    shared_ptr<T>::shared_ptr(shared_ptr<T>&& p)
    {
        this->data_ = p.data_;
        this->ref_count_ = p.ref_count_;
        p.data_ = nullptr;
        p.ref_count_ = nullptr;
    }

    template<typename T>
    shared_ptr<T>& shared_ptr<T>::operator=(const shared_ptr<T>& p)
    {
        if(p.ref_count_ != nullptr)
        {
            ++(*p.ref_count_);

        }
        if(ref_count_ !=nullptr && --(*ref_count_) == 0)
        {
            delete data_;
            delete ref_count_;
        }
        this->data_ = p.data_;
        this->ref_count_ = p.ref_count_;
        return *this;
    }

    template<typename T>
    shared_ptr<T>& shared_ptr<T>::operator=(shared_ptr<T>&& p)
    {
        if(ref_count_ != nullptr && --(*ref_count_) == 0)
        {
            delete data_;
            delete ref_count_;
        }
        this->data_ = p.data_;
        this->ref_count_ = p.ref_count_;
        p.data_ = nullptr;
        p.ref_count_ = nullptr;
        return *this;
    }

    template<typename U>
    std::ostream& operator<<(std::ostream& out, const shared_ptr<U>& p)
    {
        out << p.data_;
        return out;
    }

    template<typename T>
    shared_ptr<T>::~shared_ptr()
    {
        if(ref_count_ != nullptr && --(*ref_count_) == 0)
        {
            delete data_;
            delete ref_count_;
        }
    }
}

#endif