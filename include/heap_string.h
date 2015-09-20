/*
 * heap_string.h
 *
 *  Created on: Sep 20, 2015
 *      Author: hbock
 */

#ifndef HEAP_STRING_H_
#define HEAP_STRING_H_

#include "string_wrapper.h"

/** \brief Heap-allocated version of string_wrapper. */
class heap_string : public string_wrapper
{
public:
   /** \brief Allocate a new empty string on the heap, with capacity \a n. */
   heap_string(size_t n);
   /** \brief Copy an existing heap_string. */
   heap_string(const heap_string& rhs);
   ~heap_string();
};

#endif /* HEAP_STRING_H_ */
