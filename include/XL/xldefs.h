/*

   xldefs.h

   Copyright (c) 2006 Daniel Markus
   All rights reserved.

________________________________________________________________________________
*/

#ifndef XLDEFS_H_
#define XLDEFS_H_

#ifdef __cplusplus
extern "C" {
#endif

enum XLStatusReturnEnum
{
  XL_STATUS_NO_ERROR                    = 0,
  XL_STATUS_NO_OPERATION                = 1,
  XL_STATUS_OUT_OF_MEMORY               = 12,
  XL_STATUS_INVALID_ID                  = 3,
  XL_STATUS_INVALID_ARGUMENT            = 4,
  XL_STATUS_INVALID_VALUE               = 5,
  XL_STATUS_INVALID_PARAMETER           = 6,

  XL_STATUS_RECEIVE_QUEUE_EMPTY         = 7,
  XL_STATUS_SEND_QUEUE_OVERFLOW         = 8,
  XL_STATUS_RECEIVE_QUEUE_OVERFLOW      = 9,
  XL_STATUS_INSUFFICIENT_RESOURCES      = 10,
  XL_STATUS_DEVICE_UNAVAILABLE          = 11,
  XL_STATUS_ACCESS_DENIED               = 22,
  XL_STATUS_DEVICE_ERROR                = 13,
  XL_STATUS_DEVICE_BUSY                 = 14,
  XL_STATUS_INVALID_CONFIGURATION       = 15,
  XL_STATUS_GENLOCK_NO_SIGNAL           = 16,
  XL_STATUS_GENLOCK_UNKNOWN_SIGNAL      = 20,
  XL_STATUS_GENLOCK_ILLEGAL_COMBINATION = 18,
  XL_STATUS_GENLOCK_TIMING_MISMATCH     = 19,

  XL_STATUS_EXISTS                      = 17,
  XL_STATUS_NO_ENTRY                    = 2,
  XL_STATUS_INCOMPATIBLE                = 30,
  XL_STATUS_INTERNAL_ERROR              = 1000
};

enum XLversionState
{
   XL_VER_STATE_ALFA                    = 1,
   XL_VER_STATE_BETA                    = 2,
   XL_VER_STATE_RELEASE                 = 3
};


#ifdef __cplusplus
}
#endif

#endif /* XLDEFS_H_ */
