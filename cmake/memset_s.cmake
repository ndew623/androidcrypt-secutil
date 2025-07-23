#
# Check for existence of memset_s
#

include(CheckCXXSourceCompiles)

# Check to see if memset_s() is present
check_cxx_source_compiles("
    #define __STDC_WANT_LIB_EXT1__ 1
    #include <string.h>
    #include <array>
    int main()
    {
        std::array<char, 32> buffer{};
        return memset_s(buffer.data(), buffer.size(), 0, buffer.size());
    }
" HAVE_MEMSET_S)
