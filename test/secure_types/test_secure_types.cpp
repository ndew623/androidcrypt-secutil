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

#include <string>
#include <terra/secutil/secure_string.h>
#include <terra/secutil/secure_vector.h>
#include <terra/secutil/secure_deque.h>
#include <terra/secutil/secure_array.h>
#include <terra/stf/stf.h>

using namespace Terra;

// Simple object for testing
class SomeObject
{
    public:
        SomeObject() {};
        SomeObject(const char *text) : data{text} {}
        SomeObject(std::string &text) : data{text} {}
        ~SomeObject() = default;
        void AssignData(std::string &text) { data = text; }
        std::string GetData() { return data; }

    protected:
        std::string data;
};

// Just ensure we can build and strings compare properly
STF_TEST(SecureTypes, SecureString)
{
    SecUtil::SecureString s = "Hello";
    SecUtil::SecureBasicString<char> q = "Hello";

    STF_ASSERT_EQ(s, q);
}

STF_TEST(SecureTypes, SecureVector1)
{
    SecUtil::SecureVector<int> vec = {1, 2, 3};

    STF_ASSERT_EQ(3, vec.size());
}

STF_TEST(SecureTypes, SecureVector2)
{
    SecUtil::SecureVector<SomeObject> vec;

    vec.emplace_back("String 1");
    vec.emplace_back("String 2");
    vec.emplace_back("String 3");
    vec.emplace_back("String 4");

    STF_ASSERT_EQ(std::string("String 3"), vec[2].GetData());

    STF_ASSERT_EQ(4, vec.size());
}

STF_TEST(SecureTypes, SecureVector3)
{
    SecUtil::SecureVector<int> vec;

    vec.emplace_back(1);
    vec.emplace_back(2);
    vec.emplace_back(3);
    vec.emplace_back(4);
    vec.emplace_back(5);

    STF_ASSERT_EQ(3, vec[2]);

    STF_ASSERT_EQ(5, vec.size());
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
