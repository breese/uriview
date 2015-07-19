#ifndef NETWORK_DETAIL_TRAITS_IPP
#define NETWORK_DETAIL_TRAITS_IPP

///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2015 Bjorn Reese <breese@users.sourceforge.net>
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

namespace network
{
namespace detail
{

//-----------------------------------------------------------------------------
// char specialization
//-----------------------------------------------------------------------------

inline bool traits<char>::is_digit(value_type value) BOOST_NOEXCEPT
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

inline bool traits<char>::is_alpha(value_type value) BOOST_NOEXCEPT
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

inline bool traits<char>::is_hexdigit(value_type value) BOOST_NOEXCEPT
{
    return is_digit(value) || is_hex_upper(value) || is_hex_lower(value);
}

inline bool traits<char>::is_hex_upper(value_type value) BOOST_NOEXCEPT
{
    switch (value)
    {
    case 0x41: case 0x42: case 0x43: case 0x44: // A-F
    case 0x45: case 0x46:
        return true;
    default:
        return false;
    }
}

inline bool traits<char>::is_hex_lower(value_type value) BOOST_NOEXCEPT
{
    switch (value)
    {
    case 0x61: case 0x62: case 0x63: case 0x64: // a-f
    case 0x65: case 0x66:
        return true;
    default:
        return false;
    }
}

inline typename traits<char>::int_type
traits<char>::hex_to_int(value_type value) BOOST_NOEXCEPT
{
    if (is_digit(value))
        return value - alpha_0;
    if (is_hex_upper(value))
        return value - alpha_A + 10;
    if (is_hex_lower(value))
        return value - alpha_a + 10;
    assert(false);
    return 0;
}

} // namespace detail
} // namespace network

#endif // NETWORK_DETAIL_TRAITS_IPP
