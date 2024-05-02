#ifndef _GLIBMM_CONFIG_H
#define _GLIBMM_CONFIG_H

/* Define to omit deprecated API from the library. */
/* #undef GLIBMM_DISABLE_DEPRECATED */

/* Major version number of glibmm. */
#define GLIBMM_MAJOR_VERSION 2

/* Minor version number of glibmm. */
#define GLIBMM_MINOR_VERSION 78

/* Micro version number of glibmm. */
#define GLIBMM_MICRO_VERSION 1

/* Define if glibmm is built as a static library */
/* #undef GLIBMM_STATIC_LIB */

/* The size of wchar_t, as computed by sizeof. */
#define GLIBMM_SIZEOF_WCHAR_T 2

/* The size of short, as computed by sizeof. */
#define GLIBMM_SIZEOF_SHORT 2

/* The size of int, as computed by sizeof. */
#define GLIBMM_SIZEOF_INT 4

/* The size of long, as computed by sizeof. */
#define GLIBMM_SIZEOF_LONG 4

/* The size of long long, as computed by sizeof. */
#define GLIBMM_SIZEOF_LONG_LONG 8

/* Defined when the -Ddebug-refcounting configure argument was given */
/* #undef GLIBMM_DEBUG_REFCOUNTING */

#ifdef _WIN32
  /* Win32 compilers have a lot of varation */
# if defined(_MSC_VER)
#  define GLIBMM_MSC 1
#  define GLIBMM_WIN32 1
#  define GLIBMM_DLL 1
# elif defined(__CYGWIN__)
#  define GLIBMM_CONFIGURE 1
# elif defined(__MINGW32__)
#  define GLIBMM_WIN32 1
#  define GLIBMM_CONFIGURE 1
# else
   /* AIX clR compiler complains about this even though it doesn't get this far */
#  error "Unknown architecture (send me gcc --dumpspecs or equiv)"
# endif
#else
# define GLIBMM_CONFIGURE 1
#endif /* _WIN32 */

#ifdef GLIBMM_CONFIGURE

/* Define only on Mac OS, COCOA */
/* #undef GLIBMM_OS_COCOA */

/* Define if extern "C" and extern "C++" function pointers are compatible. */
#define GLIBMM_CAN_ASSIGN_NON_EXTERN_C_FUNCTIONS_TO_EXTERN_C_CALLBACKS 1

/* Define if non-instantiated templates may dynamic_cast<> to an undefined
   type. */
#define GLIBMM_CAN_USE_DYNAMIC_CAST_IN_UNUSED_TEMPLATE_WITHOUT_DEFINITION 1

/* Defined if a static member variable may be initialized inline to
   std::string::npos */
#define GLIBMM_HAVE_ALLOWS_STATIC_INLINE_NPOS 1

/* Define if the compiler disambiguates template specializations for const and
   non-const types. */
#define GLIBMM_HAVE_DISAMBIGUOUS_CONST_TEMPLATE_SPECIALIZATIONS 1

/* Defined if std::iterator_traits<> is standard-conforming */
#define GLIBMM_HAVE_STD_ITERATOR_TRAITS 1

/* Defined if std::reverse_iterator is in Sun libCstd style */
/* #undef GLIBMM_HAVE_SUN_REVERSE_ITERATOR */

/* Defined if the STL containers have templated sequence ctors */
#define GLIBMM_HAVE_TEMPLATE_SEQUENCE_CTORS 1

/* Define to 1 if wide stream is available. */
#define GLIBMM_HAVE_WIDE_STREAM 1

#endif /* GLIBMM_CONFIGURE */

#ifdef GLIBMM_MSC
# define GLIBMM_HAVE_STD_ITERATOR_TRAITS 1
# define GLIBMM_HAVE_TEMPLATE_SEQUENCE_CTORS 1
# define GLIBMM_HAVE_WIDE_STREAM 1
# define GLIBMM_HAVE_DISAMBIGUOUS_CONST_TEMPLATE_SPECIALIZATIONS 1
# define GLIBMM_CAN_USE_DYNAMIC_CAST_IN_UNUSED_TEMPLATE_WITHOUT_DEFINITION 1
# define GLIBMM_CAN_ASSIGN_NON_EXTERN_C_FUNCTIONS_TO_EXTERN_C_CALLBACKS 1
# pragma warning (disable: 4786 4355 4800 4181)

/* We have GLIBMM_HAVE_ALLOWS_STATIC_INLINE_NPOS for Visual Studio 2017+ */
#if (_MSC_VER >= 1910)
#define GLIBMM_HAVE_ALLOWS_STATIC_INLINE_NPOS 1
#endif

#endif /* GLIBMM_MSC */

/* Enable DLL-specific stuff only when not building a static library */
#if !defined(__CYGWIN__) && defined(__MINGW32__) && !defined(GLIBMM_STATIC_LIB)
# define GLIBMM_DLL 1
#endif

#ifdef GLIBMM_DLL
# if defined(GLIBMM_BUILD)
#  define GLIBMM_API __declspec(dllexport)
# elif !defined (__GNUC__)
#  define GLIBMM_API __declspec(dllimport)
# else /* don't dllimport classes/methods on GCC/MinGW */
#  define GLIBMM_API
# endif
/* Build a static or non-native-Windows library */
#else
# define GLIBMM_API
#endif /* GLIBMM_DLL */

#endif /* _GLIBMM_CONFIG_H */
