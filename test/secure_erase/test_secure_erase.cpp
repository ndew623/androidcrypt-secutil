/*
 *  test_secure_erase.cpp
 *
 *  Copyright (C) 2024
 *  Terrapane Corporation
 *  All Rights Reserved
 *
 *  Author:
 *      Paul E. Jones <paulej@packetizer.com>
 *
 *  Description:
 *      Unit tests for the SecureErase function.
 *
 *  Portability Issues:
 *      None.
 */

#include <vector>
#include <cstdint>
#include <cstddef>
#include <span>
#include <terra/secutil/secure_erase.h>
#include <terra/secutil/secure_string.h>
#include <terra/stf/stf.h>

using namespace Terra;

bool CheckZeroSpan(std::span<const std::uint32_t> block)
{
    for (const auto i : block) if (i != 0) return false;

    return true;
}

STF_TEST(SecureErase, EraseBuffer)
{
    constexpr std::size_t Buffer_Size{128};
    std::vector<uint8_t> zeros(Buffer_Size, 0);
    std::uint8_t *buffer = new std::uint8_t[Buffer_Size];

    // Initialize the buffer
    for (std::size_t i = 0; i < Buffer_Size; i++) buffer[i] = i % 10;

    // Erase the buffer
    SecUtil::SecureErase(buffer, Buffer_Size);

    // Verify the buffer is erased
    STF_ASSERT_MEM_EQ(buffer, &zeros.front(), Buffer_Size);

    // Free buffer
    delete[] buffer;
}

STF_TEST(SecureErase, EraseString)
{
    std::string string = "This is a test";
    std::vector<uint8_t> zeros(14, 0);

    // Erase the string
    SecUtil::SecureErase(string);

    // Verify the string is erased
    STF_ASSERT_MEM_EQ(string.data(), &zeros.front(), zeros.size());
}

STF_TEST(SecureErase, EraseWideString)
{
    std::wstring string = L"This is a test";
    std::wstring zero_string(14, L'\0');

    // Verify the strings are not equal
    STF_ASSERT_NE(zero_string, string);

    // Erase the string
    SecUtil::SecureErase(string);

    // Verify the string is erased
    STF_ASSERT_EQ(zero_string, string);
}

STF_TEST(SecureErase, EraseIntegral)
{
    std::uint32_t i = 100;

    SecUtil::SecureErase(i);

    STF_ASSERT_EQ(0, i);
}

STF_TEST(SecureErase, EraseEnum)
{
    enum class EnumType
    {
        Type1 = 0,
        Type2
    };

    EnumType enum_type = EnumType::Type2;

    SecUtil::SecureErase(enum_type);

    STF_ASSERT_EQ(EnumType::Type1, enum_type);
}

STF_TEST(SecureErase, ErasePointer)
{
    int i = 100;
    int *p = &i;

    STF_ASSERT_NE(p, nullptr);

    SecUtil::SecureErase(p);

    STF_ASSERT_EQ(p, nullptr);

    STF_ASSERT_EQ(100, i);
}

STF_TEST(SecureErase, EraseArray)
{
    std::array<std::uint32_t, 10> elements;

    // Populate the array
    for (std::size_t i = 0; i < elements.size(); i++) elements[i] = 0xdeadbeef;

    STF_ASSERT_FALSE(CheckZeroSpan(elements));

    SecUtil::SecureErase(elements);

    STF_ASSERT_TRUE(CheckZeroSpan(elements));
}

STF_TEST(SecureErase, EraseVector)
{
    std::vector<std::uint32_t> elements(10, 0xdeadbeef);

    STF_ASSERT_FALSE(CheckZeroSpan(elements));

    SecUtil::SecureErase(elements);

    STF_ASSERT_TRUE(CheckZeroSpan(elements));
}
