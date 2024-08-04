/*
 *  secure_erase.h
 *
 *  Copyright (C) 2024
 *  Terrapane Corporation
 *  All Rights Reserved
 *
 *  Author:
 *      Paul E. Jones <paulej@packetizer.com>
 *
 *  Description:
 *      This header file defines a function that will securely erase memory.
 *      These functions are intended to erase characters and numbers, as those
 *      are not constructed objects.  They may be used with more complex object,
 *      but care should be taken as one should not erase memory objects that
 *      contain pointers or have other objects that need data to exist for
 *      the destructor to execute properly.
 *
 *  Portability Issues:
 *      None.
 */

#pragma once

#include <cstddef>
#include <span>
#include <array>
#include <vector>
#include <string>
#include <type_traits>

namespace Terra::SecUtil
{

/*
 *  SecureErase()
 *
 *  Description:
 *      This function will securely erase memory.
 *
 *  Parameters:
 *      buffer [in]
 *          Pointer to buffer to erase.
 *
 *      length [in]
 *          Number of octets to set to zero.
 *
 *  Returns:
 *      Nothing.
 *
 *  Comments:
 *      None.
 */
void SecureErase(void *buffer, std::size_t length);

/*
 *  SecureErase()
 *
 *  Description:
 *      This function will securely erase the given span of values.
 *
 *  Parameters:
 *      values [in]
 *          A span of values of the type T.
 *
 *  Returns:
 *      Nothing.
 *
 *  Comments:
 *      Use this function with caution.  One should not erase memory held by
 *      objects that needs that memory to be valid when the destructor is
 *      called.  In general, this should only be used simple data types
 *      or plain old data objects (structures with basic types).
 */
template<typename T>
void SecureErase(std::span<T> values)
{
    SecureErase(values.data(), values.size() * sizeof(T));
}

/*
 *  SecureErase()
 *
 *  Description:
 *      This function will securely erase a string.
 *
 *  Parameters:
 *      value [in]
 *          The string to erase.
 *
 *  Returns:
 *      Nothing.
 *
 *  Comments:
 *      Use this function with caution.  One should not erase memory held by
 *      objects that needs that memory to be valid when the destructor is
 *      called.  In general, this should only be used simple data types
 *      or plain old data objects (structures with basic types).
 */
template<typename T>
void SecureErase(std::basic_string<T> &value)
{
    SecureErase(value.data(), value.length() * sizeof(T));
}

/*
 *  SecureErase()
 *
 *  Description:
 *      This function will securely erase the given array of values.
 *
 *  Parameters:
 *      array [in]
 *          An array of elements of the type T.
 *
 *  Returns:
 *      Nothing.
 *
 *  Comments:
 *      Use this function with caution.  One should not erase memory held by
 *      objects that needs that memory to be valid when the destructor is
 *      called.  In general, this should only be used simple data types
 *      or plain old data objects (structures with basic types).
 */
template<typename T, std::size_t N>
void SecureErase(std::array<T, N> &array)
{
    SecureErase(array.data(), array.size() * sizeof(T));
}

/*
 *  SecureErase()
 *
 *  Description:
 *      This function will securely erase the given vector of values.
 *
 *  Parameters:
 *      vector [in]
 *          A vector of elements of the type T.
 *
 *  Returns:
 *      Nothing.
 *
 *  Comments:
 *      Use this function with caution.  One should not erase memory held by
 *      objects that needs that memory to be valid when the destructor is
 *      called.  In general, this should only be used simple data types
 *      or plain old data objects (structures with basic types).
 */
template<typename T>
void SecureErase(std::vector<T> &vector)
{
    SecureErase(vector.data(), vector.size() * sizeof(T));
}

/*
 *  SecureErase()
 *
 *  Description:
 *      This function will securely memory for the specified variable.  This
 *      template function is intended for basic types like integers, floats,
 *      enums, pointers, etc.  For securely erasing blocks of memory, one should
 *      call the version that accepts a pointer and length.  Note, too, that
 *      erasing a pointer does not erase the data pointed to by that pointer.
 *
 *  Parameters:
 *      value [in]
 *          The value to securely erase.
 *
 *  Returns:
 *      Nothing.
 *
 *  Comments:
 *      None.
 */
template<typename T,
         typename std::enable_if<std::is_arithmetic<T>::value ||
                                 std::is_enum<T>::value ||
                                 std::is_pointer<T>::value, bool>::type = true>
void SecureErase(T &value)
{
    SecureErase(&value, sizeof(T));
}

} // namespace Terra::SecUtil
