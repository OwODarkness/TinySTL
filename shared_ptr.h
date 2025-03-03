#ifndef TINYSTL_SHARED_PTR_H
#define TINYSTL_SHARED_PTR_H

#include "deleter.h"

namespace tinystl{
    struct smart_ptr_control_block{
        virtual void Delete()= 0;
        size_t ref_count = 0;
        size_t weak_count = 0;
    };

    template<typename T, typename DeleterType = default_delete<T>>
    struct shared_ptr_control_block: public smart_ptr_control_block{
        shared_ptr_control_block():ptr(nullptr){}
        shared_ptr_control_block(T* p): ptr(p){ref_count = 1;}
        void Delete() override
        {
            deleter(ptr);
            ptr = nullptr;
        }
        DeleterType deleter;
        T* ptr;
    };
    template<typename T, typename U>
    class weak_ptr;

    template<typename T,typename Deleter = default_delete<T>>
    class shared_ptr{
    public:
        using value_type = T;
        using pointer = T*;

        explicit shared_ptr();

        explicit shared_ptr(const pointer p);

        shared_ptr(const shared_ptr& p);

        shared_ptr(shared_ptr&& p);

        shared_ptr(shared_ptr_control_block<T, Deleter>* cbk);

        inline size_t use_count() const noexcept{return cbk_ ? cbk_->ref_count : 0;}

        inline pointer get() const noexcept{return data_;}

        inline Deleter& get_deleter() const noexcept {return cbk_->deleter;}

        inline bool unique() const noexcept{return use_count() == 1;}

        void reset(pointer p);

        shared_ptr& operator=(const shared_ptr& p);

        shared_ptr& operator=(shared_ptr&& p);

        value_type& operator*() const{return *data_;};
        
        pointer operator->() const{return data_;}

        operator bool() const{return (data_ != nullptr) ;}

        ~shared_ptr();
        
        template<typename U, typename...VARS> 
        friend shared_ptr<U> make_shared(VARS... );
    private:
        template<typename U, typename T>
        friend class weak_ptr;

        pointer data_;
        shared_ptr_control_block<T, Deleter>* cbk_;
    };

    template<typename U, typename... VARS>
    shared_ptr<U> make_shared(VARS... vars)
    {
        shared_ptr<U> p;
        p.data_ = new U(vars...);
        p.cbk_ = new shared_ptr_control_block<U>(p.data_);
        return p;
    }

    //default constructor
    template<typename T, typename Deleter>
    shared_ptr<T, Deleter>::shared_ptr():
    data_(nullptr), cbk_(nullptr){}

    //constructor with parameter raw pointer
    template<typename T, typename Deleter>
    shared_ptr<T, Deleter>::shared_ptr(const typename shared_ptr<T, Deleter>::pointer p):
    data_(p), cbk_(new shared_ptr_control_block<T, Deleter>(p)){}

    //copy constructor
    template<typename T, typename Deleter>
    shared_ptr<T, Deleter>::shared_ptr(const shared_ptr<T, Deleter>& p)
    {
        this->data_ = p.data_;
        this->cbk_ = p.cbk_;
        if(p.cbk_ != nullptr && p.cbk_->ref_count != 0)
            p.cbk_->ref_count++;
    }

    //move constructor
    template<typename T, typename Deleter>
    shared_ptr<T, Deleter>::shared_ptr(shared_ptr&& p)
    {
        this->data_ = p.data_;
        this->cbk_ = p.cbk_;
        p.data_ = nullptr;
        p.cbk_ = nullptr;
    }

    template<typename T, typename Deleter>
    shared_ptr<T, Deleter>::shared_ptr(shared_ptr_control_block<T, Deleter>* cbk)
    {
        if(cbk)
        {
            this->cbk_ = cbk;
            if(cbk->ref_count == 0)
            {
                this->data_ = nullptr;
            }
            else
            {
                this->data_ = cbk->ptr;
                ++(this->cbk_->ref_count);
            }
        }
        else
        {
            this->data_  = nullptr;
            this->cbk_ = nullptr;
        }
    }

    template<typename T, typename Deleter>
    shared_ptr<T, Deleter>& shared_ptr<T, Deleter>::operator=(const shared_ptr& p)
    {
        if( this->cbk_ != nullptr && --this->cbk_->ref_count == 0)
        {
            this->cbk_->Delete();
            if(this->cbk_->weak_count == 0)
            {
                delete this->cbk_;
            }
        }
        this->data_ = p.data_;
        this->cbk_ = p.cbk_;
        if(p.cbk_ != nullptr && p.cbk_->ref_count != 0)
        {
            ++this->cbk_ ->ref_count;
        }
        return *this;
    }

    template<typename T, typename Deleter>
    shared_ptr<T, Deleter>& shared_ptr<T, Deleter>::operator=(shared_ptr&& p)
    {
        if( this->cbk_ != nullptr && --this->cbk_->ref_count == 0)
        {
            this->cbk_->Delete();
            if(this->cbk_->weak_count == 0)
            {
                delete this->cbk_;
            }
        }
        this->data_ = p.data_;
        this->cbk_ = p.cbk_;
        p.data_ = nullptr;
        p.cbk_ = nullptr;
        return *this;
    }


    template<typename T, typename Deleter>
    shared_ptr<T, Deleter>::~shared_ptr()
    {
        if( this->cbk_ != nullptr && --this->cbk_->ref_count == 0)
        {
            std::cout << "shared ptr des\n"; 
            this->cbk_->Delete();
            if(this->cbk_->weak_count == 0)
            {
                delete this->cbk_;
            }
        }
    }

    template<typename T, typename Deleter>
    void shared_ptr<T, Deleter>::reset(typename shared_ptr<T, Deleter>::pointer p)
    {
        if(this->cbk_->ref_count != 0 && --this->cbk_->ref_count == 0)
        {
            this->cbk_->Delete();
            if(this->cbk_->weak_count == 0)
            {
                delete this->cbk_;
            }
        }
        if(p != nullptr)
        {
            this->data_ = p;
            this->cbk_ = new shared_ptr_control_block(p);
        }
    }

    template<typename T, typename Deleter = default_delete<T>>
    class weak_ptr{
    public:
        weak_ptr();
        weak_ptr(const shared_ptr<T, Deleter>& p);
        inline size_t use_count() const{return cbk_ ? cbk_->ref_count : 0;}
        shared_ptr<T, Deleter> lock();
        weak_ptr& operator=(const shared_ptr<T, Deleter>& p);
        //weak_ptr& operator=(shared_ptr<T, Deleter>&& p) = delete;

    private:
        shared_ptr_control_block<T>* cbk_;
    };

    template<typename T,typename Deleter>
    weak_ptr<T, Deleter>::weak_ptr():cbk_(nullptr){}

    template<typename T, typename Deleter>
    weak_ptr<T, Deleter>::weak_ptr(const shared_ptr<T, Deleter>& p)
    {
        if(p)
        {
            this->cbk_ = p.cbk_;
            if(this->cbk_)
            {
                ++this->cbk_->weak_count;
            }
        }
        else
        {
            cbk_ = nullptr;
        }
    }

    template<typename T, typename Deleter>
    shared_ptr<T, Deleter>  weak_ptr<T, Deleter>::lock()
    {
        //TODO: return a shared_ptr
        shared_ptr<T, Deleter> p(cbk_);
        return p;
    }

    template<typename T, typename Deleter>
    weak_ptr<T, Deleter>& weak_ptr<T, Deleter>::operator=(const shared_ptr<T, Deleter>& p)
    {
        if(cbk_)
        {
            --cbk_->weak_count;
        }
        this->cbk_ = p.cbk_;
        if(this->cbk_)
        {
            ++this->cbk_->weak_count;
        }
        return *this;
    }
}

#endif //TINYSLT_SHARED_PTR_H