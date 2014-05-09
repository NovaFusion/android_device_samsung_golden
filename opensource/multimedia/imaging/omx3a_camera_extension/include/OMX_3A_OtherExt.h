/*
 * Copyright (C) ST-Ericsson SA 2011. All rights reserved.
 * This code is ST-Ericsson proprietary and confidential.
 * Any use of the code for whatever purpose is subject to
 * specific written permission of ST-Ericsson SA.
 */

#ifndef _OMX_3A_OTHEREXT_H_
#define _OMX_3A_OTHEREXT_H_
#include "OMX_Other.h"

#define OMX_3A_OTHER_GLACE_GRID_MAX_WIDTH           72
#define OMX_3A_OTHER_GLACE_GRID_MAX_HEIGHT          54
#define OMX_3A_OTHER_STATSAEWB_DATASIZE               3888		//OMX_3A_OTHER_GLACE_GRID_MAX_WIDTH * OMX_3A_OTHER_GLACE_GRID_MAX_HEIGHT
#define OMX_3A_OTHER_STATSAF_WINDOW_NUMBER_MAX      10

typedef struct OMX_3A_OTHER_STATSAF_HOSTZONESTATUS
{
  /// focus Measure for each zone when valid for AF .
  OMX_U32 focus;
  /// current Host Programmed Start X with in WOI X size . !! 16b
  OMX_U32 afZoneStartX;
  /// current Host Programmed Start Y with in WOI Y size . !! 16b
  OMX_U32 afZoneStartY;
  /// current Host Programmed End X with in WOI X size . !! 16b
  OMX_U32 afZoneEndX;
  /// current Host Programmed End Y with in WOI Y size . !! 16b
  OMX_U32 afZoneEndY;
  /// Width of the zone !! 16b
  OMX_U32 afZonesWidth;
  /// Height of the zone !! 16b
  OMX_U32 afZonesHeight;
  /// Light Measure for each zone when valid for AF . !! 8b
  OMX_U32 light;
  /// weight assigned to zone . !! 8b
  OMX_U32 weightAssigned;
  /// enable disable host zone
  OMX_BOOL boEnabled;
} OMX_3A_OTHER_STATSAF_HOSTZONESTATUS;

typedef struct OMX_3A_OTHER_STATSAEWBDATATYPE {
  OMX_U32 nSize;
  OMX_VERSIONTYPE nVersion;
  OMX_U8 statsR[OMX_3A_OTHER_STATSAEWB_DATASIZE];
  OMX_U8 statsG[OMX_3A_OTHER_STATSAEWB_DATASIZE];
  OMX_U8 statsB[OMX_3A_OTHER_STATSAEWB_DATASIZE];
  OMX_U8 statsS[OMX_3A_OTHER_STATSAEWB_DATASIZE];
} OMX_3A_OTHER_STATSAEWBDATATYPE;

typedef struct OMX_3A_OTHER_STATSAFDATATYPE {
  OMX_U32 nSize;
  OMX_VERSIONTYPE nVersion;
  OMX_3A_OTHER_STATSAF_HOSTZONESTATUS afStats[OMX_3A_OTHER_STATSAF_WINDOW_NUMBER_MAX];
  OMX_U32 afStatsLensPos;
} OMX_3A_OTHER_STATSAFDATATYPE;

typedef enum OMX_3A_OTHER_FORMATTYPE {
  OMX_3A_OTHER_FormatStatsAewb = OMX_OTHER_FormatVendorReserved,
  OMX_3A_OTHER_FormatStatsAf,
} OMX_3A_OTHER_FORMATTYPE;

// TODO:TENTATIVE
// Define all the structures of sw3A side as provisional processing.
// Is new DATATYPE making only for the element that will be necessary later necessary?


typedef enum {
  OMX_3A_OTHER_TYPEERROR = 0,
  OMX_3A_OTHER_AEWB,
  OMX_3A_OTHER_AF
} OMX_3A_OTHER_PROCTYPE;

#endif
