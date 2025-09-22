# Include standard installation directory variables
include(GNUInstallDirs)

# Create the library
add_library(secutil STATIC secure_erase.cpp)
add_library(Terra::secutil ALIAS secutil)

# Specify the internal and public include directories
target_include_directories(secutil
    PRIVATE
        ${CMAKE_CURRENT_BINARY_DIR}
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

# Specify the C++ standard to observe
set_target_properties(secutil
    PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF)

# Check for the existence of memset_s and explicit_bzero
include(${CMAKE_CURRENT_SOURCE_DIR}/../cmake/memset_s.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/../cmake/explicit_bzero.cmake)

if(HAVE_EXPLICIT_BZERO)
    target_compile_definitions(secutil PRIVATE HAVE_EXPLICIT_BZERO)
endif()

if(HAVE_MEMSET_S)
    # Must define __STDC_WANT_LIB_EXT1__ to get memset_s
    target_compile_definitions(secutil PRIVATE __STDC_WANT_LIB_EXT1__=1)
    target_compile_definitions(secutil PRIVATE HAVE_MEMSET_S)
endif()

# If requesting clang-tidy, try to look for it
if(secutil_CLANG_TIDY)
    find_program(CLANG_TIDY_COMMAND NAMES "clang-tidy")
    if(CLANG_TIDY_COMMAND)
        set_target_properties(secutil PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_COMMAND}")
    else()
        message(WARNING "Could not find clang-tidy")
    endif()
endif()

# Use the following compile options
target_compile_options(secutil
    PRIVATE
        $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:-Wpedantic -Wextra -Wall -Werror>
        $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>)

# Install target and associated include files
if(secutil_INSTALL)
    install(TARGETS secutil EXPORT secutilTargets ARCHIVE)
    install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/ TYPE INCLUDE)
    install(EXPORT secutilTargets
            FILE secutilConfig.cmake
            NAMESPACE Terra::
            DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/secutil)
endif()
