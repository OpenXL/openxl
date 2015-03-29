/*

   dataxl.h

   Copyright (c) 2006 Daniel Markus
   All rights reserved.

________________________________________________________________________________
*/

#ifndef XL_DATA_H_
#define XL_DATA_H_


enum XLDataSettingsEnum
{
   XL_DATA_USER_ID_INT32                     = 0,
   XL_DATA_SOURCE_PBYTE                      = 1,
   XL_DATA_ACCESS_SPATIAL_LOCALITY_INT32     = 2,
   XL_DATA_ACCESS_TEMPORAL_LOCALITY_INT32    = 3,
   XL_DATA_ACCESS_GRANULARITY_MIN_SIZE_INT64 = 4
};

enum XLDataSpatialLocalityEnum
{
   XL_SPATIAL_LOCALITY_ROW    = 0,
   XL_SPATIAL_LOCALITY_COLUMN = 1,
   XL_SPATIAL_LOCALITY_AREA   = 2,
   XL_SPATIAL_LOCALITY_BLOCK  = 3
};

enum XLDataSpatialLocalityAreaEnum
{
   XL_SPATIAL_LOCALITY_AREA_SIZE_X_INT64 = 0,
   XL_SPATIAL_LOCALITY_AREA_SIZE_Y_INT64 = 1
};

enum XLDataSpatialLocalityBlockEnum
{
   XL_SPATIAL_LOCALITY_BLOCK_SIZE_X_INT64 = 0,
   XL_SPATIAL_LOCALITY_BLOCK_SIZE_Y_INT64 = 1,
   XL_SPATIAL_LOCALITY_BLOCK_SIZE_Z_INT64 = 2,
};

enum XLDataTemporalLocalityEnum
{
   XL_TEMPORAL_LOCALITY_ONCE   = 0,
   XL_TEMPORAL_LOCALITY_ALWAYS = 1
};

#endif /* XL_DATA_H_ */

