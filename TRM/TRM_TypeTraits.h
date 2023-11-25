/******************************************************************************
/ TRM_TypeTraits.h
/
/ Copyright (c) 2023 Ték Róbert Máté
/
/ Permission is hereby granted, free of charge, to any person obtaining a copy
/ of this software and associated documentation files (the "Software"), to deal
/ in the Software without restriction, including without limitation the rights to
/ use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
/ of the Software, and to permit persons to whom the Software is furnished to
/ do so, subject to the following conditions:
/
/ The above copyright notice and this permission notice shall be included in all
/ copies or substantial portions of the Software.
/
/ THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
/ EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
/ OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
/ NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
/ HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
/ WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
/ FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
/ OTHER DEALINGS IN THE SOFTWARE.
/
******************************************************************************/
#pragma once


namespace TRM {

namespace detail
{
    template<class T>
    struct type_identity { using type = T; };
 
    template<class T> // Note that `cv void&` is a substitution failure
    auto try_add_lvalue_reference(int) -> type_identity<T&>;
    template<class T> // Handle T = cv void case
    auto try_add_lvalue_reference(...) -> type_identity<T>;
 
    template<class T>
    auto try_add_rvalue_reference(int) -> type_identity<T&&>;
    template<class T>
    auto try_add_rvalue_reference(...) -> type_identity<T>;
} // namespace detail
 
template<class T>
struct add_lvalue_reference : decltype(detail::try_add_lvalue_reference<T>(0)) {};
 
template<class T>
struct add_rvalue_reference : decltype(detail::try_add_rvalue_reference<T>(0)) {};


template<typename T>
typename add_rvalue_reference<T>::type declval() noexcept;


struct false_type { static constexpr bool value = false; };
struct true_type  { static constexpr bool value = true;  };


template<class T>   struct remove_reference      { using type = T; };
template<class T>   struct remove_reference<T&>  { using type = T; };
template<class T>   struct remove_reference<T&&> { using type = T; };

template<class T>   struct is_lvalue_reference     : false_type {};
template<class T>   struct is_lvalue_reference<T&> : true_type  {};

template<class T>   struct is_rvalue_reference      : false_type {};
template<class T>   struct is_rvalue_reference<T&&> : true_type  {};

template<class T>   struct is_reference      : false_type {};
template<class T>   struct is_reference<T&>  : true_type  {};
template<class T>   struct is_reference<T&&> : true_type  {};


template<class T>   struct is_const          : false_type {};
template<class T>   struct is_const<const T> : true_type  {};

namespace detail {
    template<class T, class> 
    struct is_destructible_impl : false_type {};

    template<class T>
    struct is_destructible_impl<T, decltype(void(declval<T>().~T()))> : true_type {};
}

template<class T> struct is_destructible : detail::is_destructible_impl<T, void> {};


template <typename T>
typename remove_reference<T>::type&& move(T&& arg) noexcept
{
  return static_cast<typename remove_reference<T>::type&&>(arg);
}


template <class T>
inline T&& forward(typename remove_reference<T>::type& t) noexcept
{
    return static_cast<T&&>(t);
}

template <class T>
inline T&& forward(typename remove_reference<T>::type&& t) noexcept
{
    static_assert(!is_lvalue_reference<T>::value,
                  "Can not forward an rvalue as an lvalue.");
    return static_cast<T&&>(t);
}


using nullptr_t = decltype(nullptr);

}   // namespace TRM