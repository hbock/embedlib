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

string_wrapper& string_wrapper::assign(const string_wrapper& str)
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

int string_wrapper::compare(const char* s) const
{
   size_t comparingSize = strlen(s);
   int result = strncmp(buffer, s, std::min(comparingSize, currentStringLength));

   if (0 == result) {
      if (currentStringLength < comparingSize) {
         result = -1;
      } else if (currentStringLength > comparingSize){
         result = 1;
      }
   }

   return result;
}

int string_wrapper::compare(const string_wrapper& str) const
{
   size_t comparingSize = str.size();
   int result = strncmp(buffer, str.buffer, std::min(comparingSize, currentStringLength));

   if (0 == result) {
      if (currentStringLength < comparingSize) {
         result = -1;
      } else if (currentStringLength > comparingSize){
         result = 1;
      }
   }

   return result;
}

size_t string_wrapper::find(char c, size_t pos) const
{
   size_t find_pos = npos;
   if (pos < currentStringLength) {
      char* needle = strchr(&buffer[pos], c);
      if (NULL != needle) {
         find_pos = (needle - buffer);
      }
   }

   return find_pos;
}

size_t string_wrapper::find(const char* s, size_t pos) const
{
   size_t find_pos = npos;
   if (pos < currentStringLength) {
      char* needle = strstr(&buffer[pos], s);
      if (NULL != needle) {
         find_pos = (needle - buffer);
      }
   }

   return find_pos;
}

size_t string_wrapper::find(const string_wrapper& str, size_t pos) const
{
   return find(str.c_str(), pos);
}

size_t string_wrapper::find(const std::string& str, size_t pos) const
{
   return find(str.c_str(), pos);
}

size_t string_wrapper::rfind(char c, size_t pos) const
{
   size_t find_pos = npos;
   if (pos >= currentStringLength) {
      pos = (currentStringLength - 1);
   }

   size_t p = pos;
   do {
      if (c == buffer[p]) {
         find_pos = p;
         break;
      }
   } while(p--);

   return find_pos;
}

// common rfind() implementation.
// FUTURE: Boyer-Moore would be great here if we are very large;
// but it's a costly implementation for not much benefit in small strings.
static size_t rfind(const char* haystack, size_t haystackLen,
                    const char* needle, size_t needleLen,
                    size_t endPos)
{
   size_t find_pos = string_wrapper::npos;

   if (endPos >= haystackLen) {
      endPos = (haystackLen - 1);
   }

   // don't even consider searching for needles larger than our
   // own string starting at pos, which will never match.
   if (needleLen <= (endPos + 1)) {
      // start at earliest possible match...
      size_t hsIndex = endPos - needleLen + 1;

      // move backwards, trying to find a match.
      do {
         if (0 == memcmp(&haystack[hsIndex], needle, needleLen)) {
            find_pos = hsIndex;
            break;
         }
      } while (hsIndex--);
   }

   return find_pos;
}

size_t string_wrapper::rfind(const char* s, size_t pos) const
{
   return ::rfind(buffer, currentStringLength,
                  s, strlen(s), pos);
}

size_t string_wrapper::rfind(const string_wrapper& str, size_t pos) const
{
   return ::rfind(buffer, currentStringLength,
                  str.buffer, str.currentStringLength, pos);
}

size_t string_wrapper::rfind(const std::string& str, size_t pos) const
{
   return ::rfind(buffer, currentStringLength,
                  str.c_str(), str.length(), pos);
}

bool string_wrapper::empty() const
{
   return 0 == currentStringLength;
}

void string_wrapper::push_back(char c)
{
   if (currentStringLength < (bufferSize - 1)) {
      buffer[currentStringLength] = c;
      buffer[currentStringLength + 1] = '\0';
      currentStringLength++;
   }
}

size_t string_wrapper::copy(char* s, size_t len, size_t pos) const
{
   size_t copyLen = 0;

   if (pos < currentStringLength) {
      copyLen = std::min(len, (currentStringLength - pos));
      memcpy(s, &buffer[pos], copyLen);
   }

   return copyLen;
}

size_t string_wrapper::find_first_of(char c, size_t pos) const
{
   return find(c, pos);
}

size_t string_wrapper::find_first_of(const char* s, size_t pos) const
{
   size_t find_pos = npos;

   for (size_t i = pos; (npos == find_pos) && (i < currentStringLength); i++) {
      for (size_t j = 0; s[j]; j++) {
         if (s[j] == buffer[i]) {
            find_pos = i;
            break;
         }
      }
   }

   return find_pos;
}

size_t string_wrapper::find_first_of(const string_wrapper& s, size_t pos) const
{
   return find_first_of(s.c_str(), pos);
}

size_t string_wrapper::find_first_of(const char* s, size_t pos, size_t n) const
{
   size_t find_pos = npos;

   for (size_t i = pos; (npos == find_pos) && (i < currentStringLength); i++) {
      for (size_t j = 0; j < n; j++) {
         if (s[j] == buffer[i]) {
            find_pos = i;
            break;
         }
      }
   }

   return find_pos;
}

// free function operators

bool operator==(const string_wrapper& lhs, const char* rhs)
{
   return (0 == lhs.compare(rhs));
}

bool operator==(const char* lhs, const string_wrapper& rhs)
{
   return (0 == rhs.compare(lhs));
}

bool operator==(const string_wrapper& lhs, const string_wrapper& rhs)
{
   return (0 == lhs.compare(rhs));
}

bool operator!=(const string_wrapper& lhs, const char* rhs)
{
   return (0 != lhs.compare(rhs));
}

bool operator!=(const char* lhs, const string_wrapper& rhs)
{
   return (0 != rhs.compare(lhs));
}

bool operator!=(const string_wrapper& lhs, const string_wrapper& rhs)
{
   return (0 != lhs.compare(rhs));
}
