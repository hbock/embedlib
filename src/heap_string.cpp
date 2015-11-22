/* This is free and unencumbered software released into the public domain.
 * Please see COPYING for more details.
 */
#include "heap_string.h"

#include <cstring>

heap_string::heap_string(size_t n)
   : string_wrapper(new char[n], n, 0)
{
}

heap_string::heap_string(const heap_string& rhs)
   : string_wrapper(new char[rhs.capacity()], rhs.capacity(), rhs.size())
{
   std::memcpy(buffer, rhs.c_str(), rhs.capacity());
}

heap_string::~heap_string()
{
   delete[] buffer;
}
