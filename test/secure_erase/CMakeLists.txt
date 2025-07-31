add_executable(test_secure_erase test_secure_erase.cpp)

target_link_libraries(test_secure_erase Terra::secutil Terra::stf)

# Specify the C++ standard to observe
set_target_properties(test_secure_erase
    PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF)

target_compile_options(test_secure_erase PRIVATE
    $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>: -Wpedantic -Wextra -Wall>
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>)

add_test(NAME test_secure_erase
         COMMAND test_secure_erase)
