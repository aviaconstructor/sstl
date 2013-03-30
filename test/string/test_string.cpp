#include <gtest/gtest.h>

//#define TEST_NATIVE_STL

#if defined(SSTL_TEST_NATIVE_STL)
    #include <string>
    #include <iterator>
    #include <sstl/sstl_common.h>

    using namespace std;
#else
    #include <sstl/string>
    #include <sstl/iterator>
    #include <sstl/_impl/string.cpp>

    using namespace SSTL_NAMESPACE;
#endif

TEST(test_string, types)
{
    SSTL_STATIC_ASSERT(typeid(char) == typeid(string::value_type), "value type is bad");

    ASSERT_EQ(0, numeric_limits<string::size_type>::min()); // unsigned
    ASSERT_LE(numeric_limits<unsigned>::max(), numeric_limits<string::size_type>::max());
    ASSERT_GE(numeric_limits<int>::min(), numeric_limits<string::difference_type>::min()); // signed

    SSTL_STATIC_ASSERT(typeid(char&) == typeid(string::reference), "reference type uis bad");
    SSTL_STATIC_ASSERT(typeid(const char&) == typeid(string::const_reference), "const reference type is bad");
    SSTL_STATIC_ASSERT(typeid(char*) == typeid(string::pointer), "pointer type uis bad");
    SSTL_STATIC_ASSERT(typeid(const char*) == typeid(string::const_pointer), "const pointer type is bad");

#if !defined(SSTL_TEST_NATIVE_STL)
    ASSERT_EQ(32, sizeof(string::_buffer_type));
#endif
}

TEST(test_string, constructors)
{
    string s1 = "AAAAAAAAAAAAAAAA";
    string s2("AAAAAAAAAAAAAAAA");
    string s3("AAAAAAAAAAAAAAAA", 16);
    string s4(16, 'A');
    string s5(s1);

    ASSERT_EQ(s1, s2);
    ASSERT_EQ(s1, s3);
    ASSERT_EQ(s1, s4);
    ASSERT_EQ(s1, s5);
}

TEST(test_string, assign)
{
    string s1 = "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA";
    string s2;
    string s3;
    string s4;
    string s5;
    string s6;

    s2 = s1;
    s3.assign(s2);
    s4.assign(s1.c_str());
    s5.assign(s4.data(), s4.size());
    s6.assign(s4.size(), 'A');

    ASSERT_EQ(s1, s2);
    ASSERT_EQ(s1, s3);
    ASSERT_EQ(s1, s4);
    ASSERT_EQ(s1, s5);
    ASSERT_EQ(s1, s6);
}

TEST(test_string, clear)
{
    string s = "01234567";
    s.clear();
    ASSERT_EQ(0, s.size());
    ASSERT_EQ(0, s.length()); // alias to size
    ASSERT_LE(0, s.capacity()); // still the buffer is allocated

    string s1 = "1";
    string s2 = s1;
    s1.clear();
    ASSERT_EQ(0, s1.size());
    ASSERT_EQ(0, s1.length()); // alias to size
    ASSERT_EQ(1, s2.size()); // the other buffer stays unchanged
    ASSERT_EQ(1, s2.length()); // alias to size
}

static void _check_string_indexing(string& s, string::size_type size)
{
    ASSERT_EQ(size, s.size());
    for (int i = 0; i < size; ++i)
    {
        char c = (i & 0x7) + '0';  // by convention
        ASSERT_EQ(c, s.at(i));
        ASSERT_EQ(c, s[i]);
    }
}

TEST(test_string, indexing)
{
    string s = "01234567";
    _check_string_indexing(s, 8);

    string ss = s;
    _check_string_indexing(s, 8);
    _check_string_indexing(ss, 8);

    ss += s;
    _check_string_indexing(s, 8);
    _check_string_indexing(ss, 16);
}

static void _check_string_iterators(string s, string::size_type size, char front, char back)
{
    ASSERT_EQ(s.size(), size);

    ASSERT_EQ(&(*s.begin()), &s[0]);
    ASSERT_EQ(*s.begin(), front);

    ASSERT_EQ(&(*s.end()), &s[0] + size);

    ASSERT_EQ(&(*s.rbegin()), &s[size - 1]);
    ASSERT_EQ(*s.rbegin(), back);

    ASSERT_EQ(&(*s.rend()), &s[0] - 1);

#if SSTL_CXX11
    ASSERT_EQ(&(*s.cbegin()), &s[0]);
    ASSERT_EQ(*s.cbegin(), front);

    ASSERT_EQ(&(*s.cend()), &s[0] + size);

    ASSERT_EQ(&(*s.crbegin()), &s[size - 1]);
    ASSERT_EQ(*s.crbegin(), back);

    ASSERT_EQ(&(*s.crend()), &s[0] - 1);

    ASSERT_EQ(s.front(), front);
    ASSERT_EQ(s.front(), s[0]);
    ASSERT_EQ(&s.front(), s.begin());

    ASSERT_EQ(s.back(), back);
    ASSERT_EQ(s.back(), s[size - 1]);
#endif
}

TEST(test_string, iterators)
{
    _check_string_iterators(string("1"), 1, '1', '1');
    _check_string_iterators(string("12"), 2, '1', '2');
    _check_string_iterators(string("0123456789"), 10, '0', '9');
}

static void _check_string_size(string s, string::size_type size)
{
    ASSERT_EQ(s.empty(), (s.size() == 0));
    ASSERT_EQ(size, s.size());
    ASSERT_EQ(size, s.length());
    ASSERT_LE(size, s.capacity());

    // When the string completes its length to capacity, the capacity becomes equal to size
    string::difference_type diff = static_cast<string::difference_type>(s.capacity() - s.size());
    s.append(static_cast<string::size_type>(diff), ' ');
    ASSERT_EQ(s.capacity(), s.size());

    string::difference_type diff_from_maximum = static_cast<string::difference_type>(s.max_size() - s.size());
    ASSERT_LE(0, diff_from_maximum);
}

TEST(test_string, size)
{
    _check_string_size(string(), 0);
    _check_string_size(string(""), 0);
    _check_string_size(string(0, ' '), 0);

    _check_string_size(string("12"), 2);
    _check_string_size(string("0123456789"), 10);

    ASSERT_LE(numeric_limits<short>::max(), string().max_size()); // in reality it is bigger
}

TEST(test_string, c_str_data)
{
    string s = "abcdef";

    ASSERT_EQ(6, s.size());
    ASSERT_TRUE(strcmp(s.c_str(), "abcdef") == 0);
    ASSERT_TRUE(memcmp(s.data(), "abcdef", 6) == 0);

    const char* saved_data = s.data();
    s = "0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789";
    ASSERT_EQ(100, s.size());
    s += s;
    ASSERT_EQ(200, s.size());

    ASSERT_NE(saved_data, s.data()); // any sane implementation would reallocate memory
    ASSERT_NE(saved_data, s.c_str());
    for (int i = 0; i < 200; i += 10)
        ASSERT_TRUE(memcmp(s.data() + i, "0123456789", 10) == 0);
}

TEST(test_string, reserve_resize)
{
    string s1 = "abc";
    string s2 = s1;
    ASSERT_LE(s1.size(), s1.capacity());
    string::size_type saved_size = s1.size();
    ASSERT_EQ(3, saved_size); // sure
    string::size_type saved_capacity = s1.capacity();

    s1.reserve(100); // surely bigger than the default capacity

    ASSERT_EQ("abc", s1); // ensure the value stayed
    ASSERT_EQ(saved_size, s1.size());
    ASSERT_LT(saved_capacity, s1.capacity());
    ASSERT_LE(100, s1.capacity());

    ASSERT_EQ("abc", s2); // ensure all properties of s2 stayed s1.reserve()
    ASSERT_EQ(saved_size, s2.size());
    ASSERT_EQ(saved_capacity, s2.capacity());

    s2.resize(16);
    ASSERT_EQ(string("abc\0\0\0\0\0\0\0\0\0\0\0\0\0", 16), s2); // this verifies size also

    s2.resize(17);
    ASSERT_EQ(string("abc\0\0\0\0\0\0\0\0\0\0\0\0\0\0", 17), s2); // this verifies size also

    s2.resize(1);
    ASSERT_EQ(string("a", 1), s2); // this verifies size also

    s2.resize(8);
    ASSERT_EQ(string("a\0\0\0\0\0\0\0", 8), s2); // this verifies size also

    s2.resize(0);
    ASSERT_EQ(string(), s2); // this verifies size also

    s2.resize(1);
    ASSERT_EQ(string("\0", 1), s2); // this verifies size also
}

#if defined(SSTL_CXX11) && SSTL_CXX11 != 0
TEST(test_string, shrink_to_fit)
{
    string s(1551, '#');

    string::size_type saved_size = s.size();
    ASSERT_EQ(1551, saved_size); // sure
    string::size_type saved_capacity = s.capacity();
    ASSERT_LE(1551, saved_capacity);

    s.resize(10);
    ASSERT_EQ(1551, saved_size); // sure
    ASSERT_EQ(saved_capacity, s.capacity()); // capacity should not change

    s.shrink_to_fit();
    ASSERT_GT(saved_capacity, s.capacity()); // capacity should become smaller
    ASSERT_GE(64, s.capacity());             // somewhat much smaller
}
#endif

static void _check_find(const string& s, char c, string::size_type pos, string::size_type expected)
{
    string::size_type result = s.find(c, pos);
    ASSERT_EQ(result, expected);
    if (expected != string::npos)
    {
        ASSERT_LE(pos, expected);
        ASSERT_LE(expected + 1, s.size());
    }
    if (pos == 0)
    {
        string::size_type result2 = s.find(c);
        ASSERT_EQ(result2, expected);
    }
    string::size_type result3 = s.find(&c, pos, 1);
    ASSERT_EQ(result3, expected);

    if (c != '\0')
    {
        char sc[2];
        sc[0] = c;
        sc[1] = '\0';
        string::size_type result4 = s.find(sc, pos);
        ASSERT_EQ(result4, expected);
    }
}

TEST(test_string, find_char)
{
    _check_find("", '\0', 0,            string::npos);
    _check_find("", 'a',  0,            string::npos);
    _check_find("", 'B',  1,            string::npos);
    _check_find("", '0',  2,            string::npos);
    _check_find("", 'Z',  1000,         string::npos);
    _check_find("", '=',  string::npos, string::npos);

    _check_find("0123456789", '\0',  0,           string::npos);
    _check_find("0123456789", '\0',  1,           string::npos);
    _check_find("0123456789", '\0',  9,           string::npos);
    _check_find("0123456789", '\0', 10,           string::npos);

    _check_find("0123456789", '0',   0,           0);
    _check_find("0123456789", '0',   1,           string::npos);
    _check_find("0123456789", '0',   9,           string::npos);

    _check_find("0123456789", '4',   0,           4);
    _check_find("0123456789", '4',   3,           4);
    _check_find("0123456789", '4',   4,           4);
    _check_find("0123456789", '4',   5,           string::npos);

    _check_find("0123456789", '9',   0,           9);
    _check_find("0123456789", '9',   3,           9);
    _check_find("0123456789", '9',   9,           9);
    _check_find("0123456789", '9',  10,           string::npos);
    _check_find("0123456789", '9', 100,           string::npos);

    string s0("012\x00xyz\x00vw", 10);
    _check_find(s0, '\0', 0,          3);
    _check_find(s0, '\0', 1,          3);
    _check_find(s0, '\0', 3,          3);
    _check_find(s0, '\0', 4,          7);
    _check_find(s0, '\0', 6,          7);
    _check_find(s0, '\0', 7,          7);
    _check_find(s0, '\0', 8,          string::npos);
}

static void _check_find(const string& s, const string& p, string::size_type pos, string::size_type expected)
{
    string::size_type result = s.find(p, pos);
    ASSERT_EQ(result, expected);
    if (expected != string::npos)
    {
        ASSERT_LE(pos, expected);
        ASSERT_LE(expected, s.size());
    }
    if (pos == 0)
    {
        string::size_type result2 = s.find(p);
        ASSERT_EQ(result2, expected);
    }
    string::size_type result3 = s.find(p.data(), pos, p.size());
    ASSERT_EQ(result3, expected);
}

TEST(test_string, find_str)
{
    _check_find("",         "", 0, 0);
    _check_find("abc",      "", 0, 0);
    _check_find("",      "abc", 0, string::npos);
    _check_find("",      "abc", 0, string::npos);

    _check_find("abc",   "abc", 0, 0);
    _check_find("aabc",  "abc", 0, 1);
    _check_find("aaabc", "abc", 0, 2);
    _check_find("aaabc", "abc", 1, 2);
    _check_find("aaabc", "abc", 2, 2);
    _check_find("ababc", "abc", 2, 2);
    _check_find("ababc", "abc", 3, string::npos);

    _check_find("abe",    "abc", 0, string::npos);
    _check_find("ab",     "abc", 0, string::npos);
    _check_find("abqabc", "abc", 0, 3);

    string s("012\x00xyz\x00vw", 10);
    string sp("2\x00x", 3);
    _check_find(s, sp, 0, 2);
    _check_find(s, sp, 1, 2);
    _check_find(s, sp, 2, 2);
    _check_find(s, sp, 3, string::npos);
}

static void _check_rfind(const string& s, char c, string::size_type pos, string::size_type expected)
{
    string::size_type result = s.rfind(c, pos);
    ASSERT_EQ(result, expected);
    if (expected != string::npos)
    {
        ASSERT_GE(pos, expected);
        ASSERT_LE(expected + 1, s.size());
    }
    if (pos >= s.size())
    {
        string::size_type result2 = s.rfind(c);
        ASSERT_EQ(result2, expected);
    }
    string::size_type result3 = s.rfind(&c, pos, 1);
    ASSERT_EQ(result3, expected);

    if (c != '\0')
    {
        char sc[2];
        sc[0] = c;
        sc[1] = '\0';
        string::size_type result4 = s.rfind(sc, pos);
        ASSERT_EQ(result4, expected);
    }
}

TEST(test_string, rfind_char)
{
    _check_rfind("", '\0', 0,            string::npos);
    _check_rfind("", 'a',  0,            string::npos);
    _check_rfind("", 'B',  1,            string::npos);
    _check_rfind("", '0',  2,            string::npos);
    _check_rfind("", 'Z',  1000,         string::npos);
    _check_rfind("", '=',  string::npos, string::npos);

    _check_rfind("0123456789", '\0',  0,           string::npos);
    _check_rfind("0123456789", '\0',  1,           string::npos);
    _check_rfind("0123456789", '\0',  9,           string::npos);
    _check_rfind("0123456789", '\0', 10,           string::npos);
    _check_rfind("0123456789", '\0', string::npos, string::npos);

    _check_rfind("0123456789", '0',   0,           0);
    _check_rfind("0123456789", '0',   1,           0);
    _check_rfind("0123456789", '0',   9,           0);
    _check_rfind("0123456789", '0',  20,           0);
    _check_rfind("0123456789", '0',  string::npos, 0);

    _check_rfind("0123456789", '4',   0,           string::npos);
    _check_rfind("0123456789", '4',   3,           string::npos);
    _check_rfind("0123456789", '4',   4,           4);
    _check_rfind("0123456789", '4',   5,           4);

    _check_rfind("0123456789", '9',   0,           string::npos);
    _check_rfind("0123456789", '9',   3,           string::npos);
    _check_rfind("0123456789", '9',   9,           9);
    _check_rfind("0123456789", '9',  10,           9);
    _check_rfind("0123456789", '9', 100,           9);
    _check_rfind("0123456789", '9', string::npos,  9);

    string s0("012\x00xyz\x00vw", 10);
    _check_rfind(s0, '\0', 0,            string::npos);
    _check_rfind(s0, '\0', 1,            string::npos);
    _check_rfind(s0, '\0', 3,            3);
    _check_rfind(s0, '\0', 4,            3);
    _check_rfind(s0, '\0', 6,            3);
    _check_rfind(s0, '\0', 7,            7);
    _check_rfind(s0, '\0', 8,            7);
    _check_rfind(s0, '\0', 10,           7);
    _check_rfind(s0, '\0', string::npos, 7);
}

static void _check_rfind(const string& s, const string& p, string::size_type pos, string::size_type expected)
{
    string::size_type result = s.rfind(p, pos);
    ASSERT_EQ(result, expected);
    if (expected != string::npos)
    {
        ASSERT_GE(pos, expected);
        ASSERT_LE(expected, s.size());
    }
    if (pos >= s.size())
    {
        string::size_type result2 = s.rfind(p);
        ASSERT_EQ(result2, expected);
    }
    string::size_type result3 = s.rfind(p.data(), pos, p.size());
    ASSERT_EQ(result3, expected);
}

TEST(test_string, rfind_str)
{
    _check_rfind("",         "", 0, 0);
    _check_rfind("abc",      "", 0, 0);
    _check_rfind("",      "abc", 0, string::npos);
    _check_rfind("",      "abc", 0, string::npos);

    _check_rfind("abc",   "abc", 0, 0);
    _check_rfind("aabc",  "abc", 0, string::npos);
    _check_rfind("aaabc", "abc", 2, 2);
    _check_rfind("aaabc", "abc", 3, 2);
    _check_rfind("aaabc", "abc", 2, 2);
    _check_rfind("ababc", "abc", 2, 2);
    _check_rfind("ababc", "abc", 3, 2);
    _check_rfind("ababc", "abc", string::npos, 2);

    _check_rfind("abe",    "abc", string::npos, string::npos);
    _check_rfind("ab",     "abc", string::npos, string::npos);
    _check_rfind("abqabc", "abc", string::npos, 3);

    _check_rfind("abcabc",       "abc", string::npos, 3);
    _check_rfind("abcabcabcabc", "abc", string::npos, 9);
    _check_rfind("abcabcabefbc", "abc", string::npos, 3);

    string s("012\x00xy2\x00xw", 10);
    string sp("2\x00x", 3);
    _check_rfind(s, sp, 0, string::npos);
    _check_rfind(s, sp, 1, string::npos);
    _check_rfind(s, sp, 2, 2);
    _check_rfind(s, sp, 3, 2);
    _check_rfind(s, sp, 7, 6);
    _check_rfind(s, sp, 8, 6);
    _check_rfind(s, sp, string::npos, 6);
}
