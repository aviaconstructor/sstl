#include <gtest/gtest.h>

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

#if defined(_SSTL__STRING_INCLUDED)
    ASSERT_EQ(32, sizeof(string::_buffer_type));
#endif
}

TEST(test_string, constructors)
{
    static const char aaa [] = "AAAAAAAAAAAAAAAA";
    string s1 = "AAAAAAAAAAAAAAAA";
    string s2(aaa);
    string s3(aaa, 16);
    string s4(16, 'A');
    string s5(s1);
    string s6(s1, 0, 16);

    ASSERT_EQ(s1, s2);
    ASSERT_EQ(s1, s3);
    ASSERT_EQ(s1, s4);
    ASSERT_EQ(s1, s5);
    ASSERT_EQ(s1, s6);

    string s7(string("012345678901234567890123456789"), 0, 10);
    ASSERT_EQ(string("0123456789"), s7);

    string s8(string("012345678901234567890123456789"), 9, 5);
    ASSERT_EQ(string("90123"), s8);

    string s9(string("012345678901234567890123456789"), 2);
    ASSERT_EQ(string("2345678901234567890123456789"), s9);

    string s10(aaa, aaa + 16);
    ASSERT_EQ(s1, s10);

    string s11(s9.rbegin(), s9.rend());
    ASSERT_EQ(s11, "9876543210987654321098765432");

    string s12 = s11;
    ASSERT_EQ(s12, "9876543210987654321098765432");
#if defined(_SSTL__STRING_INCLUDED)
    ASSERT_TRUE(s12.is_shared());
    ASSERT_TRUE(s11.is_shared());
#endif

    string s13(s12.begin(), s12.end());
    ASSERT_EQ(s13, s12);

#if SSTL_CXX11 != 0 || defined(_SSTL__STRING_INCLUDED) // if we test sstl, or if this is a C++11 compliant STL
    string s14(s11.cbegin(), s11.cend());
    ASSERT_EQ(s14, s11);

    string s15(s9.crbegin(), s9.crend());
    ASSERT_EQ(s15, s11); // reverse
#endif

    // collect constructed shared strings
    string s16(s13);
    string s17 = s13;
    string s18(s16);
    string s19(s13);
    string s20 = s13;
}

TEST(test_string, assign)
{
    string s1 = "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA";
    string s2 = "abc";
    string s3 = "abcdefghijABCDEFGHIJabcdefghijABCDEFGHIJ";
    string s4(500, 'z');
    string s5(1000, '\0');
    string s6;
    string s7;
    string s8;
    string s9(10000, 'q');
    string s10("dummy");

    s2 = s1;
    s3.assign(s2);
    s4.assign(s1.c_str());
    s5.assign(s4.data(), s4.size());
    s6.assign(s4.size(), 'A');
    s7.assign(s6, 0, s6.size());
    s8.assign(s7, 0, 1000); // will be truncated to size()
    s9.assign(s8, 0, string::npos);
    s10.assign(s2.begin(), s2.end());

    ASSERT_EQ(s1, s2);
    ASSERT_EQ(s1, s3);
    ASSERT_EQ(s1, s4);
    ASSERT_EQ(s1, s5);
    ASSERT_EQ(s1, s6);
    ASSERT_EQ(s1, s7);
    ASSERT_EQ(s1, s8);
    ASSERT_EQ(s1, s9);
    ASSERT_EQ(s1, s10);

    string s11 = "ABCDEFGH";
    string s12;
    s12.assign(s11.rbegin(), s11.rend());
    ASSERT_EQ(s12, "HGFEDCBA");

    // now change the original s1, unshare in SSTL
    s1.assign("00000");
    ASSERT_EQ(s3, s2); // these should stay
    s2 = s1;
    s3.assign(s2);
    s4.assign(s1.c_str());
    s5.assign(s4.data(), s4.size());
    s6.assign(s4.size(), '0');
    s7.assign(s6, 0, s6.size());
    s8.assign(s7, 0, 1000); // will be truncated to size()
    s9.assign(s8, 0, string::npos);
    s10.assign(s2.begin(), s2.end());

    ASSERT_EQ(s1, "00000");
    ASSERT_EQ(s1, s2);
    ASSERT_EQ(s1, s3);
    ASSERT_EQ(s1, s4);
    ASSERT_EQ(s1, s5);
    ASSERT_EQ(s1, s6);
    ASSERT_EQ(s1, s7);
    ASSERT_EQ(s1, s8);
    ASSERT_EQ(s1, s9);
    ASSERT_EQ(s1, s10);
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

#if SSTL_CXX11 != 0 || defined(_SSTL__STRING_INCLUDED) // if we test sstl, or if this is a C++11 compliant STL
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

#if SSTL_CXX11 != 0 || defined(_SSTL__STRING_INCLUDED) // if we test sstl, or if this is a C++11 compliant STL
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

TEST(test_string, push_back)
{
    // make sure the amount if pushbacks is big enough to cause reallocation
    string s;
    s.push_back('0');    s.push_back('1');    s.push_back('2');    s.push_back('3');    s.push_back('4');
    s.push_back('5');    s.push_back('6');    s.push_back('7');    s.push_back('8');    s.push_back('9');
    ASSERT_EQ(s, "0123456789");
    s.push_back('A');    s.push_back('B');    s.push_back('C');    s.push_back('D');    s.push_back('E');
    s.push_back('F');    s.push_back('G');    s.push_back('H');    s.push_back('I');    s.push_back('J');
    s.push_back('K');    s.push_back('L');    s.push_back('M');    s.push_back('N');    s.push_back('O');
    ASSERT_EQ(s, "0123456789ABCDEFGHIJKLMNO");
    s.push_back('a');    s.push_back('b');    s.push_back('c');    s.push_back('d');    s.push_back('e');
    s.push_back('f');    s.push_back('g');    s.push_back('h');    s.push_back('i');    s.push_back('j');
    s.push_back('k');    s.push_back('l');    s.push_back('m');    s.push_back('n');    s.push_back('o');
    ASSERT_EQ(s, "0123456789ABCDEFGHIJKLMNOabcdefghijklmno");
}

#if SSTL_CXX11 != 0 || defined(_SSTL__STRING_INCLUDED) // if we test sstl, or if this is a C++11 compliant STL
TEST(test_string, pop_back)
{
    string s = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    s.pop_back();
    ASSERT_EQ(s, "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxy");
    s.pop_back();    s.pop_back();    s.pop_back();    s.pop_back();    s.pop_back();
    s.pop_back();    s.pop_back();    s.pop_back();    s.pop_back();    s.pop_back();
    ASSERT_EQ(s, "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmno");
    s.pop_back();    s.pop_back();    s.pop_back();    s.pop_back();    s.pop_back();
    s.pop_back();    s.pop_back();    s.pop_back();    s.pop_back();    s.pop_back();
    ASSERT_EQ(s, "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcde");
    s.pop_back();    s.pop_back();    s.pop_back();    s.pop_back();    s.pop_back();
    s.pop_back();    s.pop_back();    s.pop_back();    s.pop_back();    s.pop_back();
    ASSERT_EQ(s, "0123456789ABCDEFGHIJKLMNOPQRSTU");
    s.pop_back();    s.pop_back();    s.pop_back();    s.pop_back();    s.pop_back();
    s.pop_back();    s.pop_back();    s.pop_back();    s.pop_back();    s.pop_back();
    ASSERT_EQ(s, "0123456789ABCDEFGHIJK");
    s.pop_back();    s.pop_back();    s.pop_back();    s.pop_back();    s.pop_back();
    s.pop_back();    s.pop_back();    s.pop_back();    s.pop_back();    s.pop_back();
    ASSERT_EQ(s, "0123456789A");
    s.pop_back();    s.pop_back();    s.pop_back();    s.pop_back();    s.pop_back();
    s.pop_back();    s.pop_back();    s.pop_back();    s.pop_back();    s.pop_back();
    ASSERT_EQ(s, "0");
    s.pop_back();
    ASSERT_EQ(s, "");
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

    string ss = s; // test on shared string (SSTL specific)
    string::size_type result_s = ss.find(c, pos);
    ASSERT_EQ(result_s, expected);
    if (expected != string::npos)
    {
        ASSERT_LE(pos, expected);
        ASSERT_LE(expected + 1, ss.size());
    }
    if (pos == 0)
    {
        string::size_type result_s2 = ss.find(c);
        ASSERT_EQ(result_s2, expected);
    }
    string::size_type result_s3 = ss.find(&c, pos, 1);
    ASSERT_EQ(result_s3, expected);

    if (c != '\0')
    {
        char sc[2];
        sc[0] = c;
        sc[1] = '\0';
        string::size_type result4 = s.find(sc, pos);
        ASSERT_EQ(result4, expected);
        string::size_type result5 = ss.find(sc, pos);
        ASSERT_EQ(result5, expected);
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

static void _check_substr(const string& s, string::size_type pos, string::size_type count, const string& result)
{
    string ret1 = s.substr(pos, count);
    ASSERT_EQ(ret1, result);

    string ret2 = s.substr(pos);
    ASSERT_EQ(ret2, string(s, pos));
    ASSERT_EQ(ret2, string(s, pos, s.size() - pos));

    string ret3 = s.substr();
    ASSERT_EQ(ret3, s);
}

TEST(test_string, substr)
{
    _check_substr("", 0, 0, "");
    _check_substr("", 0, 1, "");
    _check_substr("", 0, string::npos, "");

    _check_substr("1", 0, 0, "");
    _check_substr("2", 0, 1, "2");
    _check_substr("3", 0, string::npos, "3");

    _check_substr("4", 1, 0, "");
    _check_substr("5", 1, 1, "");
    _check_substr("6", 1, string::npos, "");

    _check_substr("ABC", 0, 3, "ABC");
    _check_substr("DEF", 1, 5, "EF");
    _check_substr("GHI", 2, 7, "I");

    _check_substr("abc", 0, string::npos, "abc");
    _check_substr("def", 1, 0, "");
    _check_substr("ghi", 2, 0, "");

    _check_substr(string("\0\1\2\3\4", 5), 1, 2, "\1\2");
    _check_substr("0123456789abcdefghijk", 0, 0, "");
    _check_substr("0123456789abcdefghijk", 20, 0, "");
    _check_substr("0123456789abcdefghijk", 2, 5, "23456");

    _check_substr("0123456789012345678901234567890123456789012345678901234567890123456789",
            2, 50,  "23456789012345678901234567890123456789012345678901");
}

static void _check_append(const string& s, const string& what, const string& result)
{
    string res = s;
    const string& ret_val = res.append(what);
    ASSERT_EQ(res, result);
    ASSERT_EQ(ret_val, result);

    res = s;
    res.append(what.c_str());
    ASSERT_EQ(res, result);

    res = s;
    res.append(what.data(), what.size());
    ASSERT_EQ(res, result);

    res = s;
    res.append(what.begin(), what.end());
    ASSERT_EQ(res, result);

    res = s;
    res.append(what.begin(), what.end());
    ASSERT_EQ(res, result);

    res = s;
    res.append(what.rbegin(), what.rend());
    ASSERT_EQ(res, s + string(what.rbegin(), what.rend()));

    if (what.size() == 1)
    {
        res = s;
        res.append(1, what[0]);
        ASSERT_EQ(res, result);
    }
}

TEST(test_string, append)
{
    string s0;
    const string& ret_val0 = s0.append("");
    ASSERT_EQ(s0, "");
    ASSERT_EQ(s0, ret_val0);

    string s1 = "";
    const string& ret_val1 = s1.append("abcde");
    ASSERT_EQ(s1, "abcde");
    ASSERT_EQ(s1, ret_val1);

    string s2 = "abc";
    const string& ret_val2 = s2.append("abcde");
    ASSERT_EQ(s2, "abcabcde");
    ASSERT_EQ(s2, ret_val2);

    string s3;
    const string& ret_val3 = s3.append(70, '0');
    ASSERT_EQ(s3, "0000000000000000000000000000000000000000000000000000000000000000000000");
    ASSERT_EQ(s3, ret_val3);

    string s4 = "01234567890123456789012345678901234567890123456789";
    const string& ret_val4 = s4.append(50, 'z');
    ASSERT_EQ(s4, "01234567890123456789012345678901234567890123456789zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz");
    ASSERT_EQ(s4, ret_val4);

    _check_append("", "", "");
    _check_append("", "1", "1");
    _check_append("2", "", "2");
    _check_append("3", "4", "34");
    _check_append("", "56", "56");
    _check_append("78", "", "78");
    _check_append("90", "AB", "90AB");
    _check_append("012345678901234567890123456789012345678901234567890123456789",
                  "ABCDEFGHIJABCDEFGHIJABCDEFGHIJABCDEFGHIJABCDEFGHIJABCDEFGHIJ",
                  "012345678901234567890123456789012345678901234567890123456789ABCDEFGHIJABCDEFGHIJABCDEFGHIJABCDEFGHIJABCDEFGHIJABCDEFGHIJ");
}

static void _check_copy(const string& s, string::size_type count, string::size_type pos)
{
    char buff[256];
    memset(buff, 0, sizeof(buff));

    s.copy(buff, count, pos);
    ASSERT_TRUE(memcmp(buff, s.data() + pos, s.size() - pos) == 0);
    memset(buff, 0, sizeof(buff));

    s.copy(buff, count);
    ASSERT_TRUE(memcmp(buff, s.data(), s.size() - pos) == 0);
    memset(buff, 0, sizeof(buff));

    s.copy(buff, sizeof(buff));
    ASSERT_TRUE(memcmp(buff, s.data(), s.size() - pos) == 0);
    memset(buff, 0, sizeof(buff));

    s.copy(buff, string::npos); // also legal, limits by s.size()
    ASSERT_TRUE(memcmp(buff, s.data(), s.size() - pos) == 0);
    memset(buff, 0, sizeof(buff));
}

TEST(test_string, copy)
{
    _check_copy("", 0, 0);
    _check_copy("A", 1, 0);
    _check_copy("B", 0, 1);
    _check_copy("CDEFGH", 100, 0);
    _check_copy("0123456789", string::npos, 5);
}

TEST(test_string, operator_plus)
{
    string s1, s2, s3;
    s1 = s2 + s3;
    ASSERT_EQ(s1, "");
    s1 = "a" + s3;
    ASSERT_EQ(s1, "a");
    s1 = s1 + s3;
    ASSERT_EQ(s1, "a");
    s1 = s1 + s1;
    ASSERT_EQ(s1, "aa");
    s1 = s1 + "b";
    ASSERT_EQ(s1, "aab");
    s1 = 'c' + s1;
    ASSERT_EQ(s1, "caab");
    s1 = s1 + 'd';
    ASSERT_EQ(s1, "caabd");
    s1 = "1" + s1 + 'z';
    ASSERT_EQ(s1, "1caabdz");
    s1 = s1 + s1 + s1;
    ASSERT_EQ(s1, "1caabdz1caabdz1caabdz");
    s1 = s1.c_str() + s1 + s1.c_str();
    ASSERT_EQ(s1, "1caabdz1caabdz1caabdz1caabdz1caabdz1caabdz1caabdz1caabdz1caabdz");

    s2 = s1;
    s3 = s1 + s2;
    ASSERT_EQ(s3, "1caabdz1caabdz1caabdz1caabdz1caabdz1caabdz1caabdz1caabdz1caabdz1caabdz1caabdz1caabdz1caabdz1caabdz1caabdz1caabdz1caabdz1caabdz");

    s2 = "%";
    s3.assign(5, '*');
    s1 = s2 + s3 + s1 + s3 + s2;
    ASSERT_EQ(s1, "%*****1caabdz1caabdz1caabdz1caabdz1caabdz1caabdz1caabdz1caabdz1caabdz*****%");

    s2.clear();
    s1 = s2 + 'a' + 'b' + 'c' + 'd' + 'e' + 'f' + 'g' + 'h' + 'i' + 'j' + 'k' + 'l' + 'm' + 'n' + 'o' + 'p' + 'q';
    ASSERT_EQ(s1, "abcdefghijklmnopq");
    s1 = s1 + "A" + "B" + "C" + "D" + "E" + "F" + "G" + "H" + "I" + "J" + "K" + "L" + "M" + "N" + "O" + "P" + "Q";
    ASSERT_EQ(s1, "abcdefghijklmnopqABCDEFGHIJKLMNOPQ");
    s1 = s1 + "0" + "1" + "2" + '3' + '4' + "5" + "6" + '7' + "8" + "9";
    ASSERT_EQ(s1, "abcdefghijklmnopqABCDEFGHIJKLMNOPQ0123456789");
}

TEST(test_string, operator_plus_equal)
{
    string s1;
    s1 += "abcdefghijklmnopqABCDEFGHIJKLMNOPQ0123456789";
    s1 += '+';
    s1 += '-';
    s1 += "()";
    s1 += "[(=)]";
    s1 += '|';
    s1 += "{}";
    ASSERT_EQ(s1, "abcdefghijklmnopqABCDEFGHIJKLMNOPQ0123456789+-()[(=)]|{}");
    s1 += '1';    s1 += '2';    s1 += '3';    s1 += '4';    s1 += '5';
    s1 += '6';    s1 += '7';    s1 += '8';    s1 += '9';    s1 += '0';
    s1 += "1";    s1 += "2";    s1 += "3";    s1 += "4";    s1 += "5";
    s1 += "6";    s1 += "7";    s1 += "8";    s1 += "9";    s1 += "0";
    ASSERT_EQ(s1, "abcdefghijklmnopqABCDEFGHIJKLMNOPQ0123456789+-()[(=)]|{}12345678901234567890");
}

TEST(test_string, sizeofs)
{
    printf("  sizeof(string) = %d\n", static_cast<int>(sizeof(string)));
}
