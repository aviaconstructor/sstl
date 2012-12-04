#include <cassert>

#if defined(TEST_NATIVE_STL)
    #include <string>
    #include <iterator>

    using namespace std;
#else
    #include <sstl/string>
    #include <sstl/iterator>
    #include <sstl/_impl/string.cpp>

    using namespace SSTL_NAMESPACE;
#endif

static void _check_string_indexing(string& s, string::size_type size)
{
    assert(s.size() == size);
    for (int i = 0; i < size; ++i)
    {
        char c = (i & 0x7) + '0';  // by convention
        assert(s.at(i) == c);
        assert(s[i] == c);
    }
}

void test_string_indexing()
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
    assert(s.size() == size);

    assert(&(*s.begin()) == &s[0]);
    assert(*s.begin() == front);

    assert(&(*s.end()) == &s[0] + size);

    assert(&(*s.rbegin()) == &s[size - 1]);
    assert(*s.rbegin() == back);

    assert(&(*s.rend()) == &s[0] - 1);

#if !defined(TEST_NATIVE_STL) || __cplusplus >= 201100
    assert(&(*s.cbegin()) == &s[0]);
    assert(*s.cbegin() == front);

    assert(&(*s.cend()) == &s[0] + size);

    assert(&(*s.crbegin()) == &s[size - 1]);
    assert(*s.crbegin() == back);

    assert(&(*s.crend()) == &s[0] - 1);

    assert(s.front() == front);
    assert(s.front() == s[0]);
    assert(&s.front() == s.begin());

    assert(s.back() == back);
    assert(s.back() == s[size - 1]);
#endif
}

void test_string_iterators()
{
    _check_string_iterators(string("1"), 1, '1', '1');
    _check_string_iterators(string("12"), 2, '1', '2');
    _check_string_iterators(string("0123456789"), 10, '0', '9');
}

static void _check_string_size(string s, string::size_type size)
{
    assert(s.empty() == (s.size() == 0));
    assert(s.size() == size);
    assert(s.length() == size);
    assert(s.capacity() >= size);

    // When the string completes its length to capacity, the capacity becomes equal to size
    string::difference_type diff = static_cast<string::difference_type>(s.capacity() - s.size());
    s.append(static_cast<string::size_type>(diff), ' ');
    assert(s.capacity() == s.size());

    string::difference_type diff_from_maximum = static_cast<string::difference_type>(string::max_size() - s.size());
    assert(diff_from_maximum >= 0);

}

void test_string_size()
{
    _check_string_size(string(), 0);
    _check_string_size(string(""), 0);
    _check_string_size(string(0, ' '), 0);

    _check_string_size(string("12"), 2);
    _check_string_size(string("0123456789"), 10);
}

int main()
{
    test_string_indexing();
    test_string_iterators();
    test_string_size();
    return 0;
}
