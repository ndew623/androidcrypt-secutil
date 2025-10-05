/*
 *  secure_array.h
 *
 *  Copyright (C) 2024, 2025
 *  Terrapane Corporation
 *  All Rights Reserved
 *
 *  Author:
 *      Paul E. Jones <paulej@packetizer.com>
 *
 *  Description:
 *      This file defines a SecureArray type, which ensures memory associated
 *      with the array is erased when the SecureArray object is destructed.
 *      Since the SecureArray object destructs before the base class and its
 *      data elements and since some objects may need access to valid data
 *      during the destruction process, SecureArray is restricted to trivial
 *      types are safe to be erased (integers, floats, pointers, trivial
 *      structs, etc.).  The intended use is to hold an array of integers or
 *      characters for security-related functions like encryption,
 *      authentication, password generation, and the like.
 *
 *  Portability Issues:
 *      None.
 */

#pragma once

#include <array>
#include <stdexcept>
#include <algorithm>
#include <type_traits>
#include "secure_erase.h"

namespace Terra::SecUtil
{

template<typename T, std::size_t N>
    requires std::is_trivial_v<T>
class SecureArray : public std::array<T, N>
{
    public:
        using std::array<T, N>::array;
        SecureArray(std::initializer_list<T> list)
        {
            // Ensure the initializer list is not too large
            if (list.size() > N)
            {
                throw std::invalid_argument("Initializer list is too large");
            }

            // Assign the first list.size() elements
            std::copy(list.begin(), list.end(), this->begin());

            // Value-initialize the remaining elements
            std::fill(this->begin() + list.size(), this->end(), T{});
        }
        virtual ~SecureArray()
        {
            SecureErase(std::array<T, N>::data(),
                        std::array<T, N>::size() * sizeof(T));
        }
};

} // namespace Terra::SecUtil

