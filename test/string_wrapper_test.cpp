#include "CppUTest/TestHarness.h"

#include "string_wrapper.h"
#include "stack_string.h"
#include "heap_string.h"
#include <string.h>

TEST_GROUP(StringWrapperTests)
{
   void setup()
   {
   }

   void teardown()
   {
   }
};

TEST(StringWrapperTests, copy)
{
   char buf[10];
   string_wrapper s(buf);
   s.assign("foobarbaz");

   char testbuf[10];
   testbuf[9] = 'A'; // junk
   LONGS_EQUAL(9, s.copy(testbuf, 10));
   // don't append terminating null.
   LONGS_EQUAL('A', testbuf[9]);
   LONGS_EQUAL(0, memcmp("foobarbaz", testbuf, 9));

   s.assign("5643");
   LONGS_EQUAL(4, s.copy(testbuf, 10));
   LONGS_EQUAL(0, memcmp("5643", testbuf, 4));

   testbuf[1] = 'A';
   LONGS_EQUAL(1, s.copy(testbuf, 10, 3));
   LONGS_EQUAL(0, memcmp("3A", testbuf, 2));

   // pos overflow; no copy
   LONGS_EQUAL(0, s.copy(testbuf, 10, 90));
   LONGS_EQUAL(0, memcmp("3A", testbuf, 2));
}

TEST(StringWrapperTests, push_back)
{
   char buf[6];
   string_wrapper s(buf);

   s.push_back('F');
   LONGS_EQUAL(1, s.size());
   STRCMP_EQUAL("F", s.c_str());

   s.push_back('L');
   s.push_back('O');
   s.push_back('O');
   s.push_back('F');
   LONGS_EQUAL(5, s.size());
   STRCMP_EQUAL("FLOOF", s.c_str());

   // overrun attempt
   s.push_back('L');
   s.push_back('O');
   s.push_back('O');
   s.push_back('F');
   LONGS_EQUAL(5, s.size());
   STRCMP_EQUAL("FLOOF", s.c_str());
}

TEST(StringWrapperTests, empty)
{
   char buf[10];
   string_wrapper s(buf);

   CHECK(true == s.empty());
   s = "foo";
   CHECK(false == s.empty());
   s.clear();
   CHECK(true == s.empty());
   s.append("a");
   CHECK(false == s.empty());
   s = "";
   CHECK(true == s.empty());
}

TEST(StringWrapperTests, rfind_std_str)
{
   char buf[10];
   char buf2[10];
      string_wrapper s(buf);
   std::string t(buf2);
   s.assign("floof");

   LONGS_EQUAL(4, s.rfind((t = "f")));
   LONGS_EQUAL(0, s.rfind((t = "fl")));
   LONGS_EQUAL(1, s.rfind((t = "l")));
   LONGS_EQUAL(3, s.rfind((t = "o")));
   LONGS_EQUAL(2, s.rfind((t = "o"), 2));
   LONGS_EQUAL(0, s.rfind((t = "f"), 3));
   LONGS_EQUAL(2, s.rfind((t = "oo"), 3));
   LONGS_EQUAL(string_wrapper::npos, s.rfind((t = "of"), 3));
   LONGS_EQUAL(string_wrapper::npos, s.rfind((t = "floop")));
   LONGS_EQUAL(string_wrapper::npos, s.rfind((t = "loof"), 0));

   // pos overrun
   LONGS_EQUAL(4, s.rfind((t = "f"), 5));
   LONGS_EQUAL(0, s.rfind((t = "fl"), 50));
   LONGS_EQUAL(0, s.rfind((t = "floof"), 500));
}

TEST(StringWrapperTests, rfind_str)
{
   char buf[10];
   char buf2[10];
   string_wrapper s(buf);
   string_wrapper t(buf2);
   s.assign("floof");

   LONGS_EQUAL(4, s.rfind((t = "f")));
   LONGS_EQUAL(0, s.rfind((t = "fl")));
   LONGS_EQUAL(1, s.rfind((t = "l")));
   LONGS_EQUAL(3, s.rfind((t = "o")));
   LONGS_EQUAL(2, s.rfind((t = "o"), 2));
   LONGS_EQUAL(0, s.rfind((t = "f"), 3));
   LONGS_EQUAL(2, s.rfind((t = "oo"), 3));
   LONGS_EQUAL(string_wrapper::npos, s.rfind((t = "of"), 3));
   LONGS_EQUAL(string_wrapper::npos, s.rfind((t = "floop")));
   LONGS_EQUAL(string_wrapper::npos, s.rfind((t = "loof"), 0));

   // pos overrun
   LONGS_EQUAL(4, s.rfind((t = "f"), 5));
   LONGS_EQUAL(0, s.rfind((t = "fl"), 50));
   LONGS_EQUAL(0, s.rfind((t = "floof"), 500));
}

TEST(StringWrapperTests, rfind_c_str)
{
   char buf[10];
   string_wrapper s(buf);
   s.assign("floof");

   LONGS_EQUAL(4, s.rfind("f"));
   LONGS_EQUAL(0, s.rfind("fl"));
   LONGS_EQUAL(1, s.rfind("l"));
   LONGS_EQUAL(3, s.rfind("o"));
   LONGS_EQUAL(2, s.rfind("o", 2));
   LONGS_EQUAL(0, s.rfind("f", 3));
   LONGS_EQUAL(2, s.rfind("oo", 3));
   LONGS_EQUAL(string_wrapper::npos, s.rfind("of", 3));
   LONGS_EQUAL(string_wrapper::npos, s.rfind("floop"));
   LONGS_EQUAL(string_wrapper::npos, s.rfind("loof", 0));

   // pos overrun
   LONGS_EQUAL(4, s.rfind("f", 5));
   LONGS_EQUAL(0, s.rfind("fl", 50));
   LONGS_EQUAL(0, s.rfind("floof", 500));
}

TEST(StringWrapperTests, rfind_char)
{
   char buf[10];
   string_wrapper s(buf);
   s.assign("floof");

   LONGS_EQUAL(4, s.rfind('f'));
   LONGS_EQUAL(1, s.rfind('l'));
   LONGS_EQUAL(2, s.rfind('o', 2));
   LONGS_EQUAL(0, s.rfind('f', 3));
   LONGS_EQUAL(string_wrapper::npos, s.rfind('k', 1));

   // overrun
   LONGS_EQUAL(4, s.rfind('f', 5));
   LONGS_EQUAL(4, s.rfind('f', 50));
   LONGS_EQUAL(4, s.rfind('f', 500));
}

TEST(StringWrapperTests, find_std_str)
{
   char buf[10], buf2[20];
   string_wrapper s(buf);
   std::string t(buf2);
   s.assign("floof");

   LONGS_EQUAL(0, s.find((t = "f")));
   LONGS_EQUAL(0, s.find((t = "fl")));
   LONGS_EQUAL(1, s.find((t = "l")));
   LONGS_EQUAL(3, s.find((t = "o"), 3));
   LONGS_EQUAL(3, s.find((t = "of"), 0));
   LONGS_EQUAL(4, s.find((t = "f"), 1));

   LONGS_EQUAL(string_wrapper::npos, s.find((t = "fl"), 1));
   LONGS_EQUAL(string_wrapper::npos, s.find((t = "floop")));
   LONGS_EQUAL(string_wrapper::npos, s.find((t = "oo"), 3));
   LONGS_EQUAL(string_wrapper::npos, s.find((t = "k"), 1));

   // pos greater than s.size()
   LONGS_EQUAL(string_wrapper::npos, s.find((t = "f"), 5));
}

TEST(StringWrapperTests, find_str)
{
   char buf[10], buf2[20];
   string_wrapper s(buf);
   string_wrapper t(buf2);
   s.assign("floof");

   t = "f";
   LONGS_EQUAL(0, s.find(t));
   t = "fl";
   LONGS_EQUAL(0, s.find(t));
   t = "l";
   LONGS_EQUAL(1, s.find(t));
   t = "o";
   LONGS_EQUAL(3, s.find(t, 3));
   t = "of";
   LONGS_EQUAL(3, s.find(t, 0));
   t = "f";
   LONGS_EQUAL(4, s.find(t, 1));

   t = "fl";
   LONGS_EQUAL(string_wrapper::npos, s.find(t, 1));
   t = "floop";
   LONGS_EQUAL(string_wrapper::npos, s.find(t));
   t = "oo";
   LONGS_EQUAL(string_wrapper::npos, s.find(t, 3));
   t = "k";
   LONGS_EQUAL(string_wrapper::npos, s.find(t, 1));

   // overrun
   LONGS_EQUAL(string_wrapper::npos, s.find("f", 5));
}

TEST(StringWrapperTests, find_c_str)
{
   char buf[10];
   string_wrapper s(buf);
   s.assign("floof");

   LONGS_EQUAL(0, s.find("f"));
   LONGS_EQUAL(0, s.find("fl"));
   LONGS_EQUAL(1, s.find("l"));
   LONGS_EQUAL(3, s.find("o", 3));
   LONGS_EQUAL(3, s.find("of", 0));
   LONGS_EQUAL(4, s.find("f", 1));
   LONGS_EQUAL(string_wrapper::npos, s.find("fl", 1));
   LONGS_EQUAL(string_wrapper::npos, s.find("floop"));
   LONGS_EQUAL(string_wrapper::npos, s.find("oo", 3));
   LONGS_EQUAL(string_wrapper::npos, s.find("k", 1));

   // overrun
   LONGS_EQUAL(string_wrapper::npos, s.find("f", 5));
   LONGS_EQUAL(string_wrapper::npos, s.find("fl", 50));
   LONGS_EQUAL(string_wrapper::npos, s.find("floof", 500));
}

TEST(StringWrapperTests, find_char)
{
   char buf[10];
   string_wrapper s(buf);
   s.assign("floof");

   LONGS_EQUAL(0, s.find('f'));
   LONGS_EQUAL(1, s.find('l'));
   LONGS_EQUAL(3, s.find('o', 3));
   LONGS_EQUAL(4, s.find('f', 1));
   LONGS_EQUAL(string_wrapper::npos, s.find('k', 1));

   // overrun
   LONGS_EQUAL(string_wrapper::npos, s.find('f', 5));
   LONGS_EQUAL(string_wrapper::npos, s.find('f', 50));
   LONGS_EQUAL(string_wrapper::npos, s.find('f', 500));
}

TEST(StringWrapperTests, op_neq_str_str)
{
   char buf[10];
   char buf2[20];
   string_wrapper s(buf);
   string_wrapper t(buf2);

   s = "FOO";
   t = "BAR";

   CHECK(true == (t != s));
   CHECK(true == (s != t));

   t = "FOO";
   CHECK(false == (t != s));
   CHECK(false == (s != t));

   t = "FOOO";
   CHECK(true == (t != s));
   CHECK(true == (s != t));

   s.clear();
   CHECK(true == (t != s));
   t.clear();
   CHECK(false == (t != s));

   // larger other buffer
   t = "FOOBARFOOBARFOOBAR";
   CHECK(true == (t != s));
}

TEST(StringWrapperTests, op_neq_c_str_str)
{
    char buf[10] = "FOO";
    string_wrapper s(buf, sizeof(buf));

    CHECK(true == ("BAR" != s));
    CHECK(true == ("FAR" != s));
    CHECK(true == ("FOA" != s));
    CHECK(true == ("GOO" != s));
    CHECK(true == ("FPP" != s));
    CHECK(true == ("FOP" != s));
    CHECK(true == ("GOOOOOOO" != s));
    CHECK(true == ("G" != s));
    CHECK(true == ("" != s));

    CHECK(false == ("FOO" != s));

    s.clear();
    CHECK(false ==  ("" != s));
    CHECK(true == ("F" != s));
    CHECK(true == ("FO" != s));
    CHECK(true == ("FOO" != s));
}

TEST(StringWrapperTests, op_neq_str_c_str)
{
    char buf[10] = "FOO";
    string_wrapper s(buf, sizeof(buf));

    CHECK(true == (s != "BAR"));
    CHECK(true == (s != "FAR"));
    CHECK(true == (s != "FOA"));
    CHECK(true == (s != "GOO"));
    CHECK(true == (s != "FPP"));
    CHECK(true == (s != "FOP"));
    CHECK(true == (s != "GOOOOOOO"));
    CHECK(true == (s != "G"));
    CHECK(true == (s != ""));

    CHECK(false == (s != "FOO"));

    s.clear();
    CHECK(false == (s != ""));
    CHECK(true == (s != "F"));
    CHECK(true == (s != "FO"));
    CHECK(true == (s != "FOO"));
}

/////
TEST(StringWrapperTests, op_eq_eq_str_str)
{
    char buf[10];
    char buf2[20];
    string_wrapper s(buf);
    string_wrapper t(buf2);

    s = "FOO";
    t = "BAR";

    CHECK(false == (t == s));
    CHECK(false == (s == t));

    t = "FOO";
    CHECK(true == (t == s));
    CHECK(true == (s == t));

    t = "FOOO";
    CHECK(false == (t == s));
    CHECK(false == (s == t));

    s.clear();
    CHECK(false == (t == s));
    t.clear();
    CHECK(true == (t == s));

    // larger other buffer
    t = "FOOBARFOOBARFOOBAR";
    CHECK(false == (t == s));
}

TEST(StringWrapperTests, op_eq_eq_c_str_str)
{
    char buf[10] = "FOO";
    string_wrapper s(buf, sizeof(buf));

    CHECK(false == ("BAR" == s));
    CHECK(false == ("FAR" == s));
    CHECK(false == ("FOA" == s));
    CHECK(false == ("GOO" == s));
    CHECK(false == ("FPP" == s));
    CHECK(false == ("FOP" == s));
    CHECK(false == ("GOOOOOOO" == s));
    CHECK(false == ("G" == s));
    CHECK(false == ("" == s));

    CHECK(true == ("FOO" == s));

    s.clear();
    CHECK(true ==  ("" == s));
    CHECK(false == ("F" == s));
    CHECK(false == ("FO" == s));
    CHECK(false == ("FOO" == s));
}

TEST(StringWrapperTests, op_eq_eq_str_c_str)
{
    char buf[10] = "FOO";
    string_wrapper s(buf, sizeof(buf));

    CHECK(false == (s == "BAR"));
    CHECK(false == (s == "FAR"));
    CHECK(false == (s == "FOA"));
    CHECK(false == (s == "GOO"));
    CHECK(false == (s == "FPP"));
    CHECK(false == (s == "FOP"));
    CHECK(false == (s == "GOOOOOOO"));
    CHECK(false == (s == "G"));
    CHECK(false == (s == ""));

    CHECK(true == (s == "FOO"));

    s.clear();
    CHECK(true == (s == ""));
    CHECK(false == (s == "F"));
    CHECK(false == (s == "FO"));
    CHECK(false == (s == "FOO"));
}

TEST(StringWrapperTests, compare_str)
{
    char buf[10] = "FOO";
    char buf2[10];
    string_wrapper s(buf, sizeof(buf));
    string_wrapper s2(buf2);

    // Value of the first character that does not match in compared string is greater than comparing.
    s2 = "BAR";
    CHECK(s.compare(s2) > 0);
    s2 = "FAR";
    CHECK(s.compare(s2) > 0);
    s2 = "FOA";
    CHECK(s.compare(s2) > 0);
    // Value of the first character that does not match in compared string is less than comparing.
    s2 = "GOO";
    CHECK(s.compare(s2) < 0);
    s2 = "FPP";
    CHECK(s.compare(s2) < 0);
    s2 = "FOP";
    CHECK(s.compare(s2) < 0);

    s2 = "FOO";
    CHECK(s.compare(s2) == 0);

    // all characters match, but compared string is shorter.
    s2 = "FOOO";
    CHECK(s.compare(s2) < 0);
    // all characters match, but compared string is longer.
    s2 = "FO";
    CHECK(s.compare(s2) > 0);
}

TEST(StringWrapperTests, compare_c_str)
{
    char buf[10] = "FOO";
    string_wrapper s(buf, sizeof(buf));

    // Value of the first character that does not match in compared string is greater than comparing.
    CHECK(s.compare("BAR") > 0);
    CHECK(s.compare("FAR") > 0);
    CHECK(s.compare("FOA") > 0);
    // Value of the first character that does not match in compared string is less than comparing.
    CHECK(s.compare("GOO") < 0);
    CHECK(s.compare("FPP") < 0);
    CHECK(s.compare("FOP") < 0);

    CHECK(s.compare("FOO") == 0);

    // all characters match, but compared string is shorter.
    CHECK(s.compare("FOOO") < 0);
    // all characters match, but compared string is longer.
    CHECK(s.compare("FO") > 0);
}

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

TEST(StringWrapperTests, assign_str)
{
   char buf[10] = "FOO";
   char buf2[20];
   string_wrapper s(buf, sizeof(buf));
   string_wrapper t(buf2);

   LONGS_EQUAL(3, s.assign((t = "BAR")).length());
   STRCMP_EQUAL("BAR", s.c_str());

   LONGS_EQUAL(9, s.assign((t = "HAM SALAD")).length());
   STRCMP_EQUAL("HAM SALAD", s.c_str());

   // truncate
   LONGS_EQUAL(9, s.assign((t = "SANDWICH OF HAM SALAD")).length());
   STRCMP_EQUAL("SANDWICH ", s.c_str());
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
   char buf[10];
   // initialize with 'garbage'
   buf[0] = 'f';
   buf[1] = 'u';
   buf[2] = 'u';
   // constructor should clear it.
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

