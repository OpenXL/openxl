/*

   xltypes.h

   Copyright (c) 2006 Daniel Markus
   All rights reserved.

________________________________________________________________________________
*/

#ifndef XLTYPES_H_
#define XLTYPES_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <XL/XLbasictypes.h>


typedef XLint32   XLstatus;
typedef XLint32   XLstdid;
typedef XLint32   XLlibid;
typedef XLint32   XLmapid;
typedef XLstatus  (*XLfncptr)(struct XLpv_t*, struct XLpv_t*);

typedef struct XLversion_t
{
   XLint32     release;    /* Release normally represent a big change. */
   XLint32     revision;   /* Revision normally represent extensions but not removals. */
   XLint32     patch;      /* A patch is normally not seen in the interface. */
   XLint32     state;      /* XL_ALFA, XL_BETA, XL_RELEASE */
   XLboolean   backComp;   /* Backwards compability. */

} XLversion;

typedef union XLvalue_u
{
   XLbyte            byte;
   XLint32           int32;
   XLint64           int64;
   XLreal32          real32;
   XLreal64          real64;
   XLstdid           stdid;
   XLfncptr          fncptr;
   XLbyte*           pByte;
   XLint32*          pInt32;
   XLint64*          pInt64;
   XLreal32*         pReal32;
   XLreal64*         pReal64;
   XLstdid*          pStdid;
   XLfncptr*         pFncptr;
   struct XLpv_t*    pPv;
   struct XLpv_t**   ppPv;

} XLvalue;

typedef struct XLpv_t
{
   XLint64           param;
   XLvalue           value;
   XLint32           maxSize;
   XLint32           size;

} XLpv;

enum XLpvMaskEnum
{
   XL_PARAM_MASK_TYPE      = 0x000000FF, /* typeof param value */

   XL_PARAM_MASK_TYPE_ELEM = 0x0000000F, /* byte, int... */
   XL_PARAM_MASK_TYPE_VAL  = 0x000000F0  /* scalar, array, pointer... */
};

enum XLpvTypeElemEnum
{
   XL_TYPE_ELEM_BYTE   = 0x01,
   XL_TYPE_ELEM_INT32  = 0x02,
   XL_TYPE_ELEM_INT64  = 0x03,
   XL_TYPE_ELEM_REAL32 = 0x05,
   XL_TYPE_ELEM_REAL64 = 0x06,
   XL_TYPE_ELEM_STDID  = 0x07,
   XL_TYPE_ELEM_FNCPTR = 0x08,
   XL_TYPE_ELEM_PV     = 0x09,
   XL_TYPE_ELEM_MSG    = 0x0a /* a message is a list of MLpv's */
};

enum XLpvTypeValEnum
{
   XL_TYPE_VAL_SCALAR  = 0x10,
   XL_TYPE_VAL_POINTER = 0x20,
   XL_TYPE_VAL_ARRAY   = 0x40
};

#define XL_END 0

enum XLpvTypeEnum
{
   XL_TYPE_BYTE           = XL_TYPE_ELEM_BYTE   | XL_TYPE_VAL_SCALAR,
   XL_TYPE_INT32          = XL_TYPE_ELEM_INT32  | XL_TYPE_VAL_SCALAR,
   XL_TYPE_INT64          = XL_TYPE_ELEM_INT64  | XL_TYPE_VAL_SCALAR,
   XL_TYPE_REAL32         = XL_TYPE_ELEM_REAL32 | XL_TYPE_VAL_SCALAR,
   XL_TYPE_REAL64         = XL_TYPE_ELEM_REAL64 | XL_TYPE_VAL_SCALAR,
   XL_TYPE_STDID          = XL_TYPE_ELEM_STDID  | XL_TYPE_VAL_SCALAR,
   XL_TYPE_FNCPTR         = XL_TYPE_ELEM_FNCPTR | XL_TYPE_VAL_SCALAR,

   XL_TYPE_BYTE_POINTER   = XL_TYPE_ELEM_BYTE   | XL_TYPE_VAL_POINTER,
   XL_TYPE_INT32_POINTER  = XL_TYPE_ELEM_INT32  | XL_TYPE_VAL_POINTER,
   XL_TYPE_INT64_POINTER  = XL_TYPE_ELEM_INT64  | XL_TYPE_VAL_POINTER,
   XL_TYPE_REAL32_POINTER = XL_TYPE_ELEM_REAL32 | XL_TYPE_VAL_POINTER,
   XL_TYPE_REAL64_POINTER = XL_TYPE_ELEM_REAL64 | XL_TYPE_VAL_POINTER,
   XL_TYPE_PV_POINTER     = XL_TYPE_ELEM_PV     | XL_TYPE_VAL_POINTER,

   XL_TYPE_BYTE_ARRAY     = XL_TYPE_ELEM_BYTE   | XL_TYPE_VAL_ARRAY,
   XL_TYPE_INT32_ARRAY    = XL_TYPE_ELEM_INT32  | XL_TYPE_VAL_ARRAY,
   XL_TYPE_INT64_ARRAY    = XL_TYPE_ELEM_INT64  | XL_TYPE_VAL_ARRAY,
   XL_TYPE_REAL32_ARRAY   = XL_TYPE_ELEM_REAL32 | XL_TYPE_VAL_ARRAY,
   XL_TYPE_REAL64_ARRAY   = XL_TYPE_ELEM_REAL64 | XL_TYPE_VAL_ARRAY,
   XL_TYPE_PV_ARRAY       = XL_TYPE_ELEM_PV     | XL_TYPE_VAL_ARRAY,
   XL_TYPE_MSG_ARRAY      = XL_TYPE_ELEM_MSG    | XL_TYPE_VAL_ARRAY
};

/* Create a new parameter name
 */
#define XL_PARAM_NAME( type, index ) ( (type)|((index)<<8) )


#ifdef __cplusplus
}
#endif

#endif /* XLTYPES_H_ */

