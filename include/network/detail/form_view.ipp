#ifndef NETWORK_DETAIL_FORM_VIEW_IPP
#define NETWORK_DETAIL_FORM_VIEW_IPP

///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2015 Bjorn Reese <breese@users.sourceforge.net>
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#include <network/detail/traits.hpp>

namespace network
{

//-----------------------------------------------------------------------------
// detail::form_converter
//-----------------------------------------------------------------------------

namespace detail
{

template <typename ReturnType>
struct form_converter
{
    typedef form_view::view_type view_type;
    static ReturnType convert(const view_type& input)
    {
        ReturnType result;

        typename view_type::const_iterator end = input.end();
        for (typename view_type::const_iterator it = input.begin();
             it != end;
             ++it)
        {
            switch (*it)
            {
            case traits<char>::alpha_plus:
                result += traits<char>::alpha_space;
                break;

            case traits<char>::alpha_percent:
                if (std::distance(it, end) >= 3)
                {
                    ++it;
                    if (!traits<char>::is_hexdigit(*it))
                        throw std::runtime_error("Incorrect percent encoding");
                    typename traits<char>::int_type number = traits<char>::hex_to_int(*it) << 4;
                    ++it;
                    if (!traits<char>::is_hexdigit(*it))
                        throw std::runtime_error("Incorrect percent encoding");
                    number += traits<char>::hex_to_int(*it);
                    result += std::char_traits<char>::to_char_type(number);
                }
                else
                {
                    throw std::runtime_error("Incorrect percent encoding");
                }
                break;

            default:
                result += *it;
                break;
            }
        }
        return result;
    }
};

} // namespace detail

//-----------------------------------------------------------------------------
// form_view
//-----------------------------------------------------------------------------

inline form_view::form_view(const view_type& input)
    : input_view(input),
      count()
{
    next();
}

inline bool form_view::next()
{
    // Parse next key-value pair
    //
    // http://www.w3.org/TR/html401/interact/forms.html#h-17.13.4.1
    //
    // form ::= ( form-list )?
    // form-list ::= form-key-value ( '&' form-key-value )*
    // form-key-value ::= text '=' text

    if (input_view.empty())
        return false;

    if (count > 0)
    {
        // Skip list separator
        if (input_view.front() != detail::traits<char>::alpha_ampersand)
            return false;
        input_view.remove_prefix(1);
    }

    size_type processed = parse_key(input_view);
    if (processed == 0)
        return false;
    key_view = input_view.substr(0, processed);
    input_view.remove_prefix(processed);

    if (input_view.front() != detail::traits<char>::alpha_equal)
        return false;
    input_view.remove_prefix(1);

    processed = parse_value(input_view);
    if (processed == 0)
        return false;
    value_view = input_view.substr(0, processed);
    input_view.remove_prefix(processed);

    ++count;

    return true;
}

inline const form_view::view_type& form_view::literal_key() const
{
    return key_view;
}

inline const form_view::view_type& form_view::literal_value() const
{
    return value_view;
}

inline form_view::string_type form_view::key() const
{
    return detail::form_converter<string_type>::convert(key_view);
}

template <typename ReturnType>
ReturnType form_view::value() const
{
    return detail::form_converter<ReturnType>::convert(value_view);
}

form_view::size_type form_view::parse_key(const view_type& view)
{
    // All characters until =, &, or EOF

    typename view_type::const_iterator begin = view.begin();
    typename view_type::const_iterator end = view.end();
    typename view_type::const_iterator it = begin;
    for (;
         it != end;
         ++it)
    {
        if (*it == detail::traits<char>::alpha_equal)
            break;
        if (*it == detail::traits<char>::alpha_ampersand)
            break;
    }
    return std::distance(begin, it);
}

form_view::size_type form_view::parse_value(const view_type& view)
{
    return parse_key(view);
}

} // namespace network

#endif // NETWORK_DETAIL_FORM_VIEW_IPP
