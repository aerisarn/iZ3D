
#ifndef BOOST_MPL_PRINT_HPP_INCLUDED
#define BOOST_MPL_PRINT_HPP_INCLUDED

// Copyright David Abrahams 2003
// Copyright Aleksey Gurtovoy 2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Id: print.hpp 49267 2008-10-11 06:19:02Z agurtovoy $
// $Date: 2008-10-11 10:19:02 +0400 (Ñá, 11 îêò 2008) $
// $Revision: 49267 $

#include <boost/mpl/aux_/config/msvc.hpp>
#include <boost/mpl/identity.hpp>

namespace boost { namespace mpl {
  
namespace aux {
#if defined(BOOST_MSVC)
# pragma warning(push, 3)
// we only want one warning from MSVC, so turn off the other one
# pragma warning(disable: 4307)
#elif defined(__MWERKS__)
# pragma warn_hidevirtual on
   struct print_base { virtual void f() {} };
#endif

#if defined(__EDG_VERSION__)
  template <class T>
  struct dependent_unsigned
  {
      static const unsigned value = 1;
  };
#endif
} // namespace aux 


template <class T>
struct print
    : mpl::identity<T>
#if defined(__MWERKS__)
    , aux::print_base
#endif 
{
#if defined(BOOST_MSVC)
    enum { n = sizeof(T) + -1 };
#elif defined(__MWERKS__)
    void f(int);
#else 
    enum {
        n =
# if defined(__EDG_VERSION__)
           aux::dependent_unsigned<T>::value > -1
# else 
           sizeof(T) > -1
# endif 
        };
#endif 
};

#if defined(BOOST_MSVC)
# pragma warning(pop)
#elif defined(__MWERKS__)
# pragma warn_hidevirtual reset
#endif

}}

#endif // BOOST_MPL_PRINT_HPP_INCLUDED
