
#ifndef BOOST_MPL_SET_SET0_C_HPP_INCLUDED
#define BOOST_MPL_SET_SET0_C_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2003-2004
// Copyright David Abrahams 2003-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Id: set0_c.hpp 49267 2008-10-11 06:19:02Z agurtovoy $
// $Date: 2008-10-11 10:19:02 +0400 (Ñá, 11 îêò 2008) $
// $Revision: 49267 $

#include <boost/mpl/set/set0.hpp>
#include <boost/mpl/integral_c.hpp>

namespace boost { namespace mpl {

template< typename T > struct set0_c
    : set0<>
{
    typedef set0_c type;
    typedef T value_type;
};

}}

#endif // BOOST_MPL_SET_SET0_C_HPP_INCLUDED
