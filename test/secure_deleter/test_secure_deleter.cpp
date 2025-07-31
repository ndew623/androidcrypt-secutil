/*
 *  test_secure_array.cpp
 *
 *  Copyright (C) 2024, 2025
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

#include <cstddef>
#include <memory>
#include <terra/secutil/secure_deleter.h>
#include <terra/stf/stf.h>

namespace
{

unsigned delete_called = 0;
unsigned test_oject_allocations = 0;
unsigned test_oject_deallocations = 0;

void InitializeTest()
{
    delete_called = 0;
    test_oject_allocations = 0;
    test_oject_deallocations = 0;
}

struct TestObject
{
    TestObject()
    {
        test_oject_allocations++;
    }

    ~TestObject()
    {
        test_oject_deallocations++;
    }

    int value;
};

template<typename T>
struct TestSecureArrayDeleter : public Terra::SecUtil::SecureArrayDeleter<T>
{
    // Default constructor
    TestSecureArrayDeleter() : Terra::SecUtil::SecureArrayDeleter<T>() {}

    // Simple constructor to store the size of an array to delete
    TestSecureArrayDeleter(std::size_t size) : Terra::SecUtil::SecureArrayDeleter<T>(size) {}

    ~TestSecureArrayDeleter() = default;

    // Invoked to delete allocated memory
    void operator()(T *array) const noexcept
    {
        Terra::SecUtil::SecureArrayDeleter<T>::operator()(array);

        delete_called++;
    }
};

template<typename T>
struct TestSecureObjectDeleter : public Terra::SecUtil::SecureObjectDeleter<T>
{
    // Default constructor and destructor is sufficient
    TestSecureObjectDeleter() = default;
    ~TestSecureObjectDeleter() = default;

    // Invoked to delete allocated memory
    void operator()(T *object) const noexcept
    {
        Terra::SecUtil::SecureObjectDeleter<T>::operator()(object);

        delete_called++;
    }
};

// Test using test object so we can verify memory release
STF_TEST(SecureDeleter, DeleteArray1)
{
    InitializeTest();

    // Verify precondition
    STF_ASSERT_EQ(0, delete_called);

    {
        std::unique_ptr<wchar_t[], TestSecureArrayDeleter<wchar_t>> foo(
            new wchar_t[100],
            TestSecureArrayDeleter<wchar_t>{100});

        STF_ASSERT_NE(nullptr, foo.get());
    }

    // Verify precondition
    STF_ASSERT_EQ(1, delete_called);
}

// Test using test object so we can verify memory release
STF_TEST(SecureDeleter, DeleteArray2)
{
    std::unique_ptr<wchar_t[], TestSecureArrayDeleter<wchar_t>> foo;
    InitializeTest();

    // Verify precondition
    STF_ASSERT_EQ(0, delete_called);

    // Create the unique pointer; important to include that second argument!
    foo = std::unique_ptr<wchar_t[], TestSecureArrayDeleter<wchar_t>>(
        new wchar_t[100],
        TestSecureArrayDeleter<wchar_t>{100});

    STF_ASSERT_NE(nullptr, foo.get());

    foo.release();

    // Verify precondition
    STF_ASSERT_EQ(0, delete_called);
}

// Test using test object so we can verify memory release
STF_TEST(SecureDeleter, DeleteArray3)
{
    InitializeTest();

    // Verify precondition
    STF_ASSERT_EQ(0, delete_called);

    {
        std::shared_ptr<wchar_t[]> foo =
            std::shared_ptr<wchar_t[]>(new wchar_t[100],
                                       TestSecureArrayDeleter<wchar_t>{100});

        STF_ASSERT_NE(nullptr, foo.get());
    }

    // Verify precondition
    STF_ASSERT_EQ(1, delete_called);
}

// Test using helper function (clean code, but cannot check deleter logic)
STF_TEST(SecureDeleter, DeleteArray4)
{
    auto foo = Terra::SecUtil::MakeUniqueSecureArray<wchar_t>(100);

    STF_ASSERT_NE(nullptr, foo.get());

    foo.release();

    STF_ASSERT_EQ(nullptr, foo.get());
}

// Test using helper function (clean code, but cannot check deleter logic)
STF_TEST(SecureDeleter, DeleteArray5)
{
    std::shared_ptr<wchar_t[]> foo =
        Terra::SecUtil::MakeSharedSecureArray<wchar_t>(100);

    STF_ASSERT_NE(nullptr, foo.get());

    foo.reset();

    STF_ASSERT_EQ(nullptr, foo.get());
}

// Test using test object so we can verify memory release
STF_TEST(SecureDeleter, DeleteArray6)
{
    InitializeTest();

    // Verify precondition
    STF_ASSERT_EQ(0, delete_called);
    STF_ASSERT_EQ(0, test_oject_allocations);
    STF_ASSERT_EQ(0, test_oject_deallocations);

    {
        std::shared_ptr<TestObject[]> foo =
            std::shared_ptr<TestObject[]>(new TestObject[10],
                                       TestSecureArrayDeleter<TestObject>{10});

        STF_ASSERT_NE(nullptr, foo.get());
    }

    // Verify precondition
    STF_ASSERT_EQ(1, delete_called);
    STF_ASSERT_EQ(10, test_oject_allocations);
    STF_ASSERT_EQ(10, test_oject_deallocations);
}

STF_TEST(SecureDeleter, DeleteObject1)
{
    InitializeTest();

    // Verify precondition
    STF_ASSERT_EQ(0, delete_called);
    STF_ASSERT_EQ(0, test_oject_allocations);
    STF_ASSERT_EQ(0, test_oject_deallocations);

    {
        std::unique_ptr<TestObject, TestSecureObjectDeleter<TestObject>> foo(
            new TestObject,
            TestSecureObjectDeleter<TestObject>{});

        STF_ASSERT_NE(nullptr, foo.get());
    }

    // Verify precondition
    STF_ASSERT_EQ(1, delete_called);
    STF_ASSERT_EQ(1, test_oject_allocations);
    STF_ASSERT_EQ(1, test_oject_deallocations);
}

STF_TEST(SecureDeleter, DeleteObject2)
{
    std::shared_ptr<TestObject> foo;

    InitializeTest();

    // Verify precondition
    STF_ASSERT_EQ(0, delete_called);
    STF_ASSERT_EQ(0, test_oject_allocations);
    STF_ASSERT_EQ(0, test_oject_deallocations);

    foo = std::shared_ptr<TestObject>(new TestObject,
                                      TestSecureObjectDeleter<TestObject>{});

    STF_ASSERT_NE(nullptr, foo.get());

    foo.reset();

    STF_ASSERT_EQ(nullptr, foo.get());

    // Verify precondition
    STF_ASSERT_EQ(1, delete_called);
    STF_ASSERT_EQ(1, test_oject_allocations);
    STF_ASSERT_EQ(1, test_oject_deallocations);
}

// Test using helper function (clean code, but cannot check deleter logic)
STF_TEST(SecureDeleter, DeleteObject3)
{
    InitializeTest();

    // Verify precondition
    STF_ASSERT_EQ(0, delete_called);
    STF_ASSERT_EQ(0, test_oject_allocations);
    STF_ASSERT_EQ(0, test_oject_deallocations);

    {
        std::unique_ptr<TestObject,
                        Terra::SecUtil::SecureObjectDeleter<TestObject>>
            foo = Terra::SecUtil::MakeUniqueSecureObject<TestObject>();

        STF_ASSERT_NE(nullptr, foo.get());
    }

    // Verify precondition
    STF_ASSERT_EQ(0, delete_called); // Cannot verify
    STF_ASSERT_EQ(1, test_oject_allocations);
    STF_ASSERT_EQ(1, test_oject_deallocations);
}

// Test using helper function (clean code, but cannot check deleter logic)
STF_TEST(SecureDeleter, DeleteObject4)
{
    std::shared_ptr<TestObject> foo;

    InitializeTest();

    // Verify precondition
    STF_ASSERT_EQ(0, delete_called);
    STF_ASSERT_EQ(0, test_oject_allocations);
    STF_ASSERT_EQ(0, test_oject_deallocations);

    foo = Terra::SecUtil::MakeSharedSecureObject<TestObject>();

    STF_ASSERT_NE(nullptr, foo.get());

    foo.reset();

    STF_ASSERT_EQ(nullptr, foo.get());

    // Verify precondition
    STF_ASSERT_EQ(0, delete_called); // Cannot verify
    STF_ASSERT_EQ(1, test_oject_allocations);
    STF_ASSERT_EQ(1, test_oject_deallocations);
}

}
