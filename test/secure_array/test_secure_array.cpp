/*
 *  test_secure_array.cpp
 *
 *  Copyright (C) 2024
 *  Terrapane Corporation
 *  All Rights Reserved
 *
 *  Author:
 *      Paul E. Jones <paulej@packetizer.com>
 *
 *  Description:
 *      Unit tests for the SecureArray object.
 *
 *  Portability Issues:
 *      None.
 */

#include <cstdint>
#include <cstring>
#include <span>
#include <terra/secutil/secure_array.h>
#include <terra/stf/stf.h>

using namespace Terra;

bool CheckZero16(std::span<const std::uint8_t, 16> block)
{
    for (const auto i : block) if (i != 0) return false;

    return true;
}

bool CheckZeroAnySize(std::span<const std::uint8_t> block)
{
    for (const auto i : block) if (i != 0) return false;

    return true;
}

bool CheckZeroAnySize(std::span<const char32_t> block)
{
    for (const auto i : block) if (i != 0) return false;

    return true;
}

enum class EnumClass
{
    Type1,
    Type2
};

bool CheckZeroAnySize(std::span<const EnumClass> block)
{
    for (const auto i : block) if (static_cast<unsigned>(i) != 0) return false;

    return true;
}

STF_TEST(SecureArray, TestExplicitExtent)
{
    SecUtil::SecureArray<std::uint8_t, 16> array_of_octets;

    STF_ASSERT_EQ(16, array_of_octets.size());

    array_of_octets[0] = 25;

    STF_ASSERT_FALSE(CheckZero16(array_of_octets));

    // Cannot test the array after destruction, but this is effectively
    // what is called during destruction
    SecUtil::SecureErase(array_of_octets);

    STF_ASSERT_TRUE(CheckZero16(array_of_octets));
}

STF_TEST(SecureArray, TestImplicitExtent)
{
    SecUtil::SecureArray<std::uint8_t, 24> array_of_octets;

    STF_ASSERT_EQ(24, array_of_octets.size());

    array_of_octets[0] = 25;

    // Test that the extent is implicitly determined
    STF_ASSERT_FALSE(CheckZeroAnySize(array_of_octets));

    // Cannot test the array after destruction, but this is effectively
    // what is called during destruction
    SecUtil::SecureErase(array_of_octets);

    STF_ASSERT_TRUE(CheckZeroAnySize(array_of_octets));
}

STF_TEST(SecureArray, TestArrayOfChars)
{
    SecUtil::SecureArray<char32_t, 10> array_of_octets;

    STF_ASSERT_EQ(10, array_of_octets.size());

    array_of_octets[0] = 10;

    // Test that the extent is implicitly determined
    STF_ASSERT_FALSE(CheckZeroAnySize(array_of_octets));

    // Cannot test the array after destruction, but this is effectively
    // what is called during destruction
    SecUtil::SecureErase(array_of_octets);

    STF_ASSERT_TRUE(CheckZeroAnySize(array_of_octets));
}

STF_TEST(SecureArray, TestArrayOfEnums)
{
    SecUtil::SecureArray<EnumClass, 10> array_of_octets;

    STF_ASSERT_EQ(10, array_of_octets.size());

    array_of_octets[0] = EnumClass::Type2;

    // Test that the extent is implicitly determined
    STF_ASSERT_FALSE(CheckZeroAnySize(array_of_octets));

    // Cannot test the array after destruction, but this is effectively
    // what is called during destruction
    SecUtil::SecureErase(array_of_octets);

    STF_ASSERT_TRUE(CheckZeroAnySize(array_of_octets));
}
