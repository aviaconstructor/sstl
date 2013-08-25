#ifndef _SSTL__SSTL_CONFIG_INCLUDED
#define _SSTL__SSTL_CONFIG_INCLUDED

///@{
/// Whether the C++11 standard is supported by the compiler
#if !defined(SSTL_CXX11)
    #if !defined(__cplusplus) // case if this header was somehow included into a non-C++ file
        #define SSTL_CXX11 0
    #elif __cplusplus >= 201103 // as defined by the standard, fixed in GCC in 2011, http://gcc.gnu.org/bugzilla/show_bug.cgi?id=1773
        #define SSTL_CXX11 1
    #else
        #define SSTL_CXX11 0
    #endif
#endif
///@}

///@{
/// Use in the multithreaded environment, add synchronization
#if !defined(SSTL_CONFIG_MULTITHREADED)
    #define SSTL_CONFIG_MULTITHREADED 1
#endif
///@}

///@{
/// Provide interoperability with compiler standard library.
///
/// This adds conversion routines for a few SSTL types, makes easier using both libraries at the same time.
#if !defined(SSTL_STL_INTEROPERABILITY)
    #define SSTL_STL_INTEROPERABILITY 0
#endif
///@}

#endif
