/*
 *  secure_vector.h
 *
 *  Copyright (C) 2024
 *  Terrapane Corporation
 *  All Rights Reserved
 *
 *  Author:
 *      Paul E. Jones <paulej@packetizer.com>
 *
 *  Description:
 *      This file defines a SecureVector type, which merely replaces the
 *      allocator used in the standard vector type with one that will ensure
 *      elements are securely erased.
 *
 *      IMPORTANT NOTE: Securely erasing the memory allocated by the vector
 *      will not erase memory allocated by objects in the vector. For example,
 *      if the vector is a vector of std::string, the memory allocated by the
 *      std::string will not be erased on destruction.
 *
 *  Portability Issues:
 *      None.
 */

#pragma once

#include <vector>
#include "secure_allocator.h"

namespace Terra::SecUtil
{

template<typename T>
using SecureVector = std::vector<T, SecureAllocator<T>>;

} // namespace Terra::SecUtil
