/*
 *  secure_deleter.h
 *
 *  Copyright (C) 2025
 *  Terrapane Corporation
 *  All Rights Reserved
 *
 *  Author:
 *      Paul E. Jones <paulej@packetizer.com>
 *
 *  Description:
 *      This file defines a SecureArrayDeleter and SecureObjectDeleter object
 *      that may be used in place of the std::default_delete<T> type used when
 *      creating std::unique_ptr<T> or std::shared_ptr<T> types for arrays or
 *      objects.  The SecureArrayDeleter and SecureObjectDeleter objects
 *      will call the SecureErase() function to ensure arrays or objects are
 *      securely erased when deleted.
 *
 *      When using a deleter, it's incumbent on the caller to ensure that the
 *      appropriate deleter type is used.  This is particular important when
 *      creating a unique_ptr.  Both the unique_ptr type indicates the
 *      deleter and the deleter object must be provided.
 *
 *      Note that when deleting an object, the SecureObjectDeleter only
 *      securely erases member variable storage and not data allocated by
 *      the object.  Thus, if the object contains things like vectors, it
 *      might be preferable to use SecureVector. SecureObjectDeleter would
 *      be useful in securely erasing simple data types, of course.
 *
 *      To simplify the use of SecureArrayDeleter, several helper functions are
 *      also defined. Example usage:
 *          auto foo = MakeUniqueSecureArray<char>(10);
 *          auto foo = MakeSharedSecureArray<char>(10);
 *          auto foo = MakeUniqueSecureObject<Object>(param, param);
 *          auto foo = MakeSharedSecureObject<Object>(param, param);
 *
 *  Portability Issues:
 *      None.
 */

#pragma once

#include <cstddef>
#include <memory>
#include "secure_erase.h"

namespace Terra::SecUtil
{

// Class to act as a deleter for arrays
template<typename T>
struct SecureArrayDeleter
{
    // Default constructor
    SecureArrayDeleter() : size{0} {}

    // Simple constructor to store the size of an array to delete
    SecureArrayDeleter(std::size_t size) : size{size} {}

    ~SecureArrayDeleter() = default;

    // Invoked to delete allocated memory
    void operator()(T *array) const noexcept
    {
        // Securely erase memory
        SecureErase(reinterpret_cast<void *>(array), size * sizeof(T));

        // Deallocate the array
        delete[] array;
    }

    std::size_t size;
};

// Class to act as a deleter for objects
template<typename T>
struct SecureObjectDeleter
{
    // Default constructor and destructor is sufficient
    SecureObjectDeleter() = default;
    ~SecureObjectDeleter() = default;

    // Invoked to delete allocated memory
    void operator()(T *object) const noexcept
    {
        // Securely erase memory
        SecureErase(reinterpret_cast<void *>(object), sizeof(T));

        // Deallocate the array
        delete object;
    }

    std::size_t size;
};

/*
 *  MakeUniqueSecureArray()
 *
 *  Description:
 *      This is a helper function to return a std::unique_ptr to an array
 *      of elements of type T that utilizes the SecureArrayDeleter<T> object
 *      to ensure the array elements are securely erased.
 *
 *  Parameters:
 *      size [in]
 *          Size of the array of objects to allocate.
 *
 *  Returns:
 *      A std::unique_ptr to the array of elements of type T.
 *
 *  Comments:
 *      None.
 */
template<typename T>
std::unique_ptr<T[], SecureArrayDeleter<T>> MakeUniqueSecureArray(
                                                            std::size_t size)
{
    return std::unique_ptr<T[], SecureArrayDeleter<T>>(
        new T[size],
        SecureArrayDeleter<T>{size});
}

/*
 *  MakeSharedSecureArray()
 *
 *  Description:
 *      This is a helper function to return a std::shared_ptr to an array
 *      of elements of type T that utilizes the SecureArrayDeleter<T> object
 *      to ensure the array elements are securely erased.
 *
 *  Parameters:
 *      size [in]
 *          Size of the array of objects to allocate.
 *
 *  Returns:
 *      A std::shared_ptr to the array of elements of type T.
 *
 *  Comments:
 *      None.
 */
template<typename T>
std::shared_ptr<T[]> MakeSharedSecureArray(size_t size)
{
    return std::shared_ptr<T[]>(new T[size], SecureArrayDeleter<T>{size});
}

/*
 *  MakeUniqueSecureObject()
 *
 *  Description:
 *      This is a helper function to return a std::unique_ptr to an object
 *      of type T that utilizes the SecureObjectDeleter<T> object
 *      to ensure the object is securely erased.
 *
 *  Parameters:
 *      ...args [in]
 *          Parameter pack (0 or more parameters) that are forwarded to the
 *          constructor of the object being constructed.
 *
 *  Returns:
 *      A std::unique_ptr to the object of type T.
 *
 *  Comments:
 *      None.
 */
template<typename T, typename... Args>
std::unique_ptr<T, SecureObjectDeleter<T>> MakeUniqueSecureObject(
                                                                Args &&...args)
{
    return std::unique_ptr<T, SecureObjectDeleter<T>>(
        new T(std::forward<Args>(args)...),
        SecureObjectDeleter<T>());
}

/*
 *  MakeSharedSecureObject()
 *
 *  Description:
 *      This is a helper function to return a std::shared_ptr to an object
 *      of type T that utilizes the SecureObjectDeleter<T> object
 *      to ensure the object is securely erased.
 *
 *  Parameters:
 *      ...args [in]
 *          Parameter pack (0 or more parameters) that are forwarded to the
 *          constructor of the object being constructed.
 *
 *  Returns:
 *      A std::shared_ptr to the object of type T.
 *
 *  Comments:
 *      None.
 */
template<typename T, typename... Args>
std::shared_ptr<T> MakeSharedSecureObject(Args &&...args)
{
    return std::shared_ptr<T>(new T(std::forward<Args>(args)...),
                              SecureObjectDeleter<T>());
}

} // namespace Terra::SecUtil
