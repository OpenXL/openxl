/*

   xlbasictypes.h

   Copyright (c) 2006 Daniel Markus
   All rights reserved.

________________________________________________________________________________
 */

#ifndef XL_BASIC_TYPES_H_
#define XL_BASIC_TYPES_H_

#ifdef __cplusplus
extern "C" {
#endif


#if defined(WIN32) || defined(WIN64)
#define _WIN32_WINNT    0x0400
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

#include <stddef.h>
#include <sys/types.h>
#ifndef _KERNEL
#include <float.h>
#endif

#if defined(_WIN64)
#include <basetsd.h>
#endif

#if defined(WIN32)
#define XL_OS_NT        1
#define XL_OS_NT32      1
#define XL_ARCH_IA32    1
#endif

#if  defined(WIN64)
#define XL_OS_NT        1
#define XL_OS_NT64      1
#define XL_ARCH_IA64    1
#endif

#if defined(__sgi)
#define XL_OS_IRIX      1
#define XL_OS_UNIX      1
#define XL_ARCH_MIPS    1
#endif

#if defined(linux)
#define XL_OS_LINUX     1
#define XL_OS_UNIX      1

#if defined(__ia64)
#define XL_ARCH_IA64 1
#endif

#if defined(__i386)
#define XL_ARCH_IA32    1
#endif
#endif

typedef signed char     XLbyte;
typedef unsigned char   XLubyte;

typedef signed char     XLint8;
typedef short           XLint16;
typedef signed int      XLint32;

typedef unsigned char   XLuint8;
typedef unsigned short  XLuint16;
typedef unsigned int    XLuint32;

#if defined(XL_ARCH_IA32)
#if defined(_WIN32)
typedef __int64            XLuint64;
#elif(_WIN64)
typedef ULONG64            XLuint64;
#else /* Linux */
typedef signed long long   XLuint64;
#endif /* _WIN32 */

#elif defined(XL_ARCH_MIPS)
typedef uint64_t           XLuint64;

#else
typedef unsigned long long XLuint64;
#endif

#if !defined(_WIN64)
typedef size_t             XLsize_t;
#else
typedef SIZE_T             XLsize_t;
#endif

typedef XLint32 XLboolean;

#define XL_FALSE   0
#define XL_TRUE    1

#if defined(XL_ARCH_MIPS)
#if (_MIPS_SZLONG == 64)
#define XLintptr     XLint64
#define _XL_SZLONG   64
#else
#define XLintptr     XLint32
#define _XL_SZLONG   32
#endif

#else
#if defined(XL_ARCH_IA32)
#define XLintptr     XLint32
#define _XL_SZLONG   32
#endif

#if defined(XL_ARCH_IA64)
#define XLintptr     XLint64
#define _XL_SZLONG   64
#endif

#endif

#ifdef XL_ARCH_MIPS
typedef int64_t            XLint64;
#else
#ifdef XL_OS_LINUX
typedef signed long long   XLint64;
#else
#ifdef XL_OS_NT32
typedef __int64            XLint64;
#else

typedef signed long long   XLint64;
#endif
#endif
#endif

typedef float              XLreal32;
typedef double             XLreal64;

typedef XLint64            XLopenid;

#ifdef  XL_OS_NT
typedef HANDLE XLwaitable;
#else
typedef int XLwaitable;
#endif

#ifdef  XL_OS_NT
#define XLAPI __stdcall
#else
#define XLAPI
#endif

#ifdef XL_OS_UNIX
#define FORMAT_LLX "llx"
#define FORMAT_LLD "lld"
#endif
#ifdef XL_OS_NT
#if defined(__MWERKS__) || defined(COMPILER_GCC)
#define FORMAT_LLX "llx"
#define FORMAT_LLD "lld"
#else
#define FORMAT_LLX "I64x"
#define FORMAT_LLD "I64d"
#endif
#endif


#ifdef __cplusplus
}
#endif

#endif /* XL_BASIC_TYPES_H_ */
