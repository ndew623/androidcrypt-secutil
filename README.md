Security Utilities Library
==========================

This repository contains various security-related object and functions,
including the following:

* SecureErase(): a function that will ensure erasure of data
* SecureAllocator<>: an Allocator class that will securely erase memory when
  de-allocating memory
* SecureArray<>: derived from std::array, this array will securely erase any
  allocated memory
* SecureVector<>: derived from std::vector, this vector will securely erase
  any memory the vector allocates
* SecureString (including wide and UTF-8 forms): these string forms use the
  SecureAllocator to allocate memory, so memory allocated by the strings
  is ensured to be erased as it is freed
