#include "CppUTest/TestHarness.h"

#include "stack_string.h"
#include <string.h>

TEST_GROUP(StackStringTests)
{
   void setup()
   {
   }

   void teardown()
   {
   }
};

TEST(StackStringTests, StackStringCopyFromStringWrapper)
{
   char foo[256];
   string_wrapper s(foo);

   LONGS_EQUAL(12, s.append("foo").format_append(" %s %u", "bar", 5643).size());

   stack_string<8> copy(s);
   LONGS_EQUAL(8, copy.capacity());
   LONGS_EQUAL(7, copy.size());
   STRCMP_EQUAL("foo bar", copy.c_str());
}

TEST(StackStringTests, StackStringCopyFromLarger)
{
   stack_string<256> s;

   LONGS_EQUAL(12, s.append("foo").format_append(" %s %u", "bar", 5643).size());
   stack_string<8> copy(s);

   LONGS_EQUAL(8, copy.capacity());
   LONGS_EQUAL(7, copy.size());
   STRCMP_EQUAL("foo bar", copy.c_str());
}

TEST(StackStringTests, StackStringCopy)
{
   stack_string<256> s;

   LONGS_EQUAL(12, s.append("foo").format_append(" %s %u", "bar", 5643).size());
   stack_string<32> copy(s);

   LONGS_EQUAL(32, copy.capacity());
   LONGS_EQUAL(12, copy.size());
   STRCMP_EQUAL("foo bar 5643", copy.c_str());
}

TEST(StackStringTests, CreateStackBufferAutoSized)
{
   stack_string<256> s;

   LONGS_EQUAL(256, s.capacity());
   LONGS_EQUAL(0, s.size());
   LONGS_EQUAL(0, s.length());
   STRCMP_EQUAL("", s.c_str());

   LONGS_EQUAL(12, s.append("foo").format_append(" %s %u", "bar", 5643).size());
   STRCMP_EQUAL("foo bar 5643", s.c_str());
}
