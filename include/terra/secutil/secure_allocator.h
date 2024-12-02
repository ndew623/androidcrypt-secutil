/*
 *  secure_allocator.h
 *
 *  Copyright (C) 2024
 *  Terrapane Corporation
 *  All Rights Reserved
 *
 *  Author:
 *      Paul E. Jones <paulej@packetizer.com>
 *
 *  Description:
 *      This object defines an allocator that will securely erase memory
 *      as it frees memory.  It is intended for use with STL containers and
 *      such that take an allocator as an argument.  For example, the following
 *      defines a SecureVector of integers:
 *
 *          using SecureVector = std::vector<int, SecureAllocator<int>>;
 *
 *  Portability Issues:
 *      None.
 */

#pragma once

#include <cstdlib>
#include <cstddef>
#include <limits>
#include <new>
#include "secure_erase.h"

namespace Terra::SecUtil
{

template<typename T>
struct SecureAllocator
{
    // Required type specification
    using value_type = T;

    // Default constructor
    constexpr SecureAllocator() = default;

    // Trivial copy constructor
    template<typename U>
    constexpr SecureAllocator(const SecureAllocator<U> &) noexcept
    {
    }

    // Default destructor
    constexpr ~SecureAllocator() = default;

    /*
     *  SecureAllocator::allocate()
     *
     *  Description:
     *      Allocates the specified number of type T items.
     *
     *  Parameters:
     *      n [in]
     *          Number of items of type T for which memory should be allocated.
     *
     *  Returns:
     *      A pointer to the allocated memory.
     *
     *  Comments:
     *      This function will throw an exception on failure.
     */
    [[nodiscard]] constexpr T *allocate(std::size_t n) const
    {
        // If the request is too large, throw an exception
        if (n > std::numeric_limits<std::size_t>::max() / sizeof(T))
        {
            throw std::bad_array_new_length();
        }

        // Attempt to allocate the requested memory (exception on failure)
        return static_cast<T *>(::operator new(sizeof(T) * n));
    }

    /*
     *  SecureAllocator::deallocate()
     *
     *  Description:
     *      Free memory previously allocated by allocate().
     *
     *  Parameters:
     *      p [in]
     *          A pointer to the memory to be freed.
     *
     *      n [in]
     *          The number of items of type T that were previously allocated.
     *
     *  Returns:
     *      Nothing.
     *
     *  Comments:
     *      None.
     */
    constexpr void deallocate(T *p, std::size_t n) const noexcept
    {
        // If the pointer is nullptr, just return
        if (p == nullptr) return;

        // Securely erase the allocated memory before deletion
        if (n > 0) SecureErase(p, sizeof(T) * n);

        // Delete the previously allocated memory
        ::operator delete(p);
    }

    /*
     *  SecureAllocator::construct()
     *
     *  Description:
     *      This function will construct an object in place at the address p.
     *
     *  Parameters:
     *      p [in]
     *          That address where an object of type U is to be constructed.
     *
     *      args [in]
     *          The variadic template arguments to be forwarded to the objects
     *          constructor.
     *
     *  Returns:
     *      Nothing.
     *
     *  Comments:
     *      None.
     */
    template<typename U, typename... Args>
    void construct(U *p, Args &&...args)
    {
        // Perform a placement new at the address given by p
        ::new ((void *) p) U(std::forward<Args>(args)...);
    }

    /*
     *  SecureAllocator::destroy()
     *
     *  Description:
     *      Destroys an object pointed to by p.
     *
     *  Parameters:
     *      None.
     *
     *  Returns:
     *      Nothing.
     *
     *  Comments:
     *      None.
     */
    void destroy(T *p)
    {
        p->~T();
    }

    /*
     *  SecureAllocator::operator==()
     *
     *  Description:
     *      Checks to see if memory allocated by one allocator can be freed by
     *      another allocator.
     *
     *  Parameters:
     *      other [in]
     *          A reference to the other allocator object.
     *
     *  Returns:
     *      Always returns true.
     *
     *  Comments:
     *      None.
     */
    template<typename U>
    constexpr bool operator==(const SecureAllocator<U> &) const noexcept
    {
        return true;
    }
};

} // namespace Terra::SecUtil
