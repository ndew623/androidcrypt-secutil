/*
 *  secure_erase.cpp
 *
 *  Copyright (C) 2024, 2025
 *  Terrapane Corporation
 *  All Rights Reserved
 *
 *  Author:
 *      Paul E. Jones <paulej@packetizer.com>
 *
 *  Description:
 *      This module contains code that will securely erase memory.
 *
 *  Portability Issues:
 *      None.
 */

#if defined(_WIN32)
#include <Windows.h>
#else
#include <cstring>
#endif
#include <terra/secutil/secure_erase.h>

namespace Terra::SecUtil
{

#if !defined(_WIN32) && !defined(HAVE_EXPLICIT_BZERO) && !defined(HAVE_MEMSET_S)
// Assign the volatile function pointer memset_secure to call std::memset.
// The compiler should not optimize a call to a volatile function pointer,
// so this function should not be compiled out during optimization.
// See: https://www.usenix.org/system/files/conference/usenixsecurity17/sec17-yang.pdf
static void *(*volatile memset_secure_func)(void *, int, size_t) = memset;
#endif

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
 *      A pointer to the erased buffer.
 *
 *  Comments:
 *      None
 */
void SecureErase(void *buffer, std::size_t length)
{
    // Nothing to do if pointer is null or if the length is zero
    if ((buffer == nullptr) || (length == 0)) return;

#if defined(_WIN32)
    SecureZeroMemory(buffer, length);
#elif defined(HAVE_MEMSET_S)
    memset_s(buffer, length, 0, length);
#elif defined(HAVE_EXPLICIT_BZERO)
    explicit_bzero(buffer, length);
#else
    memset_secure_func(buffer, 0, length);
#endif
}

} // namespace Terra::SecUtil
