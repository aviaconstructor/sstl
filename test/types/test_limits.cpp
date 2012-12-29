#include <gtest/gtest.h>
#include <float.h>
#include <limits.h>

//#define TEST_NATIVE_STL

#if defined(TEST_NATIVE_STL)
    #include <limits>
    #include <sstl/sstl_common.h>

    using namespace std;
#else
    #include <sstl/limits>

    using namespace SSTL_NAMESPACE;
#endif

TEST(test_limits, bool)
{
#if 0 // not supported yet
    ASSERT_TRUE(numeric_limits<bool>::is_specialized);
    ASSERT_FALSE(numeric_limits<bool>::is_signed);
    ASSERT_TRUE(numeric_limits<bool>::is_integer);
    ASSERT_TRUE(numeric_limits<bool>::is_exact);
    ASSERT_FALSE(numeric_limits<bool>::has_infinity);
    ASSERT_FALSE(numeric_limits<bool>::has_quiet_NaN);
    ASSERT_FALSE(numeric_limits<bool>::has_signaling_NaN);
    ASSERT_EQ(numeric_limits<bool>::has_denorm, denorm_absent);
    ASSERT_FALSE(numeric_limits<bool>::has_denorm_loss);
    ASSERT_EQ(numeric_limits<bool>::round_style, round_toward_zero);
    ASSERT_FALSE(numeric_limits<bool>::is_iec559);
    ASSERT_FALSE(numeric_limits<bool>::is_bounded);
    ASSERT_FALSE(numeric_limits<bool>::is_modulo);
    ASSERT_EQ(numeric_limits<bool>::digits, 1);
    ASSERT_EQ(numeric_limits<bool>::digits10, 0);
    ASSERT_EQ(numeric_limits<bool>::max_digits10, 0);
    ASSERT_EQ(numeric_limits<bool>::radix, 0);
    ASSERT_EQ(numeric_limits<bool>::min_exponent, 0);
    ASSERT_EQ(numeric_limits<bool>::min_exponent10, 0);
    ASSERT_EQ(numeric_limits<bool>::max_exponent, 0);
    ASSERT_EQ(numeric_limits<bool>::max_exponent10, 0);
#endif
    ASSERT_FALSE(numeric_limits<bool>::min());
#if defined(SSTL_CXX11) && SSTL_CXX11 != 0
    ASSERT_FALSE(numeric_limits<bool>::lowest());
#endif
    ASSERT_TRUE(numeric_limits<bool>::max());
    ASSERT_FALSE(numeric_limits<bool>::epsilon());
}

TEST(test_limits, char)
{
    EXPECT_EQ(CHAR_MIN, numeric_limits<char>::min());
#if defined(SSTL_CXX11) && SSTL_CXX11 != 0
    ASSERT_EQ(numeric_limits<char>::lowest(), CHAR_MIN);
#endif
    EXPECT_EQ(CHAR_MAX, numeric_limits<char>::max());
    EXPECT_EQ(0, numeric_limits<char>::epsilon());
}

TEST(test_limits, signed_char)
{
    ASSERT_EQ(numeric_limits<signed char>::min(), SCHAR_MIN);
#if defined(SSTL_CXX11) && SSTL_CXX11 != 0
    ASSERT_EQ(numeric_limits<signed char>::lowest(), SCHAR_MIN);
#endif
    ASSERT_EQ(numeric_limits<signed char>::max(), SCHAR_MAX);
    ASSERT_EQ(numeric_limits<signed char>::epsilon(), 0);
}

TEST(test_limits, unsigned_char)
{
    ASSERT_EQ(numeric_limits<unsigned char>::min(), 0);
#if defined(SSTL_CXX11) && SSTL_CXX11 != 0
    ASSERT_EQ(numeric_limits<unsigned char>::lowest(), 0);
#endif
    ASSERT_EQ(numeric_limits<unsigned char>::max(), UCHAR_MAX);
    ASSERT_EQ(numeric_limits<unsigned char>::epsilon(), 0);
}

TEST(test_limits, short)
{
    ASSERT_EQ(numeric_limits<short>::min(), SHRT_MIN);
#if defined(SSTL_CXX11) && SSTL_CXX11 != 0
    ASSERT_EQ(numeric_limits<short>::lowest(), SHRT_MIN);
#endif
    ASSERT_EQ(numeric_limits<short>::max(), SHRT_MAX);
    ASSERT_EQ(numeric_limits<short>::epsilon(), 0);
}

TEST(test_limits, unsigned_short)
{
    ASSERT_EQ(numeric_limits<unsigned short>::min(), 0);
#if defined(SSTL_CXX11) && SSTL_CXX11 != 0
    ASSERT_EQ(numeric_limits<unsigned short>::lowest(), 0);
#endif
    ASSERT_EQ(numeric_limits<unsigned short>::max(), USHRT_MAX);
    ASSERT_EQ(numeric_limits<unsigned short>::epsilon(), 0);
}

TEST(test_limits, int)
{
    ASSERT_EQ(numeric_limits<int>::min(), INT_MIN);
#if defined(SSTL_CXX11) && SSTL_CXX11 != 0
    ASSERT_EQ(numeric_limits<int>::lowest(), INT_MIN);
#endif
    ASSERT_EQ(numeric_limits<int>::max(), INT_MAX);
    ASSERT_EQ(numeric_limits<int>::epsilon(), 0);
}

TEST(test_limits, unsigned_int)
{
    ASSERT_EQ(numeric_limits<unsigned int>::min(), 0);
#if defined(SSTL_CXX11) && SSTL_CXX11 != 0
    ASSERT_EQ(numeric_limits<unsigned int>::lowest(), 0);
#endif
    ASSERT_EQ(numeric_limits<unsigned int>::max(), UINT_MAX);
    ASSERT_EQ(numeric_limits<unsigned int>::epsilon(), 0);
}

TEST(test_limits, long)
{
    ASSERT_EQ(numeric_limits<long>::min(), LONG_MIN);
#if defined(SSTL_CXX11) && SSTL_CXX11 != 0
    ASSERT_EQ(numeric_limits<long>::lowest(), LONG_MIN);
#endif
    ASSERT_EQ(numeric_limits<long>::max(), LONG_MAX);
    ASSERT_EQ(numeric_limits<long>::epsilon(), 0);
}

TEST(test_limits, unsigned_long)
{
    ASSERT_EQ(numeric_limits<unsigned long>::min(), 0);
#if defined(SSTL_CXX11) && SSTL_CXX11 != 0
    ASSERT_EQ(numeric_limits<unsigned long>::lowest(), 0);
#endif
    ASSERT_EQ(numeric_limits<unsigned long>::max(), ULONG_MAX);
    ASSERT_EQ(numeric_limits<unsigned long>::epsilon(), 0);
}

TEST(test_limits, sstl_int64)
{
    ASSERT_EQ(numeric_limits<sstl_int64>::min(), LLONG_MIN);
#if defined(SSTL_CXX11) && SSTL_CXX11 != 0
    ASSERT_EQ(numeric_limits<sstl_int64>::lowest(), LLONG_MIN);
#endif
    ASSERT_EQ(numeric_limits<sstl_int64>::max(), LLONG_MAX);
    ASSERT_EQ(numeric_limits<sstl_int64>::epsilon(), 0);
}

TEST(test_limits, sstl_uint64)
{
    ASSERT_EQ(numeric_limits<sstl_uint64>::min(), 0);
#if defined(SSTL_CXX11) && SSTL_CXX11 != 0
    ASSERT_EQ(numeric_limits<sstl_uint64>::lowest(), 0);
#endif
    ASSERT_EQ(numeric_limits<sstl_uint64>::max(), ULLONG_MAX);
    ASSERT_EQ(numeric_limits<sstl_uint64>::epsilon(), 0);
}

TEST(test_limits, float)
{
    ASSERT_EQ(numeric_limits<float>::min(), FLT_MIN);
#if defined(SSTL_CXX11) && SSTL_CXX11 != 0
    ASSERT_EQ(numeric_limits<float>::lowest(), -FLT_MAX);
#endif
    ASSERT_EQ(numeric_limits<float>::max(), FLT_MAX);
    ASSERT_EQ(numeric_limits<float>::epsilon(), FLT_EPSILON);
}

TEST(test_limits, double)
{
    ASSERT_EQ(numeric_limits<double>::min(), DBL_MIN);
#if defined(SSTL_CXX11) && SSTL_CXX11 != 0
    ASSERT_EQ(numeric_limits<double>::lowest(), -DBL_MAX);
#endif
    ASSERT_EQ(numeric_limits<double>::max(), DBL_MAX);
    ASSERT_EQ(numeric_limits<double>::epsilon(), DBL_EPSILON);
}
