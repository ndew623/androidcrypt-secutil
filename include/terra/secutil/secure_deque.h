/*
 *  secure_deque.h
 *
 *  Copyright (C) 2024
 *  Terrapane Corporation
 *  All Rights Reserved
 *
 *  Author:
 *      Paul E. Jones <paulej@packetizer.com>
 *
 *  Description:
 *      This file defines a SecureDeque type, which merely replaces the
 *      allocator used in the standard deque type with one that will ensure
 *      elements are securely erased.
 *
 *      IMPORTANT NOTE: Securely erasing the memory allocated by the deque
 *      will not erase memory allocated by objects in the deque. For example,
 *      if the deque is a deque of std::string, the memory allocated by the
 *      std::string will not be erased on destruction.
 *
 *  Portability Issues:
 *      None.
 */

#pragma once

#include <deque>
#include "secure_allocator.h"

namespace Terra::SecUtil
{

template<typename T>
using SecureDeque = std::deque<T, SecureAllocator<T>>;

} // namespace Terra::SecUtil
