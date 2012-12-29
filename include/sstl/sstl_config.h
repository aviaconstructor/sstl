#ifndef _SSTL__SSTL_CONFIG_INCLUDED
#define _SSTL__SSTL_CONFIG_INCLUDED

#if !defined(SSTL_CXX11)
    #if !defined(__cplusplus) // case if this header was somehow included into a non-C++ file
        #define SSTL_CXX11 0
    #elif __cplusplus >= 201103 // as defined by the standard, fixed in GCC in 2011, http://gcc.gnu.org/bugzilla/show_bug.cgi?id=1773
        #define SSTL_CXX11 1
    #else
        #define SSTL_CXX11 0
    #endif
#endif

#if !defined(SSTL_CONFIG_MULTITHREADED)
    #define SSTL_CONFIG_MULTITHREADED 1
#endif

#endif
