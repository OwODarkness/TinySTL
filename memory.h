#ifndef TINYSTL_MEMORY_H
#define TINYSTL_MEMORY_H

#include <utility>

namespace tinystl{
    template<typename T>
    struct default_delete{
        default_delete() = default;
        void operator()(T* p)
        {
            std::cout << "default delete\n" ;
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


    //shared_ptr
    template<typename T,typename Deleter = default_delete<T>>
    class shared_ptr{
    public:
        using value_type = T;
        using pointer = T*;
        explicit shared_ptr();

        inline size_t use_count() const noexcept{return ref_count_ != nullptr ? *ref_count_ : 0;}

        inline pointer get() const noexcept{return data_;}

        inline bool unique() const noexcept{return use_count() == 1;}

        void reset(pointer p);

        template<typename U, typename...VARS> 
        friend shared_ptr<U> make_shared(VARS... vars);

        explicit shared_ptr(const pointer p);

        shared_ptr(const shared_ptr& p);

        shared_ptr(shared_ptr&& p);

        shared_ptr& operator=(const shared_ptr& p);

        shared_ptr& operator=(shared_ptr&& p);

        value_type& operator*() const{return *data_;};
        
        pointer operator->() const{return data_;}

        operator bool() const{return (data_ != nullptr) ;}

        template<typename U>
        friend std::ostream& operator<<(std::ostream& out, const shared_ptr<U>& p);

        ~shared_ptr();
    private:
        pointer data_;
        size_t* ref_count_;

        Deleter deleter;
    };

    template<typename U, typename... VARS>
    shared_ptr<U> make_shared(VARS... vars)
    {
        shared_ptr<U> p;
        p.data_ = new U(vars...);
        p.ref_count_ = new size_t(1);
        return p;
    }


    template<typename T, typename Deleter>
    shared_ptr<T, Deleter>::shared_ptr():
    data_(nullptr), ref_count_(nullptr), deleter(Deleter()){}

    template<typename T, typename Deleter>
    shared_ptr<T, Deleter>::shared_ptr(typename shared_ptr<T, Deleter>::pointer p):
    data_(p), ref_count_(new size_t(1)), deleter(Deleter()){}

    template<typename T, typename Deleter>
    shared_ptr<T, Deleter>::shared_ptr(const shared_ptr<T, Deleter>& p)
    {
        this->data_ = p.data_;
        this->ref_count_ = p.ref_count_;
        if(this->ref_count_ != nullptr)
            (*this->ref_count_)++;
    }

    template<typename T, typename Deleter>
    shared_ptr<T, Deleter>::shared_ptr(shared_ptr&& p)
    {
        this->data_ = p.data_;
        this->ref_count_ = p.ref_count_;
        p.data_ = nullptr;
        p.ref_count_ = nullptr;
    }

    template<typename T, typename Deleter>
    shared_ptr<T, Deleter>& shared_ptr<T, Deleter>::operator=(const shared_ptr& p)
    {
        if(p.ref_count_ != nullptr)
        {
            ++(*p.ref_count_);

        }
        if(ref_count_ !=nullptr && --(*ref_count_) == 0)
        {
            deleter(data_);
            delete ref_count_;
            data_  = nullptr;
            ref_count_ = nullptr;
        }
        this->data_ = p.data_;
        this->ref_count_ = p.ref_count_;
        return *this;
    }

    template<typename T, typename Deleter>
    shared_ptr<T, Deleter>& shared_ptr<T, Deleter>::operator=(shared_ptr&& p)
    {
        if(ref_count_ != nullptr && --(*ref_count_) == 0)
        {
            deleter(data_);
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

    template<typename T, typename Deleter>
    shared_ptr<T, Deleter>::~shared_ptr()
    {
        if(ref_count_ != nullptr && --(*ref_count_) == 0)
        {
            //delete data_;
            deleter(data_);
            delete ref_count_;
            data_ = nullptr;
            ref_count_ = nullptr;
        }
    }

    template<typename T, typename Deleter>
    void shared_ptr<T, Deleter>::reset(typename shared_ptr<T, Deleter>::pointer p)
    {
        if(ref_count_ != nullptr && --(*ref_count_)==0)
        {
            deleter(data_);
            delete ref_count_;
            data_ = nullptr;
            ref_count_ = nullptr;
        }
        if(p != nullptr)
        {
            data_ = p;
            ref_count_ = new size_t(1);
        }
    }
}

#endif