// This file was autogenerated using foonathan/compatibility.
// See https://github.com/foonathan/compatibility for further information.
// Do not edit manually!

#ifndef COMP_FOONATHAN_HOSTED_IMPLEMENTATION_HPP_INCLUDED
#define COMP_FOONATHAN_HOSTED_IMPLEMENTATION_HPP_INCLUDED

#include <cstddef>

#define FOONATHAN_HAS_HOSTED_IMPLEMENTATION 1


#include "noexcept.hpp"
#include "rvalue_ref.hpp"

// HAS_HOSTED_IMPLEMENTATION doesn't sond that nice... :D
#define FOONATHAN_HOSTED_IMPLEMENTATION FOONATHAN_HAS_HOSTED_IMPLEMENTATION

#include <type_traits>

#if FOONATHAN_HOSTED_IMPLEMENTATION
    #include <utility>
#endif

namespace foonathan_comp
{
#if FOONATHAN_HAS_RVALUE_REF
    // move - taken from http://stackoverflow.com/a/7518365
    template <typename T>
    typename std::remove_reference<T>::type&& move(T&& arg) FOONATHAN_NOEXCEPT
    {
        return static_cast<typename std::remove_reference<T>::type&&>(arg);
    }

    // forward - taken from http://stackoverflow.com/a/27501759
    template <class T>
    T&& forward(typename std::remove_reference<T>::type& t) FOONATHAN_NOEXCEPT
    {
        return static_cast<T&&>(t);
    }

    template <class T>
    T&& forward(typename std::remove_reference<T>::type&& t) FOONATHAN_NOEXCEPT
    {
        static_assert(!std::is_lvalue_reference<T>::value,
                      "Can not forward an rvalue as an lvalue.");
        return static_cast<T&&>(t);
    }
#endif

#if FOONATHAN_HOSTED_IMPLEMENTATION
    using std::swap;
#else
    #if FOONATHAN_HAS_RVALUE_REF
        template <typename T>
        void swap(T &a, T &b) FOONATHAN_NOEXCEPT_IF(std::is_nothrow_move_assignable<T>::value
                                                   && std::is_nothrow_move_constructible<T>::value)
       {
           T tmp = move(a);
           a = move(b);
           b = move(tmp);
        }
    #else
        template <typename T>
        void swap(T &a, T &b)
        {
            T tmp(a);
            a = b;
            b = tmp;
        }
    #endif
#endif
}

#endif
