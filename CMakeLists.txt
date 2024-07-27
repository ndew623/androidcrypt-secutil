cmake_minimum_required(VERSION 3.21)

# Define the secutil project
project(secutil
        VERSION 1.0.0.0
        DESCRIPTION "Security-Related Utilities Library"
        LANGUAGES CXX)

# Set options depending on whether this is a subproject
if(PROJECT_IS_TOP_LEVEL)
    # Option to control whether tests are built
    option(secutil_BUILD_TESTS "Build Tests for Security Utilities Library" ON)
else()
    # Option to control whether tests are built
    option(secutil_BUILD_TESTS "Build Tests for Security Utilities Library" OFF)
endif()

# Option to control ability to install the library
option(secutil_INSTALL "Install the Security-Related Utilities Library" ON)

add_subdirectory(dependencies)
add_subdirectory(src)

include(CTest)

if(BUILD_TESTING AND secutil_BUILD_TESTS)
    add_subdirectory(test)
endif()
