/*
 * Copyright (C) ST-Ericsson SA 2010. All rights reserved.
 * This code is ST-Ericsson proprietary and confidential.
 * Any use of the code for whatever purpose is subject to
 * specific written permission of ST-Ericsson SA.
 */

#ifndef _VIDEO_CSA_INDEX_H_
#define _VIDEO_CSA_INDEX_H_

#include "CSA_Index.h"


typedef enum CSA_VIDEOINDEXTYPE{

  CSA_IndexVideoStartExtensions =    CSA_IndexVideoStartUnused,

  // more index to be added here

  /// @brief for GPS Info
  /// modified the structure being referenced with this index for ER348736
  /**( reference: OMX_SYMBIAN_CONFIG_GPSLOCATIONTYPE */
  OMX_IndexConfigImageGPSLocation, /**< reference: OMX_SYMBIAN_CONFIG_GPSLOCATIONTYPE */

  /// @brief for Color primary information
  /**( reference: OMX_SYMBIAN_PARAM_COLORPRIMARYTYPE */
  OMX_IndexParamColorPrimary,   /**< reference: OMX_SYMBIAN_PARAM_COLORPRIMARYTYPE */

  /// @brief  for returning headers (MPEG4 VOS+VOL)
  /**( reference: OMX_SYMBIAN_VIDEO_PARAM_SEQUENCEHEADERTYPE */
  OMX_IndexParamVideoSequenceHeader, /**< reference: OMX_SYMBIAN_VIDEO_PARAM_SEQUENCEHEADERTYPE */

  /// @brief Allow to get / set the pixel aspect ratio
  /**( reference: OMX_PARAM_PIXELASPECTRATIO */
  OMX_IndexParamPixelAspectRatio,  /**< reference: OMX_PARAM_PIXELASPECTRATIO */
} CSA_VIDEOINDEXTYPE;

#endif // _VIDEO_CSA_INDEX_H_

