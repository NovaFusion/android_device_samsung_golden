/*
 * Copyright (C) ST-Ericsson SA 2010. All rights reserved.
 * This code is ST-Ericsson proprietary and confidential.
 * Any use of the code for whatever purpose is subject to
 * specific written permission of ST-Ericsson SA.
 */

#ifndef _AUDIO_CODEC_TYPE_H_
#define _AUDIO_CODEC_TYPE_H_

#include "OMX_Audio.h"
typedef enum NOKIACSA_AUDIO_CODINGTYPE {
    CSA_AUDIO_CodingVendorStartUnused = OMX_AUDIO_CodingVendorStartUnused,
    OMX_AUDIO_CodingFLAC      ,  /**< Any variant of FLAC encoded data */
    OMX_AUDIO_CodingEAC3      ,  /**< Any variant of EAC3 encoded data */
    OMX_AUDIO_CodingAC3       ,  /**< Any variant of AC3 encoded data */
    OMX_AUDIO_CodingDTS       ,  /**< Any variant of DTS  encoded data */
    OMX_AUDIO_CodingWMAPRO    ,  /**< Any variant of WMAPRO encoded data */
    CSA_AUDIO_CodingMax = 0x7FFFFFFF
}NOKIACSA_AUDIO_CODINGTYPE;
  
#endif
