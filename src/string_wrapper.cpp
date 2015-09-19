/*
 * string_wrapper.cpp
 *
 *  Created on: Sep 18, 2015
 *      Author: swgroup
 */

#include "string_wrapper.h"

#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <algorithm>

string_wrapper::string_wrapper(char* wrappedBuffer, size_t wrappedBufferSize)
   : buffer(wrappedBuffer),
     bufferSize(wrappedBufferSize),
     currentStringLength(strlen(buffer))
{
}

string_wrapper::string_wrapper(char* wrappedBuffer, size_t wrappedBufferSize, size_t initialLength)
   : buffer(wrappedBuffer),
     bufferSize(wrappedBufferSize),
     currentStringLength(initialLength)
{
   // assert(initialLength < bufferSize)
   buffer[initialLength] = '\0';
}

heap_string::heap_string(size_t n)
   : string_wrapper(new char[n], n, 0)
{
}

heap_string::heap_string(const heap_string& rhs)
   : string_wrapper(new char[rhs.capacity()], rhs.capacity(), rhs.size())
{
   memcpy(buffer, rhs.c_str(), rhs.capacity());
}

heap_string::~heap_string()
{
   delete[] buffer;
}

size_t string_wrapper::size() const
{
   return currentStringLength;
}

size_t string_wrapper::length() const
{
   return size();
}

size_t string_wrapper::capacity() const
{
   return bufferSize;
}

string_wrapper& string_wrapper::format_append(const char *fmt, ...)
{
   size_t formattedChars;
   va_list args;
   va_start(args, fmt);
   formattedChars = vsnprintf(&buffer[currentStringLength], (bufferSize - currentStringLength), fmt, args);
   va_end(args);

   // vsnprintf() returns the number of characters it WOULD HAVE formatted.
   // adjust currentStringLength according to what actually was written.
   currentStringLength = std::min((currentStringLength + formattedChars), (bufferSize - 1));

   return *this;
}

string_wrapper& string_wrapper::format(const char *fmt, ...)
{
   size_t formattedChars;
   va_list args;
   va_start(args, fmt);
   formattedChars = vsnprintf(buffer, bufferSize, fmt, args);
   va_end(args);

   // vsnprintf() returns the number of characters it WOULD HAVE formatted.
   // adjust currentStringLength according to what actually was written.
   currentStringLength = std::min(formattedChars, (bufferSize - 1));

   return *this;
}

string_wrapper& string_wrapper::assign(const char *s, size_t n)
{
   // avoid strncpy; it is slow.
   size_t copyLen = std::min(n, (bufferSize - 1));
   memcpy(buffer, s, copyLen);
   buffer[copyLen] = '\0';

   currentStringLength = copyLen;

   return *this;
}

string_wrapper& string_wrapper::assign(const char *s)
{
   return assign(s, strlen(s));
}

string_wrapper& string_wrapper::assign(const std::string& str)
{
   return assign(str.c_str(), str.length());
}

string_wrapper& string_wrapper::operator=(const char *s)
{
   return assign(s);
}

string_wrapper& string_wrapper::append(const char *s)
{
   // avoid strncat; it will force us to do a strlen of the whole result string.
   size_t sLen = strlen(s);
   size_t copyLen = std::min(sLen, (bufferSize - currentStringLength - 1));
   if (copyLen > 0) {
      memcpy(&buffer[currentStringLength], s, copyLen);
      currentStringLength += copyLen;
      buffer[currentStringLength] = '\0';
   }

   return *this;
}

string_wrapper& string_wrapper::operator+=(const char *s)
{
   return append(s);
}

void string_wrapper::clear()
{
   buffer[0] = '\0';
   currentStringLength = 0;
}

const char& string_wrapper::operator[](size_t pos) const
{
   return buffer[pos];
}

char& string_wrapper::operator[](size_t pos)
{
   return buffer[pos];
}
