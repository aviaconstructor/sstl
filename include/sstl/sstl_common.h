#ifndef _SSTL__SSTL_COMMON_INCLUDED
#define _SSTL__SSTL_COMMON_INCLUDED

#include "sstl_version.h"
#include "sstl_config.h"

#include <memory.h>
#include <malloc.h>
#include <time.h>
#include <assert.h>
//#include <string.h>
//#include <stdarg.h>
//#include <stdlib.h>
//#include <stdio.h>
//#include <ctype.h>
#include <limits.h>

#if defined(_WIN32)  // Generic Windows, both 32 and 64
    #include <winbase.h>
#else
    #include <unistd.h>
#endif

#if !defined(SSTL_NAMESPACE)
    #define SSTL_NAMESPACE sstl
#endif

#if !defined(SSTL_STATIC_ASSERT)
    #define SSTL_STATIC_ASSERT(expression, str)  typedef char _sstl__static_assert_ ## __LINE__ [((expression) && (str)) ? 1 : -1]
#endif

#if !defined(SSTL_ASSERT)
    #define SSTL_ASSERT(expression)  assert(expression)
#endif

#define SSTL_USE(variable)  (variable)

#define SSTL_CONSTEXPR

#if defined(_WIN32)
    typedef __int64            sstl_int64;
    typedef unsigned __int64   sstl_uint64;
#else
    typedef long long          sstl_int64;
    typedef unsigned long long sstl_uint64;
#endif

#endif
