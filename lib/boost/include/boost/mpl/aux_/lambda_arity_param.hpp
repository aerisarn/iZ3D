
#ifndef BOOST_MPL_AUX_LAMBDA_ARITY_PARAM_HPP_INCLUDED
#define BOOST_MPL_AUX_LAMBDA_ARITY_PARAM_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2001-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Id: lambda_arity_param.hpp 49267 2008-10-11 06:19:02Z agurtovoy $
// $Date: 2008-10-11 10:19:02 +0400 (Ñá, 11 îêò 2008) $
// $Revision: 49267 $

#include <boost/mpl/aux_/config/ttp.hpp>

#if !defined(BOOST_MPL_CFG_EXTENDED_TEMPLATE_PARAMETERS_MATCHING)
#   define BOOST_MPL_AUX_LAMBDA_ARITY_PARAM(param)    
#else
#   define BOOST_MPL_AUX_LAMBDA_ARITY_PARAM(param) , param
#endif

#endif // BOOST_MPL_AUX_LAMBDA_ARITY_PARAM_HPP_INCLUDED
