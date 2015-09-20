# `embedlib` - Small C++ Utilities for Embedded Machines 


`embedlib` is a small library of C++ classes and utilities
that are designed to balance efficient data structures
and algorithms with the needs of embedded systems. Costs
such as inlining, memory usage and allocation, code size, 
and others must be taken into consideration, and the weight given
to each is different for every embedded system.

## License
`embedlib` is released into the public domain.   It comes with no warranty.  Enjoy!

## Building `embedlib`
`embedlib` uses CMake as its build system, and requires version 
2.6 or higher.

## Utilities

### `string_wrapper`

`string_wrapper` is a string container designed to be as close to 
source-compatible with `std::string` as possible, without allowing
for hidden heap allocation, growth, or implicit copying.  
It does this by "wrapping" arrays or pre-allocated C-style
(null-terminated) strings.  It does not yet support multibyte
encoding.

The library provides helper subclasses of `string_wrapper` for
allocating the required memory in one step.  `stack_string` is a 
`string_wrapper` that automatically allocates its storage on the
stack (unless the class is created via `new`!), allowing you to 
skip the pre-allocation step.

`heap_string` is similar to `stack_string`, but as its name implies,
allocates memory on the heap.  Like its parent and sibling classes,
however, it does not allow for implicit growth.
