#include "CppUTest/TestHarness.h"
#include "string_wrapper.h"

TEST_GROUP(StringWrapperTests)
{
   void setup()
   {
   }

   void teardown()
   {
   }
};

TEST(StringWrapperTests, op_array)
{
   char buf[10] = "FOO";
   string_wrapper s(buf, sizeof(buf));

   LONGS_EQUAL('F', s[0]);
   LONGS_EQUAL('O', s[1]);
   LONGS_EQUAL('O', s[2]);
   LONGS_EQUAL('\0', s[3]);

   s[0] = 'D';

   LONGS_EQUAL('D', s[0]);
   LONGS_EQUAL('O', s[1]);
   LONGS_EQUAL('O', s[2]);
   LONGS_EQUAL('\0', s[3]);
}

TEST(StringWrapperTests, op_array_const)
{
   char buf[10] = "FOO";
   const string_wrapper s(buf, sizeof(buf));

   LONGS_EQUAL('F', s[0]);
   LONGS_EQUAL('O', s[1]);
   LONGS_EQUAL('O', s[2]);
   LONGS_EQUAL('\0', s[3]);
}


TEST(StringWrapperTests, clear)
{
   char buf[10] = "FOO";
   string_wrapper s(buf, sizeof(buf));

   s.clear();

   LONGS_EQUAL(0, s.length());
   STRCMP_EQUAL("", s.c_str());
}

TEST(StringWrapperTests, op_plus_equals_c_str)
{
   char buf[10] = "FOO";
   string_wrapper s(buf, sizeof(buf));

   LONGS_EQUAL(6, (s += "BAR").length());
   STRCMP_EQUAL("FOOBAR", s.c_str());

   LONGS_EQUAL(9, (s += "HAM SALAD").length());
   STRCMP_EQUAL("FOOBARHAM", s.c_str());

   // truncate
   LONGS_EQUAL(9, (s += "SANDWICH OF HAM SALAD").length());
   STRCMP_EQUAL("FOOBARHAM", s.c_str());
}

TEST(StringWrapperTests, append_c_str)
{
   char buf[10] = "FOO";
   string_wrapper s(buf, sizeof(buf));

   LONGS_EQUAL(6, s.append("BAR").length());
   STRCMP_EQUAL("FOOBAR", s.c_str());

   LONGS_EQUAL(9, s.append("HAM SALAD").length());
   STRCMP_EQUAL("FOOBARHAM", s.c_str());

   // truncate
   LONGS_EQUAL(9, s.append("SANDWICH OF HAM SALAD").length());
   STRCMP_EQUAL("FOOBARHAM", s.c_str());
}


TEST(StringWrapperTests, assign_std_str)
{
   char buf[10] = "FOO";
   string_wrapper s(buf, sizeof(buf));

   LONGS_EQUAL(3, s.assign(std::string("BAR")).length());
   STRCMP_EQUAL("BAR", s.c_str());

   LONGS_EQUAL(9, s.assign(std::string("HAM SALAD")).length());
   STRCMP_EQUAL("HAM SALAD", s.c_str());

   // truncate
   LONGS_EQUAL(9, s.assign(std::string("SANDWICH OF HAM SALAD")).length());
   STRCMP_EQUAL("SANDWICH ", s.c_str());
}

TEST(StringWrapperTests, op_eq_c_str)
{
   char buf[10] = "FOO";
   string_wrapper s(buf, sizeof(buf));

   LONGS_EQUAL(3, (s = "BAR").length());
   STRCMP_EQUAL("BAR", s.c_str());

   LONGS_EQUAL(9, (s = "HAM SALAD").length());
   STRCMP_EQUAL("HAM SALAD", s.c_str());

   // truncate
   LONGS_EQUAL(9, (s = "SANDWICH OF HAM SALAD").length());
   STRCMP_EQUAL("SANDWICH ", s.c_str());
}

TEST(StringWrapperTests, assign_buffer)
{
   char buf[10] = "FOO";
   string_wrapper s(buf, sizeof(buf));

   LONGS_EQUAL(3, s.assign("BAR", 3).length());
   STRCMP_EQUAL("BAR", s.c_str());

   LONGS_EQUAL(6, s.assign("HAM SALAD", 6).length());
   STRCMP_EQUAL("HAM SA", s.c_str());

   // truncate
   LONGS_EQUAL (9, s.assign("SANDWICH OF HAM SALAD", 10).length());
   STRCMP_EQUAL("SANDWICH ", s.c_str());
}

TEST(StringWrapperTests, assign_c_str)
{
   char buf[10] = "FOO";
   string_wrapper s(buf, sizeof(buf));

   LONGS_EQUAL(3, s.assign("BAR").length());
   STRCMP_EQUAL("BAR", s.c_str());

   LONGS_EQUAL(9, s.assign("HAM SALAD").length());
   STRCMP_EQUAL("HAM SALAD", s.c_str());

   // truncate
   LONGS_EQUAL(9, s.assign("SANDWICH OF HAM SALAD").length());
   STRCMP_EQUAL("SANDWICH ", s.c_str());
}

TEST(StringWrapperTests, format_append_truncate)
{
   char buf[10] = "FOO";
   string_wrapper s(buf, sizeof(buf));

   LONGS_EQUAL(9, s.format_append("foo %s baz, ham %u salad ", "bar", 5u).length());
   STRCMP_EQUAL("FOOfoo ba", s.c_str());
}

TEST(StringWrapperTests, format_append)
{
   char buf[256] = "FOO";
   string_wrapper s(buf, sizeof(buf));

   LONGS_EQUAL(28, s.format_append("foo %s baz, ham %u salad ", "bar", 5u).length());
   STRCMP_EQUAL("FOOfoo bar baz, ham 5 salad ", s.c_str());
}

TEST(StringWrapperTests, format_truncate)
{
   char buf[10] = "FOO";
   string_wrapper s(buf, sizeof(buf));

   LONGS_EQUAL(9, s.format("foo %s baz, ham %u salad ", "bar", 5u).length());
   STRCMP_EQUAL("foo bar b", s.c_str());
}

TEST(StringWrapperTests, format)
{
   char buf[256] = "FOO";
   string_wrapper s(buf, sizeof(buf));

   LONGS_EQUAL(25, s.format("foo %s baz, ham %u salad ", "bar", 5u).length());
   STRCMP_EQUAL("foo bar baz, ham 5 salad ", s.c_str());
}

TEST(StringWrapperTests, CopyHeapBuffer)
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

TEST(StringWrapperTests, CreateHeapBuffer)
{
   heap_string s(256);

   LONGS_EQUAL(256, s.capacity());
   LONGS_EQUAL(0, s.size());
   LONGS_EQUAL(0, s.length());
   STRCMP_EQUAL("", s.c_str());

   LONGS_EQUAL(12, s.append("foo").format_append(" %s %u", "bar", 5643).size());
   STRCMP_EQUAL("foo bar 5643", s.c_str());
}


TEST(StringWrapperTests, CreateStackBufferAutoSized)
{
   stack_string<256> s;

   LONGS_EQUAL(256, s.capacity());
   LONGS_EQUAL(0, s.size());
   LONGS_EQUAL(0, s.length());
   STRCMP_EQUAL("", s.c_str());

   LONGS_EQUAL(12, s.append("foo").format_append(" %s %u", "bar", 5643).size());
   STRCMP_EQUAL("foo bar 5643", s.c_str());
}

TEST(StringWrapperTests, CreateAutoSized)
{
   char buf[256] = "";
   string_wrapper s(buf);

   LONGS_EQUAL(sizeof(buf), s.capacity());
   LONGS_EQUAL(0, s.size());
   LONGS_EQUAL(0, s.length());
   STRCMP_EQUAL("", s.c_str());
}

TEST(StringWrapperTests, CreateEmpty)
{
   char buf[256] = "";
   string_wrapper s(buf, sizeof(buf));

   LONGS_EQUAL(sizeof(buf), s.capacity());
   LONGS_EQUAL(0, s.size());
   LONGS_EQUAL(0, s.length());
   STRCMP_EQUAL("", s.c_str());
}

TEST(StringWrapperTests, CreateSpecifySize)
{
   char buf[256] = "FOOBARBAZ";
   string_wrapper s(buf, sizeof(buf), 6);

   LONGS_EQUAL(sizeof(buf), s.capacity());
   LONGS_EQUAL(6, s.size());
   LONGS_EQUAL(6, s.length());
   STRCMP_EQUAL("FOOBAR", s.c_str());
}

TEST(StringWrapperTests, Create)
{
   char buf[256] = "FOO";
   string_wrapper s(buf, sizeof(buf));

   LONGS_EQUAL(sizeof(buf), s.capacity());
   LONGS_EQUAL(3, s.size());
   LONGS_EQUAL(3, s.length());
   STRCMP_EQUAL("FOO", s.c_str());
}

