#ifndef TINYSTL_ITERATOR_H
#define TINYSTL_ITERATOR_H

#include <cstddef>

#include "type_trait.h"

namespace tinystl
{
    //iterator_category
    struct input_iterator_tag{};
    struct output_iterator_tag{};
    struct forward_iterator_tag: public input_iterator_tag, output_iterator_tag{};
    struct bidirectional_iterator_tag: public forward_iterator_tag{};
    struct random_access_iterator_tag: public bidirectional_iterator_tag{};
    
    template <typename Category, typename T, class Distance = ptrdiff_t, class Pointer = T*, class Reference = T&>
    struct iterator
    {
        using iterator_category = Category;
        using value_type = T;
        using difference_type = Distance;//address distance
        using pointer = Pointer;//T*
        using reference = Reference;//T&
    };

    //check if T has iterator_category nested type
    template<typename T>
    struct has_iterator_category{
        struct two{char a, b;};
        template<typename U> static two test(...);
        template<typename U> static char test(typename U::iterator_category* = 0);
        static const bool value = (sizeof(test<T>(0)) == sizeof(char));
    };

    template<typename Iterator, bool value>
    struct iterator_trait_impl{};

    template<typename Iterator>
    struct iterator_trait_impl<Iterator, true>
    {
        using iterator_category = typename Iterator::iterator_category;
        using value_type = typename Iterator::value_type;
        using difference_type = typename Iterator::difference_type;
        using pointer = typename Iterator::pointer;
        using reference = typename Iterator::reference;
    };

    template<typename Iterator, bool value>
    struct iterator_trait_helper{};

    template<typename Iterator>
    constexpr bool k_is_iterator =         
    std::is_convertible<typename Iterator::iterator_category, input_iterator_tag>::value ||
    std::is_convertible<typename Iterator::iterator_category, output_iterator_tag>::value;

    template<typename Iterator>
    struct iterator_trait_helper<Iterator, true>
    : public iterator_trait_impl<Iterator, k_is_iterator >{};

    template<typename Iterator>
    struct iterator_trait: public iterator_trait_helper<Iterator, has_iterator_category<Iterator>::value>{};

    template<typename T>
    struct iterator_trait<T*>{
        using iterator_category = random_access_iterator_tag;
        using value_type = T;
        using difference_type = ptrdiff_t;
        using pointer = T*;
        using reference = T&;
    };

    template<typename T>
    struct iterator_trait<const T*>{
        using iterator_category = random_access_iterator_tag;
        using value_type = T;
        using difference_type = ptrdiff_t;
        using pointer = const T*;
        using reference = const T&;
    };

    template<typename Iterator>
    typename iterator_trait<Iterator>::value_type* value_type(const Iterator&)
    {
        return static_cast<typename iterator_trait<Iterator>::value_type*>(0);
    }

    template<typename Iterator>
    typename iterator_trait<Iterator>::difference_type* difference_type(const Iterator&)
    {
        return static_cast<typename iterator_trait<Iterator>::difference_type*>(0);
    }

    template<typename Iterator>
    typename iterator_trait<Iterator>::iterator_category iterator_category(const Iterator&)
    {
        using Category = typename iterator_trait<Iterator>::iterator_category;
        return Category();
    }

    //check whether T is a iterator, what the iterator category(U) is
    template<typename T, typename U, bool value = has_iterator_category<T>::value>
    struct has_iterator_category_of
    : public m_bool_constant<std::is_convertible<typename T::iterator_category,U>>{};

    template<typename T, typename U>
    struct has_iterator_category_of<T, U, false>: public m_false_type{};

    //check specify iterator
    //inpuut
    template<typename Iterator> struct is_input_iterator: public has_iterator_category_of<Iterator, input_iterator_tag>{};
    //output
    template<typename Iterator> struct is_output_iterator: public has_iterator_category_of<Iterator, output_iterator_tag>{};
    //forward
    template<typename Iterator> struct is_forward_iterator: public has_iterator_category_of<Iterator, forward_iterator_tag>{};
    //bidirectional
    template<typename Iterator> struct is_bidirectional_iterator: public has_iterator_category_of<Iterator, bidirectional_iterator_tag>{};
    //random_access
    template<typename Iterator> struct is_random_access_iterator: public has_iterator_category_of<Iterator, random_access_iterator_tag>{};

    template<typename Iterator> struct is_iterator: public m_bool_constant<b_is_iterator>{};

    //input advance
    template<typename Iterator, typename Distance>
    void advance_dispatch(Iterator& it, Distance n, input_iterator_tag)
    {
        while(n--) ++it;
    }

    //forward advance
    template<typename Iterator, typename Distance>
    void advance_dispatch(Iterator& it, Distance n, forward_iterator_tag)
    {
        advance_dispatch(it, n, input_iterator_tag())
    }

    //bidirectional advance
    template<typename Iterator, typename Distance>
    void advance_dispatch(Iterator& it, Distance n, bidirectional_iterator_tag)
    {
        if(n>0)
        {
            while(n--) ++it;
        }
        else
        {
            while(n++) --t;
        }
    }

    //random_access advance
    template<typename Iterator, typename Distance>
    void advance_dispatch(Iterator& it, Distance n, random_access_iterator_tag)
    {
        it += n;
    }

    template<typename Iterator, typename Distance>
    void advance(Iterator& it, Distance n)
    {
        advance_dispatch(it, n, iterator_category(it));
    }

    template<typename Iterator>
    typename iterator_trait<Iterator>::difference_type distance_dispatch(Iterator begin, Iterator end, input_iterator_tag)
    {
        typename iterator_trait<Iterator>::difference_type dist{};
        while(begin != end)
        {
            end--;
            dist++;
        }
        return dist;    
    }

    template<typename Iterator>
    typename iterator_trait<Iterator>::difference_type distance_dispatch(Iterator begin, Iterator end, random_access_iterator_tag)
    {
        return end - begin;
    }

    template<typename Iterator>
    typename iterator_trait<Iterator>::difference_type distance(Iterator begin, Iterator end)
    {
        return distance_dispatch(begin, end, iterator_category(begin));
    }
};

#endif // TINYSTL_ITERATOR_H