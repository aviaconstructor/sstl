#ifndef _SSTL__SSTL_TYPES_INCLUDED
#define _SSTL__SSTL_TYPES_INCLUDED

///@{
/// 64-bit type compatible with older compilers
///
#if defined(_WIN32)
    typedef __int64            sstl_int64;
    typedef unsigned __int64   sstl_uint64;
#else
    typedef long long          sstl_int64;
    typedef unsigned long long sstl_uint64;
#endif
///@}

#if !defined(SSTL_EXTERNAL_SIZE_TYPEDEFS) || SSTL_EXTERNAL_SIZE_TYPEDEFS == 0
    typedef size_t    sstl_size_t;
    typedef ptrdiff_t sstl_ptrdiff_t;
#endif

#endif
