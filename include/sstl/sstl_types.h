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

    /// Size type as used by sstl in containers
    ///
    /// By purpose, even for 64-bit platforms, it is unsigned, which is 32-bits in most architectures.
    /// This can be changed by defining sstl_size_type outside and defining the macro
    /// SSTL_EXTERNAL_SIZE_TYPEDEFS to 1.
    ///
    typedef unsigned sstl_size_type;

    /// Difference type as used by sstl in containers
    ///
    /// By purpose, even for 64-bit platforms, it is int, which is 32-bits in most architectures.
    /// This can be changed by defining sstl_size_type outside and defining the macro
    /// SSTL_EXTERNAL_SIZE_TYPEDEFS to 1.
    ///
    typedef int sstl_difference_type;
#endif

#endif
