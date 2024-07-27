/*
 *  test_secure_types.cpp
 *
 *  Copyright (C) 2024
 *  Terrapane Corporation
 *  All Rights Reserved
 *
 *  Author:
 *      Paul E. Jones <paulej@packetizer.com>
 *
 *  Description:
 *      Unit tests for various secure types (e.g., strings, vectors, arrays).
 *
 *  Portability Issues:
 *      None.
 */

#include <terra/secutil/secure_string.h>
#include <terra/secutil/secure_vector.h>
#include <terra/secutil/secure_deque.h>
#include <terra/secutil/secure_array.h>
#include <terra/stf/stf.h>

using namespace Terra;

// Just ensure we can build and strings compare properly
STF_TEST(SecureTypes, SecureString)
{
    SecUtil::SecureString s = "Hello";
    SecUtil::SecureBasicString<char> q = "Hello";

    STF_ASSERT_EQ(s, q);
}

STF_TEST(SecureTypes, SecureVector)
{
    SecUtil::SecureVector<int> vec = {1, 2, 3};

    STF_ASSERT_EQ(3, vec.size());
}

STF_TEST(SecureTypes, SecureDeque)
{
    SecUtil::SecureDeque<int> deque = {1, 2, 3};

    STF_ASSERT_EQ(3, deque.size());
}

STF_TEST(SecureTypes, SecureArray)
{
    SecUtil::SecureArray<int, 3> array = {1, 2, 3};

    STF_ASSERT_EQ(3, array.size());
}
