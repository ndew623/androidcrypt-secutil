/*
 *  secure_string.h
 *
 *  Copyright (C) 2024
 *  Terrapane Corporation
 *  All Rights Reserved
 *
 *  Author:
 *      Paul E. Jones <paulej@packetizer.com>
 *
 *  Description:
 *      This file defines a SecureString type, which merely replaces the
 *      allocator used in the standard string type with one that will ensure
 *      elements are securely erased.
 *
 *  Portability Issues:
 *      None.
 */

#pragma once

#include <string>
#include "secure_allocator.h"

namespace Terra::SecUtil
{

template<typename CharT, typename Traits = std::char_traits<CharT>>
using SecureBasicString = std::basic_string<CharT,
                                            Traits,
                                            SecureAllocator<CharT>>;

using SecureString = SecureBasicString<char>;
using SecureWString = SecureBasicString<wchar_t>;
using SecureU8String = SecureBasicString<char8_t>;

} // namespace Terra::SecUtil
