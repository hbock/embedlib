#ifndef STRING_WRAPPER_H_
#define STRING_WRAPPER_H_

#include <stddef.h>
#include <string>

/** \class string_wrapper
 * \brief Class to wrap an existing C-string, buffer, or array in an API
 *        that is source-compatible with std::string.
 */
class string_wrapper
{
public:
   /** \brief Wrap pre-existing buffer \a wrappedBuffer of size \a wrappedBufferSize.
    * \note Initial string length is automatically determined by calling
    *       strlen() on \a wrappedBuffer.  Calling this constructor on an uninitialized
    *       array has undefined behavior.
    */
   string_wrapper(char* wrappedBuffer, size_t wrappedBufferSize);

   /** \brief Wrap pre-existing buffer \a wrappedBuffer of capacity \a wrappedBufferSize,
    *         setting initial string length to \a initialLength.
    * \note Behavior is undefined if \a initialLength is greater than or
    *       equal to \a wrappedBufferSize.
    */
   string_wrapper(char* wrappedBuffer, size_t wrappedBufferSize, size_t initialLength);

   /** \brief Wrap pre-existing array \a wrappedBuffer.
    * \note This constructor auto-sizes the array.  This must be called with a stack-allocated
    *       array of size known at compile time.  The string length is initialized to 0.
    */
   template<size_t n>
   string_wrapper(char (&wrappedBuffer)[n])
      : buffer(wrappedBuffer),
        bufferSize(n),
        currentStringLength(0)
   {
   }

   // retrieval
   const char& operator[](size_t pos) const;
   char& operator[](size_t pos);

   // comparison
   int compare(const char* other) const;
   int compare(const std::string& other) const;
   int compare(const string_wrapper& other) const;

   void clear();

   // assignment
   string_wrapper& assign(const char* s);              // c-str
   string_wrapper& assign(const char* s, size_t n);    // buffer
   string_wrapper& assign(const std::string& str);     // std::string
   string_wrapper& assign(const string_wrapper& str);  // string_wrapper

   string_wrapper& operator=(const char* s);
   string_wrapper& operator=(const std::string& str);
   string_wrapper& operator=(const string_wrapper& str);

   // append
   string_wrapper& append(const char* s);
   string_wrapper& append(const std::string& str);
   string_wrapper& append(const string_wrapper& str);

   string_wrapper& operator+=(const char* s);
   string_wrapper& operator+=(const std::string& str);
   string_wrapper& operator+=(const string_wrapper& str);

   string_wrapper& format(const char* fmt, ...)
      // 'this' is argument 1...
      __attribute__((format (printf, 2, 3)));

   string_wrapper& format_append(const char* fmt, ...)
      // 'this' is argument 1...
      __attribute__((format (printf, 2, 3)));

   static const size_t npos = static_cast<size_t>(-1);

   size_t size() const;
   size_t length() const;
   size_t capacity() const;

   const char* c_str() const { return buffer; }

protected:
   char* buffer;
   const size_t bufferSize;
   size_t currentStringLength;

private:
   // we are not copyable!
   string_wrapper(const string_wrapper& rhs);
};

// relational operators
bool operator==(const string_wrapper& lhs, const char* rhs);
bool operator==(const char* lhs, const string_wrapper& rhs);
bool operator==(const string_wrapper& lhs, const string_wrapper& rhs);

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

// string_wrapper, heap allocated
class heap_string : public string_wrapper
{
public:
   heap_string(size_t n);
   heap_string(const heap_string& rhs);
   ~heap_string();
};


#endif /* STRING_WRAPPER_H_ */
