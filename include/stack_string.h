/* This is free and unencumbered software released into the public domain.
 * Please see COPYING for more details.
 */
#ifndef STACK_STRING_H_
#define STACK_STRING_H_

#include "string_wrapper.h"

// string_wrapper, stack allocated
template<size_t allocatedLength>
class stack_string : public string_wrapper
{
public:
   stack_string()
      : string_wrapper(stackBuffer, allocatedLength, 0)
   {
   }

   // copy constructor, from arbitrary-sized other stack_string
   template<size_t otherAllocatedLength>
   stack_string(const stack_string<otherAllocatedLength>& other)
      : string_wrapper(stackBuffer, allocatedLength, 0)
   {
      assign(other.c_str(), other.length());
   }

   // copy constructor, from string_wrapper
   stack_string(const string_wrapper& other)
      : string_wrapper(stackBuffer, allocatedLength, 0)
   {
      assign(other.c_str(), other.length());
   }

private:
   char stackBuffer[allocatedLength];
};

#endif /* STACK_STRING_H_ */
