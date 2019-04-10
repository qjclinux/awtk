/* Define to 1 if you have the <stdint.h> header file. */
#define HAVE_STDINT_H 1

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H 1

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 1

/* Define to the sub-directory where libtool stores uninstalled libraries. */
#define LT_OBJDIR ".libs/"

/* Name of package */
#define PACKAGE "pixman"

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT "pixman@lists.freedesktop.org"

/* Define to the full name of this package. */
#define PACKAGE_NAME "pixman"

/* Define to the full name and version of this package. */
#define PACKAGE_STRING "pixman 0.38.0"

/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME "pixman"

/* Define to the home page for this package. */
#define PACKAGE_URL ""

/* Define to the version of this package. */
#define PACKAGE_VERSION "0.38.0"

/* Version number of package */
#define VERSION "0.38.0"

/* Define to 1 if you have the ANSI C header files. */
#define STDC_HEADERS 1

/* use GNU-style inline assembler */
#define USE_GCC_INLINE_ASM 1

/* use ARM IWMMXT compiler intrinsics */
/* #undef USE_ARM_IWMMXT */

/* use ARM NEON assembly optimizations */
/* #undef USE_ARM_NEON */

/* use MIPS DSPr2 assembly optimizations */
/* #undef USE_MIPS_DSPR2 */

#if defined(ARM_MATH_CM7) || defined(ARM_MATH_CM4)
/* use ARM SIMD assembly optimizations */
//# define USE_ARM_SIMD 1
/* The size of `long', as computed by sizeof. */
#define SIZEOF_LONG 4
#else /*x86*/
/* use SSE2 compiler intrinsics */
#define USE_SSE2 1
/* use SSSE3 compiler intrinsics */
#define USE_SSSE3 1
/* use VMX compiler intrinsics */
/* #undef USE_VMX */
/* use x86 MMX compiler intrinsics */
#define USE_X86_MMX 1
/* The size of `long', as computed by sizeof. */
#define SIZEOF_LONG 8

#endif/**/
