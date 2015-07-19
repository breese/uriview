#ifndef NETWORK_DETAIL_TRAITS_HPP
#define NETWORK_DETAIL_TRAITS_HPP

///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2015 Bjorn Reese <breese@users.sourceforge.net>
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#include <boost/config.hpp>

namespace network
{
namespace detail
{

template <typename CharT>
class traits
{
};

//-----------------------------------------------------------------------------
// char specialization
//-----------------------------------------------------------------------------

template <>
class traits<char>
{
public:
    typedef char value_type;
    typedef typename std::char_traits<value_type>::int_type int_type;

    static bool is_digit(value_type value) BOOST_NOEXCEPT;
    static bool is_alpha(value_type value) BOOST_NOEXCEPT;
    static bool is_hexdigit(value_type value) BOOST_NOEXCEPT;
    static int_type hex_to_int(value_type value) BOOST_NOEXCEPT;

    BOOST_STATIC_CONSTANT(value_type, alpha_space = ' ');
    BOOST_STATIC_CONSTANT(value_type, alpha_exclamation = '!');
    BOOST_STATIC_CONSTANT(value_type, alpha_number_sign = '#');
    BOOST_STATIC_CONSTANT(value_type, alpha_dollar = '$');
    BOOST_STATIC_CONSTANT(value_type, alpha_percent = '%');
    BOOST_STATIC_CONSTANT(value_type, alpha_ampersand = '&');
    BOOST_STATIC_CONSTANT(value_type, alpha_apostrophe = '\'');
    BOOST_STATIC_CONSTANT(value_type, alpha_parens_open = '(');
    BOOST_STATIC_CONSTANT(value_type, alpha_parens_close = ')');
    BOOST_STATIC_CONSTANT(value_type, alpha_asterisk = '*');
    BOOST_STATIC_CONSTANT(value_type, alpha_plus = '+');
    BOOST_STATIC_CONSTANT(value_type, alpha_comma = ',');
    BOOST_STATIC_CONSTANT(value_type, alpha_minus = '-');
    BOOST_STATIC_CONSTANT(value_type, alpha_dot = '.');
    BOOST_STATIC_CONSTANT(value_type, alpha_slash = '/');
    BOOST_STATIC_CONSTANT(value_type, alpha_0 = '0');
    BOOST_STATIC_CONSTANT(value_type, alpha_colon = ':');
    BOOST_STATIC_CONSTANT(value_type, alpha_semicolon = ';');
    BOOST_STATIC_CONSTANT(value_type, alpha_equal = '=');
    BOOST_STATIC_CONSTANT(value_type, alpha_question_mark = '?');
    BOOST_STATIC_CONSTANT(value_type, alpha_at = '@');
    BOOST_STATIC_CONSTANT(value_type, alpha_A = 'A');
    BOOST_STATIC_CONSTANT(value_type, alpha_a = 'a');
    BOOST_STATIC_CONSTANT(value_type, alpha_bracket_open = '[');
    BOOST_STATIC_CONSTANT(value_type, alpha_bracket_close = ']');
    BOOST_STATIC_CONSTANT(value_type, alpha_underscore = '_');
    BOOST_STATIC_CONSTANT(value_type, alpha_tilde = '~');

private:
    static bool is_hex_upper(value_type value) BOOST_NOEXCEPT;
    static bool is_hex_lower(value_type value) BOOST_NOEXCEPT;
};

} // namespace detail
} // namespace network

#include <network/detail/traits.ipp>

#endif // NETWORK_DETAIL_TRAITS_HPP
