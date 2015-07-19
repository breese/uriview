###############################################################################
#
# Copyright (C) 2015 Bjorn Reese <breese@users.sourceforge.net>
#
# Distributed under the Boost Software License, Version 1.0.
#    (See accompanying file LICENSE_1_0.txt or copy at
#          http://www.boost.org/LICENSE_1_0.txt)
#
###############################################################################

cmake_minimum_required(VERSION 2.8)
project(network.uri)

set(NETWORK_URI_BUILD_DIR ${CMAKE_BINARY_DIR})
set(NETWORK_URI_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
set(LIBRARY_OUTPUT_PATH ${NETWORK_URI_BUILD_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${NETWORK_URI_BUILD_DIR}/bin)

add_definitions(-g)
add_definitions(-Wall)
#add_definitions(-std=c++11)

###############################################################################
# Boost package
###############################################################################

set(Boost_COMPONENTS system unit_test_framework system)
find_package(Boost 1.55.0 COMPONENTS ${Boost_COMPONENTS})
if (NOT ${Boost_FOUND})
  message(FATAL_ERROR "${Boost_ERROR_REASON}")
endif()
include_directories(BEFORE ${Boost_INCLUDE_DIR})
set(NETWORK_URI_DEPENDENT_LIBRARIES
  ${Boost_SYSTEM_LIBRARY})

###############################################################################
# Network.Uri package
###############################################################################

include_directories(BEFORE include)

###############################################################################
# Test
###############################################################################

add_executable(test_uri
  test/runner.cpp
  test/uri_view_suite.cpp
  test/form_view_suite.cpp
)

target_link_libraries(test_uri
  ${NETWORK_URI_DEPENDENT_LIBRARIES}
  ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
