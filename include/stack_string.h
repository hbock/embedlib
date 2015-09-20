/*
 * stack_string.h
 *
 *  Created on: Sep 20, 2015
 *      Author: hbock
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

private:
   char stackBuffer[allocatedLength];
};

#endif /* STACK_STRING_H_ */
