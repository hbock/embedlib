#include "CppUTest/TestHarness.h"

#include "heap_string.h"
#include <string.h>

TEST_GROUP(HeapStringTests)
{
   void setup()
   {
   }

   void teardown()
   {
   }
};

TEST(HeapStringTests, CopyHeapBuffer)
{
   heap_string s(256);

   LONGS_EQUAL(256, s.capacity());
   LONGS_EQUAL(0, s.size());
   LONGS_EQUAL(0, s.length());
   STRCMP_EQUAL("", s.c_str());

   LONGS_EQUAL(12, s.append("foo").format_append(" %s %u", "bar", 5643).size());
   STRCMP_EQUAL("foo bar 5643", s.c_str());

   {
      heap_string s2(s);
      LONGS_EQUAL(12, s2.size());
      STRCMP_EQUAL("foo bar 5643", s2.c_str());
   }
}

TEST(HeapStringTests, CreateHeapBuffer)
{
   heap_string s(256);

   LONGS_EQUAL(256, s.capacity());
   LONGS_EQUAL(0, s.size());
   LONGS_EQUAL(0, s.length());
   STRCMP_EQUAL("", s.c_str());

   LONGS_EQUAL(12, s.append("foo").format_append(" %s %u", "bar", 5643).size());
   STRCMP_EQUAL("foo bar 5643", s.c_str());
}
