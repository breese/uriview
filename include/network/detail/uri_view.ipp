#ifndef NETWORK_DETAIL_URI_VIEW_IPP
#define NETWORK_DETAIL_URI_VIEW_IPP

#include <cassert>

namespace network
{

inline uri_view::uri_view(const string_view& input)
{
    parse(input);
}

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

inline void uri_view::parse(string_view input)
{
    // RFC 3986 Section 3
    //
    // URI = scheme ":" hier-part [ "?" query ] [ "#" fragment ]

    if (input.empty())
        return;

    size_type processed = parse_scheme(input);
    input.remove_prefix(processed);

    const value_type colon = ':';
    if (input.empty() || (input[0] != colon))
        return; // FIXME: Report error
    input.remove_prefix(sizeof(colon));

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

    if (!is_alpha(*current))
        return 0;
    ++current;

    for (;
         current != input.end();
         ++current)
    {
        if (!is_scheme(*current))
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

    if ((input[0] == '/') && (input[1] == '/'))
    {
        input.remove_prefix(2);
        size_type processed = parse_authority(input);
        // FIXME: path-abempty
        return processed;
    }
    else
    {
        assert(false); // Not implemented yet
    }
    return 0;
}

inline uri_view::size_type uri_view::parse_authority(string_view input)
{
    // RFC 3986 Section 3.2
    //
    // authority = [ userinfo "@" ] host [ ":" port ]

    size_type processed = parse_userinfo(input);
    const value_type ampersand = '@';
    if (input[processed] == ampersand)
    {
        userinfo_view = input.substr(processed);
        input.remove_prefix(processed + sizeof(ampersand));
    }

    processed = parse_host(input);
    if (processed == 0)
        return 0;

    const value_type colon = ':';
    if (input[processed] == colon)
    {
        return parse_port(input.substr(processed + sizeof(colon)));
    }
    return processed;
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

    size_type processed = parse_ipv4address(input);
    if (processed == 0)
        return 0;

    host_view = input.substr(0, processed);
    return processed;
}

inline uri_view::size_type uri_view::parse_ipv4address(string_view input)
{
    // RFC 3986 Section 3.2.2
    //
    // IPv4address = dec-octet "." dec-octet "." dec-octet "." dec-octet

    const value_type dot = '.';

    // First octet
    size_type processed = parse_dec_octet(input);
    size_type total = processed;
    if ((processed == 0) || (input[total] != dot))
        return 0;
    total += sizeof(dot);

    // Second octet
    processed = parse_dec_octet(input.substr(total));
    total += processed;
    if ((processed == 0) || (input[total] != dot))
        return 0;
    total += sizeof(dot);

    // Third octet
    processed = parse_dec_octet(input.substr(total));
    total += processed;
    if ((processed == 0) || (input[total] != dot))
        return 0;
    total += sizeof(dot);

    // Fourth octet
    processed = parse_dec_octet(input.substr(total));
    if (processed == 0)
        return 0;
    total += processed;

    return total;
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
        if (!is_digit(input[1]))
            return 1;
        if (!is_digit(input[2]))
            return 2;
        return 3;

    case 0x32:
        switch (input[1])
        {
        case 0x30: case 0x31: case 0x32: case 0x33:
        case 0x34:
            if (is_digit(input[2]))
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
        if (is_digit(input[1]))
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
        if (!is_digit(*current))
            break;
        ++current;
    }
    size_type processed = std::distance(input.begin(), current);
    if (processed == 0)
        return 0;
    port_view = input.substr(0, processed);
    return processed;
}

inline bool uri_view::is_alpha(value_type value) const
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

inline bool uri_view::is_digit(value_type value) const
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

inline bool uri_view::is_scheme(value_type value) const
{
    // RFC 3986 Section 3.1
    //
    // Optional part of scheme.
    //
    // ALPHA / DIGIT / "+" / "-" / "."

    switch (value)
    {
    case 0x2B: // +
    case 0x2D: // -
    case 0x2E: // .
    case 0x30: case 0x31: case 0x32: case 0x33: // DIGIT
    case 0x34: case 0x35: case 0x36: case 0x37:
    case 0x38: case 0x39:
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

} // namespace network

#endif // NETWORK_DETAIL_URI_VIEW_IPP
