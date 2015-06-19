#ifndef NETWORK_DETAIL_URI_VIEW_IPP
#define NETWORK_DETAIL_URI_VIEW_IPP

///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2015 Bjorn Reese <breese@users.sourceforge.net>
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#include <cassert>

namespace network
{

inline uri_view::uri_view(const string_view& input)
{
    parse(input);
}

//-----------------------------------------------------------------------------
// Accessors
//-----------------------------------------------------------------------------

inline const uri_view::string_view& uri_view::scheme() const
{
    return scheme_view;
}

inline const uri_view::string_view& uri_view::userinfo() const
{
    return userinfo_view;
}

inline const uri_view::string_view& uri_view::host() const
{
    return host_view;
}

inline const uri_view::string_view& uri_view::port() const
{
    return port_view;
}

inline const uri_view::string_view& uri_view::authority() const
{
    return authority_view;
}

inline const uri_view::string_view& uri_view::path() const
{
    return path_view;
}

//-----------------------------------------------------------------------------
// Parser
//-----------------------------------------------------------------------------

inline void uri_view::parse(string_view input)
{
    // RFC 3986 Section 3
    //
    // URI = scheme ":" hier-part [ "?" query ] [ "#" fragment ]

    if (input.empty())
        return;

    size_type processed = parse_scheme(input);
    input.remove_prefix(processed);

    if (input.empty() || (input.front() != token_colon))
        return; // FIXME: Report error
    input.remove_prefix(sizeof(token_colon));

    processed = parse_hier_part(input);

    // FIXME: query and fragment
}

inline uri_view::size_type uri_view::parse_scheme(string_view input)
{
    // RFC 3986 Section 3.1
    //
    // scheme = ALPHA *( ALPHA / DIGIT / "+" / "-" / "." )

    assert(!input.empty());

    string_view::const_iterator current = input.begin();

    if (!is_alpha_token(*current))
        return 0;
    ++current;

    for (;
         current != input.end();
         ++current)
    {
        if (!is_scheme_token(*current))
            break;
    }
    const size_type result = std::distance(input.begin(), current);
    scheme_view = input.substr(0, result);
    return result;
}

inline uri_view::size_type uri_view::parse_hier_part(string_view input)
{
    // RFC 3986 Section 3
    //
    // hier-part = "//" authority path-abempty
    //           / path-absolute
    //           / path-rootless
    //           / path-empty

    if (input[0] == token_slash)
    {
        if (input[1] == token_slash)
        {
            input.remove_prefix(2);
            size_type total = 0;
            size_type processed = parse_authority(input);
            if (processed == 0)
                return total;
            authority_view = input.substr(total, processed);
            total += processed;

            processed = parse_path_abempty(input.substr(processed));
            if (processed == 0)
                return total;
            path_view = input.substr(total, processed);
            total += processed;
            return total;
        }
        else
        {
            assert(false); // Not implemented yet
        }
    }
    return 0;
}

inline uri_view::size_type uri_view::parse_authority(string_view input)
{
    // RFC 3986 Section 3.2
    //
    // authority = [ userinfo "@" ] host [ ":" port ]

    size_type processed = parse_userinfo(input);
    size_type total = processed;
    if (input[processed] == token_at)
    {
        userinfo_view = input.substr(processed);
        input.remove_prefix(processed + sizeof(token_at));
    }

    processed = parse_host(input);
    if (processed == 0)
        return 0;
    total += processed;

    if (input[processed] == token_colon)
    {
        processed = parse_port(input.substr(processed + sizeof(token_colon)));
        if (processed > 0)
            total += processed + sizeof(token_colon);
    }
    return total;
}

inline uri_view::size_type uri_view::parse_userinfo(string_view input)
{
    // RFC 3986 Section 3.2.1
    //
    // userinfo = *( unreserved / pct-encoded / sub-delims / ":" )

    return 0; // FIXME
}

inline uri_view::size_type uri_view::parse_host(string_view input)
{
    // RFC 3986 Section 3.2.2
    //
    // host = IP-literal / IPv4address / reg-name

    size_type processed = 0;

    if (input[0] == token_bracket_open)
    {
        processed = parse_ipliteral(input);
        if (processed == 0)
            return 0;
        // Skip brackets
        host_view = input.substr(1, processed - 2);
    }
    else
    {
        processed = parse_ipv4address(input);
        if (processed == 0)
        {
            processed = parse_regname(input);
            if (processed == 0)
                return 0;
        }
        host_view = input.substr(0, processed);
    }

    return processed;
}

inline uri_view::size_type uri_view::parse_ipliteral(string_view input)
{
    // RFC 3986 Section 3.2.2
    //
    // IP-literal = "[" ( IPv6address / IPvFuture  ) "]"

    string_view::const_iterator current = input.begin();
    if (*current != token_bracket_open)
        return 0;
    ++current;

    size_type processed = parse_ipv6address(&*current);
    if (processed == 0)
    {
        // FIXME: IPvFuture
        return 0;
    }
    current += processed;
    if (*current != token_bracket_close)
        return 0;
    ++current;

    return std::distance(input.begin(), current);
}

inline uri_view::size_type uri_view::parse_ipv4address(string_view input)
{
    // RFC 3986 Section 3.2.2
    //
    // IPv4address = dec-octet "." dec-octet "." dec-octet "." dec-octet

    // First octet
    size_type processed = parse_dec_octet(input);
    size_type total = processed;
    if ((processed == 0) || (input[total] != token_dot))
        return 0;
    total += sizeof(token_dot);

    // Second octet
    processed = parse_dec_octet(input.substr(total));
    total += processed;
    if ((processed == 0) || (input[total] != token_dot))
        return 0;
    total += sizeof(token_dot);

    // Third octet
    processed = parse_dec_octet(input.substr(total));
    total += processed;
    if ((processed == 0) || (input[total] != token_dot))
        return 0;
    total += sizeof(token_dot);

    // Fourth octet
    processed = parse_dec_octet(input.substr(total));
    if (processed == 0)
        return 0;
    total += processed;

    return total;
}

inline uri_view::size_type uri_view::parse_ipv6address(const string_view& input)
{
    // RFC 3986 Section 3.2.2
    //
    // IPv6address =                            6( h16 ":" ) ls32
    //             /                       "::" 5( h16 ":" ) ls32
    //             / [               h16 ] "::" 4( h16 ":" ) ls32
    //             / [ *1( h16 ":" ) h16 ] "::" 3( h16 ":" ) ls32
    //             / [ *2( h16 ":" ) h16 ] "::" 2( h16 ":" ) ls32
    //             / [ *3( h16 ":" ) h16 ] "::"    h16 ":"   ls32
    //             / [ *4( h16 ":" ) h16 ] "::"              ls32
    //             / [ *5( h16 ":" ) h16 ] "::"              h16
    //             / [ *6( h16 ":" ) h16 ] "::"

    size_type processed = parse_ipv6address_1(input);
    if (processed > 0)
        return processed;

    processed = parse_ipv6address_2(input);
    if (processed > 0)
        return processed;

    processed = parse_ipv6address_3(input, 1, 4);
    if (processed > 0)
        return processed;

    processed = parse_ipv6address_3(input, 2, 3);
    if (processed > 0)
        return processed;

    processed = parse_ipv6address_3(input, 3, 2);
    if (processed > 0)
        return processed;

    processed = parse_ipv6address_3(input, 4, 1);
    if (processed > 0)
        return processed;

    processed = parse_ipv6address_3(input, 5, 0);
    if (processed > 0)
        return processed;

    processed = parse_ipv6address_4(input);
    if (processed > 0)
        return processed;

    processed = parse_ipv6address_5(input);
    if (processed > 0)
        return processed;

    return 0;
}

inline uri_view::size_type uri_view::parse_ipv6address_1(const string_view& input)
{
    // See parse_ipv6address
    //
    // 6( h16 ":" ) ls32

    string_view::const_iterator current = input.begin();

    size_type processed = 0;
    for (size_type after = 0; after < 6; ++after)
    {
        processed = parse_ipv6address_h16(&*current);
        if (processed == 0)
            return 0;
        current += processed;
        if (current == input.end() || (*current != token_colon))
            return 0;
        ++current;
        if (current == input.end())
            return 0;
    }

    processed = parse_ipv6address_ls32(&*current);
    if (processed == 0)
        return 0;
    current += processed;

    return std::distance(input.begin(), current);
}

inline uri_view::size_type uri_view::parse_ipv6address_2(const string_view& input)
{
    // See parse_ipv6address
    //
    // "::" 5( h16 ":" ) ls32

    string_view::const_iterator current = input.begin();

    if ((current == input.end()) || (*current != token_colon))
        return 0;
    ++current;
    if ((current == input.end()) || (*current != token_colon))
        return 0;
    ++current;

    size_type processed = 0;
    for (size_type after = 0; after < 5; ++after)
    {
        processed = parse_ipv6address_h16(&*current);
        if (processed == 0)
            return 0;
        current += processed;
        if (current == input.end() || (*current != token_colon))
            return 0;
        ++current;
        if (current == input.end())
            return 0;
    }

    processed = parse_ipv6address_ls32(&*current);
    if (processed == 0)
        return 0;
    current += processed;
    
    return std::distance(input.begin(), current);
}

inline uri_view::size_type uri_view::parse_ipv6address_3(const string_view& input,
                                                         size_type beforeLimit,
                                                         size_type afterLimit)
{
    // See parse_ipv6address
    //
    // [ h16 ] "::" 4( h16 ":" ) ls32
    // [ *1( h16 ":" ) h16 ] "::" 3( h16 ":" ) ls32
    // [ *2( h16 ":" ) h16 ] "::" 2( h16 ":" ) ls32
    // [ *3( h16 ":" ) h16 ] "::"    h16 ":"   ls32

    string_view::const_iterator current = input.begin();

    size_type processed = 0;
    for (size_type before = 0; before < beforeLimit; ++before)
    {
        processed = parse_ipv6address_h16(&*current);
        current += processed;
        if ((current == input.end()) || (*current != token_colon))
            return 0;
        ++current;
        if (current == input.end())
            return 0;
        if (*current == token_colon)
            break;
    }
    if (*current != token_colon)
        return 0;
    ++current;

    for (size_type after = 0; after < afterLimit; ++after)
    {
        processed = parse_ipv6address_h16(&*current);
        if (processed == 0)
            return 0;
        current += processed;
        if (current == input.end() || (*current != token_colon))
            return 0;
        ++current;
        if (current == input.end())
            return 0;
    }

    processed = parse_ipv6address_ls32(&*current);
    if (processed == 0)
        return 0;
    current += processed;
    
    return std::distance(input.begin(), current);
}

inline uri_view::size_type uri_view::parse_ipv6address_4(const string_view& input)
{
    // See parse_ipv6address
    //
    // [ *5( h16 ":" ) h16 ] "::"              h16

    string_view::const_iterator current = input.begin();

    size_type processed = 0;
    for (size_type before = 0; before < 6; ++before)
    {
        processed = parse_ipv6address_h16(&*current);
        current += processed;
        if ((current == input.end()) || (*current != token_colon))
            return 0;
        ++current;
        if (current == input.end())
            return 0;
        if (*current == token_colon)
            break;
    }
    if (*current != token_colon)
        return 0;
    ++current;

    processed = parse_ipv6address_h16(&*current);
    if (processed == 0)
        return 0;
    current += processed;
    
    return std::distance(input.begin(), current);
}

inline uri_view::size_type uri_view::parse_ipv6address_5(const string_view& input)
{
    // See parse_ipv6address
    //
    // [ *6( h16 ":" ) h16 ] "::"

    string_view::const_iterator current = input.begin();

    size_type processed = 0;
    for (size_type before = 0; before < 7; ++before)
    {
        processed = parse_ipv6address_h16(&*current);
        current += processed;
        if ((current == input.end()) || (*current != token_colon))
            return 0;
        ++current;
        if (current == input.end())
            return 0;
        if (*current == token_colon)
            break;
    }
    if (*current != token_colon)
        return 0;
    ++current;

    return std::distance(input.begin(), current);
}

inline uri_view::size_type uri_view::parse_ipv6address_h16(string_view input)
{
    // RFC 3986 Section 3.2.2
    //
    // h16         = 1*4HEXDIG
    //             ; 16 bits of address represented in hexadecimal

    if (input.size() < 4)
        return 0;

    if (!is_hexdig_token(input[0]))
        return 0;
    if (!is_hexdig_token(input[1]))
        return 1;
    if (!is_hexdig_token(input[2]))
        return 2;
    if (!is_hexdig_token(input[3]))
        return 3;
    return 4;
}

inline uri_view::size_type uri_view::parse_ipv6address_ls32(string_view input)
{
    // RFC 3986 Section 3.2.2
    //
    // ls32        = ( h16 ":" h16 ) / IPv4address
    //             ; least-significant 32 bits of address

    string_view::const_iterator current = input.begin();
    size_type processed = parse_ipv4address(&*current);
    if (processed > 0)
        return processed;

    processed = parse_ipv6address_h16(&*current);
    if (processed == 0)
        return 0;
    std::advance(current, processed);
    if (*current != token_colon)
        return 0;
    std::advance(current, 1);
    processed = parse_ipv6address_h16(&*current);
    if (processed == 0)
        return 0;
    std::advance(current, processed);
    return std::distance(input.begin(), current);
}

inline uri_view::size_type uri_view::parse_regname(string_view input)
{
    // RFC 3986 Section 3.2.2
    //
    // reg-name = *( unreserved / pct-encoded / sub-delims )

    return 0; // FIXME
}

inline uri_view::size_type uri_view::parse_dec_octet(string_view input)
{
    // RFC 3986 Section 3.2.2
    //
    // dec-octet   = DIGIT                 ; 0-9
    //             / %x31-39 DIGIT         ; 10-99
    //             / "1" 2DIGIT            ; 100-199
    //             / "2" %x30-34 DIGIT     ; 200-249
    //             / "25" %x30-35          ; 250-255
    switch (input[0])
    {
    case 0x30:
        return 1;

    case 0x31:
        if (!is_digit_token(input[1]))
            return 1;
        if (!is_digit_token(input[2]))
            return 2;
        return 3;

    case 0x32:
        switch (input[1])
        {
        case 0x30: case 0x31: case 0x32: case 0x33:
        case 0x34:
            if (is_digit_token(input[2]))
                return 3;
            return 2;

        case 0x35:
            switch (input[2])
            {
            case 0x30: case 0x31: case 0x32: case 0x33:
            case 0x34: case 0x35:
                return 3;
            default:
                return 2; // FIXME
            }

        default:
            return 1;
        }

    case 0x33:
    case 0x34:
    case 0x35:
    case 0x36:
    case 0x37:
    case 0x38:
    case 0x39:
        if (is_digit_token(input[1]))
            return 2;
        return 1;

    default:
        return 0;
    }
}

inline uri_view::size_type uri_view::parse_port(string_view input)
{
    // RFC 3986 Section 3.2.3
    //
    // port = *DIGIT

    string_view::const_iterator current = input.begin();
    while (current != input.end())
    {
        if (!is_digit_token(*current))
            break;
        ++current;
    }
    size_type processed = std::distance(input.begin(), current);
    if (processed == 0)
        return 0;
    port_view = input.substr(0, processed);
    return processed;
}

inline uri_view::size_type uri_view::parse_path_abempty(string_view input)
{
    // RFC 3986 Section 3.3
    //
    // path-abempty  = *( "/" segment )

    size_type total = 0;
    while (input[0] == token_slash)
    {
        size_type processed = sizeof(token_slash);
        input.remove_prefix(processed);
        total += processed;
        processed = parse_segment(input);
        if (processed == 0)
            break;
        total += processed;
        input.remove_prefix(processed);
    }
    return total;
}

inline uri_view::size_type uri_view::parse_segment(string_view input)
{
    // RFC 3986 Section 3.3
    //
    // segment = *pchar

    string_view::const_iterator current = input.begin();
    while (current != input.end())
    {
        size_type processed = parse_pchar(&*current);
        if (processed == 0)
            break;
        std::advance(current, processed);
    }
    return std::distance(input.begin(), current);
}

inline uri_view::size_type uri_view::parse_pchar(string_view input)
{
    // RFC 3986 Section 3.3
    //
    // pchar = unreserved / pct-encoded / sub-delims / ":" / "@"

    switch (input[0])
    {
    case token_percent:
        return parse_pct_encoded(input);

    case token_colon:
    case token_at:
        return 1;

    default:
        if (is_unreserved_token(input[0]) || is_subdelims_token(input[0]))
            return 1;
        return 0;
    }
}

inline uri_view::size_type uri_view::parse_pct_encoded(string_view input)
{
    // pct-encoded = "%" HEXDIG HEXDIG

    return 0; // FIXME
}

//-----------------------------------------------------------------------------
// Tokens
//-----------------------------------------------------------------------------

inline bool uri_view::is_alpha_token(value_type value) const
{
    // RFC 2234 Section 6.1
    //
    // ALPHA = %x41-5A / %x61-7A  ; A-Z / a-z

    switch (value)
    {
    case 0x41: case 0x42: case 0x43: // A-Z
    case 0x44: case 0x45: case 0x46: case 0x47:
    case 0x48: case 0x49: case 0x4A: case 0x4B:
    case 0x4C: case 0x4D: case 0x4E: case 0x4F:
    case 0x50: case 0x51: case 0x52: case 0x53:
    case 0x54: case 0x55: case 0x56: case 0x57:
    case 0x58: case 0x59: case 0x5A:
    case 0x61: case 0x62: case 0x63: // a-z
    case 0x64: case 0x65: case 0x66: case 0x67:
    case 0x68: case 0x69: case 0x6A: case 0x6B:
    case 0x6C: case 0x6D: case 0x6E: case 0x6F:
    case 0x70: case 0x71: case 0x72: case 0x73:
    case 0x74: case 0x75: case 0x76: case 0x77:
    case 0x78: case 0x79: case 0x7A:
        return true;
    default:
        return false;
    }
}

inline bool uri_view::is_digit_token(value_type value) const
{
    // RFC 2234 Section 6.1
    //
    // DIGIT =  %x30-39 ; 0-9

    switch (value)
    {
    case 0x30: case 0x31: case 0x32: case 0x33:
    case 0x34: case 0x35: case 0x36: case 0x37:
    case 0x38: case 0x39:
        return true;
    default:
        return false;
    }
}

inline bool uri_view::is_hexdig_token(value_type value) const
{
    // RFC 2234 Section 6.1
    //
    // HEXDIG =  DIGIT / "A" / "B" / "C" / "D" / "E" / "F"

    switch (value)
    {
    case 0x30: case 0x31: case 0x32: case 0x33:
    case 0x34: case 0x35: case 0x36: case 0x37:
    case 0x38: case 0x39:
    case 0x41: case 0x42: case 0x43: case 0x44: // A-F
    case 0x45: case 0x46:
    case 0x61: case 0x62: case 0x63: case 0x64: // a-f
    case 0x65: case 0x66:
        return true;
    default:
        return false;
    }
}
    
inline bool uri_view::is_scheme_token(value_type value) const
{
    // RFC 3986 Section 3.1
    //
    // Optional part of scheme.
    //
    // ALPHA / DIGIT / "+" / "-" / "."

    switch (value)
    {
    case token_plus:
    case token_minus:
    case token_dot:
        return true;

    default:
        return (is_alpha_token(value) || is_digit_token(value));
    }
}

inline bool uri_view::is_unreserved_token(value_type value) const
{
    // unreserved = ALPHA / DIGIT / "-" / "." / "_" / "~"

    switch (value)
    {
    case token_minus:
    case token_dot:
    case token_underscore:
    case token_tilde:
        return true;

    default:
        return (is_alpha_token(value) || is_digit_token(value));
    }
}

inline bool uri_view::is_subdelims_token(value_type value) const
{
    // sub-delims = "!" / "$" / "&" / "'" / "(" / ")" / "*" / "+" / "," / ";" / "="

    switch (value)
    {
    case token_exclamation:
    case token_dollar:
    case token_at:
    case token_apostrophe:
    case token_parens_open:
    case token_parens_close:
    case token_asterisk:
    case token_comma:
    case token_semicolon:
    case token_equal:
        return true;

    default:
        return false;
    }
}

} // namespace network

#endif // NETWORK_DETAIL_URI_VIEW_IPP
