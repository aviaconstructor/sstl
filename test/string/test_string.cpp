#include <gtest/gtest.h>

#define TEST_NATIVE_STL

#if defined(TEST_NATIVE_STL)
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

static void _check_string_indexing(string& s, string::size_type size)
{
    ASSERT_EQ(s.size(), size);
    for (int i = 0; i < size; ++i)
    {
        char c = (i & 0x7) + '0';  // by convention
        ASSERT_EQ(s.at(i), c);
        ASSERT_EQ(s[i], c);
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
    ASSERT_EQ(s.size(), size);
    ASSERT_EQ(s.length(), size);
    ASSERT_GE(s.capacity(), size);

    // When the string completes its length to capacity, the capacity becomes equal to size
    string::difference_type diff = static_cast<string::difference_type>(s.capacity() - s.size());
    s.append(static_cast<string::size_type>(diff), ' ');
    ASSERT_EQ(s.capacity(), s.size());

    string::difference_type diff_from_maximum = static_cast<string::difference_type>(s.max_size() - s.size());
    ASSERT_GE(diff_from_maximum, 0);
}

TEST(test_string, size)
{
    _check_string_size(string(), 0);
    _check_string_size(string(""), 0);
    _check_string_size(string(0, ' '), 0);

    _check_string_size(string("12"), 2);
    _check_string_size(string("0123456789"), 10);
}
