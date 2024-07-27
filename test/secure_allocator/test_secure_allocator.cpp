/*
 *  test_secure_allocator.cpp
 *
 *  Copyright (C) 2024
 *  Terrapane Corporation
 *  All Rights Reserved
 *
 *  Author:
 *      Paul E. Jones <paulej@packetizer.com>
 *
 *  Description:
 *      Unit tests for the SecureAllocator object.
 *
 *  Portability Issues:
 *      None.
 */

#include <vector>
#include <list>
#include <cstdint>
#include <cstddef>
#include <string>
#include <terra/secutil/secure_allocator.h>
#include <terra/stf/stf.h>

using namespace Terra;

// Used to ensure symmetric calls are made
static unsigned Allocations = 0;
static unsigned Deallocations = 0;

// Define a derived class to catch calls to allocate/deallocate
template<typename T>
struct TestSecureAllocator : public SecUtil::SecureAllocator<T>
{
    // Constructors and destructor
    constexpr TestSecureAllocator() = default;
    template<typename U>
    constexpr TestSecureAllocator(const TestSecureAllocator<U> &) noexcept
    {
    }
    constexpr ~TestSecureAllocator() = default;

    constexpr T *allocate(std::size_t n)
    {
        Allocations++;
        return SecUtil::SecureAllocator<T>::allocate(n);
    }
    constexpr void deallocate(T *p, std::size_t n) noexcept
    {
        Deallocations++;
        SecUtil::SecureAllocator<T>::deallocate(p, n);
    }
};

STF_TEST(SecureAllocator, TestSecureVector)
{
    // Reset globals
    Allocations = 0;
    Deallocations = 0;

    {
        std::vector<int, TestSecureAllocator<int>> secure_vector;

        secure_vector.push_back(1);
        secure_vector.push_back(2);
        secure_vector.push_back(3);
        secure_vector.push_back(4);
        secure_vector.resize(100);
    }

    // There should be an equal number of allocations and deallocations
    STF_ASSERT_EQ(Allocations, Deallocations);

    // Ensure there was at least 1 allocation
    STF_ASSERT_GT(Allocations, 0);
}

STF_TEST(SecureAllocator, TestSecureList)
{
    // Reset globals
    Allocations = 0;
    Deallocations = 0;

    {
        std::list<int, TestSecureAllocator<int>> secure_list;

        secure_list.push_back(1);
        secure_list.push_back(2);
        secure_list.push_back(3);
        secure_list.push_back(4);
        secure_list.push_back(5);
        secure_list.push_back(6);
        secure_list.push_back(7);
    }

    // There should be an equal number of allocations and deallocations
    STF_ASSERT_EQ(Allocations, Deallocations);

    // Ensure there was at least 1 allocation
    STF_ASSERT_GT(Allocations, 0);
}

STF_TEST(SecureAllocator, TestSecureString)
{
    // Reset globals
    Allocations = 0;
    Deallocations = 0;

    {
        using SecureString = std::basic_string<char,
                                               std::char_traits<char>,
                                               TestSecureAllocator<char>>;

        SecureString s;
        for (auto i = 0; i < 1000; i++) s += "a";
        STF_ASSERT_EQ(1000, s.length());
    }

    // There should be an equal number of allocations and deallocations
    STF_ASSERT_EQ(Allocations, Deallocations);

    // Ensure there was at least 1 allocation
    STF_ASSERT_GT(Allocations, 0);
}
