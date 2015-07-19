#ifndef NETWORK_FORM_VIEW_HPP
#define NETWORK_FORM_VIEW_HPP

///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2015 Bjorn Reese <breese@users.sourceforge.net>
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#include <string>
#include <boost/config.hpp>
#include <boost/utility/string_ref.hpp>

namespace network
{

// http://www.w3.org/TR/html401/interact/forms.html

class form_view
{
public:
    typedef std::string string_type;
    typedef boost::string_ref view_type;
    typedef view_type::size_type size_type;
    typedef view_type::value_type value_type;

    form_view(const view_type&);

    bool next() BOOST_NOEXCEPT;

    const view_type& literal_key() const BOOST_NOEXCEPT;
    const view_type& literal_value() const BOOST_NOEXCEPT;

    string_type key() const;
    template <typename ReturnType> ReturnType value() const;

private:
    size_type parse_key(const view_type&);
    size_type parse_value(const view_type&);

private:
    view_type input_view;
    view_type key_view;
    view_type value_view;
    size_type count;
};

} // namespace network

#include <network/detail/form_view.ipp>

#endif // NETWORK_FORM_VIEW_HPP
