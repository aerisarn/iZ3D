
#ifndef BOOST_MPL_KEY_TYPE_HPP_INCLUDED
#define BOOST_MPL_KEY_TYPE_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2003-2004
// Copyright David Abrahams 2003-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Id: key_type.hpp 49267 2008-10-11 06:19:02Z agurtovoy $
// $Date: 2008-10-11 10:19:02 +0400 (Ñá, 11 îêò 2008) $
// $Revision: 49267 $

#include <boost/mpl/key_type_fwd.hpp>
#include <boost/mpl/sequence_tag.hpp>
#include <boost/mpl/apply_wrap.hpp>
#include <boost/mpl/aux_/na_spec.hpp>
#include <boost/mpl/aux_/lambda_support.hpp>

namespace boost { namespace mpl {

template<
      typename BOOST_MPL_AUX_NA_PARAM(AssociativeSequence)
    , typename BOOST_MPL_AUX_NA_PARAM(T)
    >
struct key_type
    : apply_wrap2< 
          key_type_impl< typename sequence_tag<AssociativeSequence>::type >
        , AssociativeSequence, T>
{
    BOOST_MPL_AUX_LAMBDA_SUPPORT(2,key_type,(AssociativeSequence,T))
};

BOOST_MPL_AUX_NA_SPEC(2, key_type)

}}

#endif // BOOST_MPL_KEY_TYPE_HPP_INCLUDED
