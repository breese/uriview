///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2015 Bjorn Reese <breese@users.sourceforge.net>
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#include <boost/test/unit_test.hpp>

#include <network/uri_view.hpp>

BOOST_AUTO_TEST_SUITE(uri_view_suite)

BOOST_AUTO_TEST_CASE(test_empty)
{
    const char input[] = "";
    network::uri_view uri(input);
    BOOST_REQUIRE_EQUAL(uri.scheme(), "");
    BOOST_REQUIRE_EQUAL(uri.host(), "");
    BOOST_REQUIRE_EQUAL(uri.port(), "");
}

BOOST_AUTO_TEST_CASE(test_scheme)
{
    const char input[] = "scheme://";
    network::uri_view uri(input);
    BOOST_REQUIRE_EQUAL(uri.scheme(), "scheme");
    BOOST_REQUIRE_EQUAL(uri.host(), "");
    BOOST_REQUIRE_EQUAL(uri.port(), "");
}

BOOST_AUTO_TEST_CASE(test_host)
{
    const char input[] = "scheme://1.2.3.4";
    network::uri_view uri(input);
    BOOST_REQUIRE_EQUAL(uri.scheme(), "scheme");
    BOOST_REQUIRE_EQUAL(uri.host(), "1.2.3.4");
    BOOST_REQUIRE_EQUAL(uri.port(), "");
}

BOOST_AUTO_TEST_CASE(test_host_port)
{
    const char input[] = "scheme://1.2.3.4:80";
    network::uri_view uri(input);
    BOOST_REQUIRE_EQUAL(uri.scheme(), "scheme");
    BOOST_REQUIRE_EQUAL(uri.host(), "1.2.3.4");
    BOOST_REQUIRE_EQUAL(uri.port(), "80");
    BOOST_REQUIRE_EQUAL(uri.authority(), "1.2.3.4:80");
}

BOOST_AUTO_TEST_CASE(test_host_port_slash)
{
    const char input[] = "scheme://1.2.3.4:80/";
    network::uri_view uri(input);
    BOOST_REQUIRE_EQUAL(uri.scheme(), "scheme");
    BOOST_REQUIRE_EQUAL(uri.host(), "1.2.3.4");
    BOOST_REQUIRE_EQUAL(uri.port(), "80");
}

//-----------------------------------------------------------------------------
// IPv4 address
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_ipv4_increasing)
{
    const char input[] = "scheme://0.1.2.3";
    network::uri_view uri(input);
    BOOST_REQUIRE_EQUAL(uri.scheme(), "scheme");
    BOOST_REQUIRE_EQUAL(uri.host(), "0.1.2.3");
    BOOST_REQUIRE_EQUAL(uri.port(), "");
}

BOOST_AUTO_TEST_CASE(test_ipv4_zeroes)
{
    const char input[] = "scheme://0.0.0.0";
    network::uri_view uri(input);
    BOOST_REQUIRE_EQUAL(uri.scheme(), "scheme");
    BOOST_REQUIRE_EQUAL(uri.host(), "0.0.0.0");
    BOOST_REQUIRE_EQUAL(uri.port(), "");
}

BOOST_AUTO_TEST_CASE(test_ipv4_ones)
{
    const char input[] = "scheme://1.10.100.0";
    network::uri_view uri(input);
    BOOST_REQUIRE_EQUAL(uri.scheme(), "scheme");
    BOOST_REQUIRE_EQUAL(uri.host(), "1.10.100.0");
    BOOST_REQUIRE_EQUAL(uri.port(), "");
}

BOOST_AUTO_TEST_CASE(test_ipv4_twos)
{
    const char input[] = "scheme://2.20.200.0";
    network::uri_view uri(input);
    BOOST_REQUIRE_EQUAL(uri.scheme(), "scheme");
    BOOST_REQUIRE_EQUAL(uri.host(), "2.20.200.0");
    BOOST_REQUIRE_EQUAL(uri.port(), "");
}

BOOST_AUTO_TEST_CASE(test_ipv4_two_halfs)
{
    const char input[] = "scheme://2.25.250.255";
    network::uri_view uri(input);
    BOOST_REQUIRE_EQUAL(uri.scheme(), "scheme");
    BOOST_REQUIRE_EQUAL(uri.host(), "2.25.250.255");
    BOOST_REQUIRE_EQUAL(uri.port(), "");
}

BOOST_AUTO_TEST_CASE(fail_ipv4_large)
{
    const char input[] = "scheme://256.0.0.0";
    network::uri_view uri(input);
    BOOST_REQUIRE_EQUAL(uri.scheme(), "scheme");
    BOOST_REQUIRE_EQUAL(uri.host(), "");
    BOOST_REQUIRE_EQUAL(uri.port(), "");
}

//-----------------------------------------------------------------------------
// Path
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_path_plain)
{
    const char input[] = "scheme://0.1.2.3:80/path";
    network::uri_view uri(input);
    BOOST_REQUIRE_EQUAL(uri.scheme(), "scheme");
    BOOST_REQUIRE_EQUAL(uri.host(), "0.1.2.3");
    BOOST_REQUIRE_EQUAL(uri.port(), "80");
    BOOST_REQUIRE_EQUAL(uri.path(), "/path");
}

BOOST_AUTO_TEST_SUITE_END()
