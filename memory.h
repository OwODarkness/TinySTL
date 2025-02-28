#ifndef TINYSTL_MEMORY_H
#define TINYSTL_MEMORY_H

#include <utility>

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


    //shared_ptr
    template<typename T,typename Deleter = default_delete<T>>
    class shared_ptr{
    public:
        using value_type = T;
        using pointer = T*;
        explicit shared_ptr();

        explicit shared_ptr(const pointer p);

        shared_ptr(const shared_ptr& p);

        shared_ptr(shared_ptr&& p);

        inline size_t use_count() const noexcept{return ref_count_ != nullptr ? *ref_count_ : 0;}

        inline pointer get() const noexcept{return data_;}

        inline Deleter& get_deleter() const noexcept {return deleter_;}

        inline bool unique() const noexcept{return use_count() == 1;}

        void reset(pointer p);

        shared_ptr& operator=(const shared_ptr& p);

        shared_ptr& operator=(shared_ptr&& p);

        value_type& operator*() const{return *data_;};
        
        pointer operator->() const{return data_;}

        operator bool() const{return (data_ != nullptr) ;}

        template<typename U>
        friend std::ostream& operator<<(std::ostream& out, const shared_ptr<U>& p);

        ~shared_ptr();
        
        template<typename U, typename...VARS> 
        friend shared_ptr<U> make_shared(VARS... );
    private:
        pointer data_;
        size_t* ref_count_;
        Deleter deleter_;
    };

    template<typename U, typename... VARS>
    shared_ptr<U> make_shared(VARS... vars)
    {
        shared_ptr<U> p;
        p.data_ = new U(vars...);
        p.ref_count_ = new size_t(1);
        return p;
    }

    //default constructor
    template<typename T, typename Deleter>
    shared_ptr<T, Deleter>::shared_ptr():
    data_(nullptr), ref_count_(nullptr), deleter_(Deleter()){}

    //constructor with parameter raw pointer
    template<typename T, typename Deleter>
    shared_ptr<T, Deleter>::shared_ptr(const typename shared_ptr<T, Deleter>::pointer p):
    data_(p), ref_count_(new size_t(1)), deleter_(Deleter()){}

    //copy constructor
    template<typename T, typename Deleter>
    shared_ptr<T, Deleter>::shared_ptr(const shared_ptr<T, Deleter>& p)
    {
        this->data_ = p.data_;
        this->ref_count_ = p.ref_count_;
        if(this->ref_count_ != nullptr)
            (*this->ref_count_)++;
    }

    //move constructor
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
            deleter_(data_);
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
            deleter_(data_);
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
            deleter_(data_);
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
            deleter_(data_);
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