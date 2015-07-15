#ifndef NETWORK_URI_VIEW_HPP
#define NETWORK_URI_VIEW_HPP

///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2015 Bjorn Reese <breese@users.sourceforge.net>
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#include <boost/utility/string_ref.hpp>

namespace network
{

class uri_view
{
public:
    typedef boost::string_ref string_view;
    typedef string_view::size_type size_type;
    typedef string_view::value_type value_type;

    uri_view(const string_view&);

    const string_view& scheme() const;
    const string_view& userinfo() const;
    const string_view& host() const;
    const string_view& port() const;
    const string_view& authority() const;
    const string_view& path() const;
    const string_view& query() const;
    const string_view& fragment() const;

private:
    void parse(string_view);
    size_type parse_scheme(const string_view&);
    size_type parse_hier_part(const string_view&);
    size_type parse_authority(string_view);
    size_type parse_userinfo(string_view);
    size_type parse_host(string_view);
    size_type parse_ipliteral(string_view);
    size_type parse_ipv4address(string_view);
    size_type parse_ipv6address(const string_view&);
    size_type parse_ipv6address_1(const string_view&);
    size_type parse_ipv6address_2(const string_view&);
    size_type parse_ipv6address_3(const string_view&, size_type before, size_type after);
    size_type parse_ipv6address_4(const string_view&);
    size_type parse_ipv6address_5(const string_view&);
    size_type parse_regname(string_view);
    size_type parse_dec_octet(string_view);
    size_type parse_ipv6address_h16(string_view);
    size_type parse_ipv6address_ls32(string_view);
    size_type parse_port(string_view);
    size_type parse_path_abempty(string_view);
    size_type parse_segment(string_view);
    size_type parse_query(const string_view&);
    size_type parse_fragment(const string_view&);
    size_type parse_pchar(string_view);
    size_type parse_pct_encoded(string_view);

    bool is_alpha_token(value_type) const;
    bool is_digit_token(value_type) const;
    bool is_hexdig_token(value_type) const;
    bool is_scheme_token(value_type) const;
    bool is_unreserved_token(value_type) const;
    bool is_subdelims_token(value_type) const;

private:
    static const value_type token_exclamation = '!';
    static const value_type token_number_sign = '#';
    static const value_type token_dollar = '$';
    static const value_type token_percent = '%';
    static const value_type token_ampersand = '&';
    static const value_type token_apostrophe = '\'';
    static const value_type token_parens_open = '(';
    static const value_type token_parens_close = ')';
    static const value_type token_asterisk = '*';
    static const value_type token_plus = '+';
    static const value_type token_comma = ',';
    static const value_type token_minus = '-';
    static const value_type token_dot = '.';
    static const value_type token_slash = '/';
    static const value_type token_colon = ':';
    static const value_type token_semicolon = ';';
    static const value_type token_equal = '=';
    static const value_type token_question_mark = '?';
    static const value_type token_at = '@';
    static const value_type token_bracket_open = '[';
    static const value_type token_bracket_close = ']';
    static const value_type token_underscore = '_';
    static const value_type token_tilde = '~';

    // Results
    string_view scheme_view;
    string_view userinfo_view;
    string_view host_view;
    string_view port_view;
    string_view authority_view;
    string_view path_view;
    string_view query_view;
    string_view fragment_view;
};

} // namespace network

#include <network/detail/uri_view.ipp>

#endif // NETWORK_URI_VIEW_HPP
