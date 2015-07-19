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

#include <network/form_view.hpp>

BOOST_AUTO_TEST_SUITE(form_view_suite)

BOOST_AUTO_TEST_CASE(test_empty)
{
    const char input[] = "";
    network::form_view form(input);
    BOOST_REQUIRE_EQUAL(form.next(), false);
}

BOOST_AUTO_TEST_CASE(test_single_pair)
{
    const char input[] = "alpha=hydrogen";
    network::form_view form(input);
    BOOST_REQUIRE_EQUAL(form.literal_key(), "alpha");
    BOOST_REQUIRE_EQUAL(form.literal_value(), "hydrogen");
    BOOST_REQUIRE_EQUAL(form.key(), "alpha");
    BOOST_REQUIRE_EQUAL(form.value<std::string>(), "hydrogen");
    BOOST_REQUIRE_EQUAL(form.next(), false);
}

BOOST_AUTO_TEST_CASE(test_single_space)
{
    const char input[] = "alpha=hydrogen+helium";
    network::form_view form(input);
    BOOST_REQUIRE_EQUAL(form.literal_key(), "alpha");
    BOOST_REQUIRE_EQUAL(form.literal_value(), "hydrogen+helium");
    BOOST_REQUIRE_EQUAL(form.key(), "alpha");
    BOOST_REQUIRE_EQUAL(form.value<std::string>(), "hydrogen helium");
    BOOST_REQUIRE_EQUAL(form.next(), false);
}

BOOST_AUTO_TEST_CASE(test_single_equal)
{
    const char input[] = "alpha=hydrogen%3Dhelium";
    network::form_view form(input);
    BOOST_REQUIRE_EQUAL(form.literal_key(), "alpha");
    BOOST_REQUIRE_EQUAL(form.literal_value(), "hydrogen%3Dhelium");
    BOOST_REQUIRE_EQUAL(form.key(), "alpha");
    BOOST_REQUIRE_EQUAL(form.value<std::string>(), "hydrogen=helium");
    BOOST_REQUIRE_EQUAL(form.next(), false);
}

BOOST_AUTO_TEST_CASE(test_list_two)
{
    const char input[] = "alpha=hydrogen&bravo=helium";
    network::form_view form(input);
    BOOST_REQUIRE_EQUAL(form.key(), "alpha");
    BOOST_REQUIRE_EQUAL(form.value<std::string>(), "hydrogen");
    BOOST_REQUIRE_EQUAL(form.next(), true);
    BOOST_REQUIRE_EQUAL(form.key(), "bravo");
    BOOST_REQUIRE_EQUAL(form.value<std::string>(), "helium");
    BOOST_REQUIRE_EQUAL(form.next(), false);
}

BOOST_AUTO_TEST_CASE(test_list_three)
{
    const char input[] = "alpha=hydrogen&bravo=helium&charlie=lithium";
    network::form_view form(input);
    BOOST_REQUIRE_EQUAL(form.key(), "alpha");
    BOOST_REQUIRE_EQUAL(form.value<std::string>(), "hydrogen");
    BOOST_REQUIRE_EQUAL(form.next(), true);
    BOOST_REQUIRE_EQUAL(form.key(), "bravo");
    BOOST_REQUIRE_EQUAL(form.value<std::string>(), "helium");
    BOOST_REQUIRE_EQUAL(form.next(), true);
    BOOST_REQUIRE_EQUAL(form.key(), "charlie");
    BOOST_REQUIRE_EQUAL(form.value<std::string>(), "lithium");
    BOOST_REQUIRE_EQUAL(form.next(), false);
}

BOOST_AUTO_TEST_CASE(fail_ampersand)
{
    const char input[] = "&";
    network::form_view form(input);
    BOOST_REQUIRE_EQUAL(form.next(), false);
}

BOOST_AUTO_TEST_CASE(fail_no_value)
{
    const char input[] = "alpha=";
    network::form_view form(input);
    BOOST_REQUIRE_EQUAL(form.next(), false);
}

BOOST_AUTO_TEST_SUITE_END()
