# `embedlib` - Small C++ Utilities for Embedded Devices 


`embedlib` is a small library of C++ classes and utilities
that are designed to balance efficient data structures
and algorithms with the needs of embedded systems. Costs
such as memory usage and allocation, code size, inlineability,
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

In addition to supporting the API of `std::string`, there are interesting
new methods, including `string_wrapper::format()` and `string_wrapper::format_append()`
to allow string formatting (a la `printf`) without using an intermediate buffer.

#### Usage (`string_wrapper`)

`string_wrapper` allows you to wrap a pre-allocated buffer as a string.  This buffer
can be provided from the stack, as a global variable, or allocated on the heap.

```c++
#include "string_wrapper.h"
#include <stdio.h>

int main(int argc, char** argv)
{
	char buffer[100];
	string_wrapper output(buffer, sizeof(buffer));
    output.append("Pre-allocated!");
    // outputs "New string: Pre-allocated!\n"
	printf("New string: %s\n", output.c_str());
    
    // formatting!
    // outputs "Pre-allocated! You provided X arguments to this program.",
    // where X is the value of argc.
    output.format_append(" You provided %d arguments to this program.", argc);
    fputs(output.c_str(), stdout);
    
    return 0;
}
```

An alternate templated constructor allows you to infer the size of the wrapped
buffer, if it was allocated globally or statically and the declaration is available:

```c++
#include "string_wrapper.h"
#include <stdio.h>

int main(int argc, char** argv)
{
	char buffer[100];
    // note we don't need the size.
	string_wrapper output(buffer);
    output.append("Pre-allocated!");
    // outputs "New string: Pre-allocated!\n"
	printf("New string: %s\n", output.c_str());
    
    return 0;
}
```

#### Usage (`stack_string`)

`stack_string` allows you to allocate a string directly on the stack by 
providing the length of the desired allocation as a template argument.

```c++
#include "stack_string.h"
#include <stdio.h>

int main(int argc, char** argv)
{
	stack_string<100> output;
    output.append("Pre-allocated!");
    // outputs "New string: Pre-allocated!\n"
	printf("New string: %s\n", output.c_str());
    
    // formatting!
    // outputs "Pre-allocated! You provided X arguments to this program.",
    // where X is the value of argc.
    output.format_append(" You provided %d arguments to this program.", argc);
    fputs(output.c_str(), stdout);
    
    return 0;
}
```
