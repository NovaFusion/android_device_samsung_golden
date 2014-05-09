/*
 * Copyright (C) ST-Ericsson SA 2011. All rights reserved.
 * This code is ST-Ericsson proprietary and confidential.
 * Any use of the code for whatever purpose is subject to
 * specific written permission of ST-Ericsson SA.
 */

#ifndef _EXTRADATA_WRAPPER_H_
#define _EXTRADATA_WRAPPER_H_
#include <OMX_Core.h>
#include <IFM_Types.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

typedef OMX_ERRORTYPE (*SetExifParamsFunc_t)(OMX_BUFFERHEADERTYPE* pOmxBufHdr,
                                             IFM_EXIF_SETTINGS* pExtradataData,
                                             OMX_VERSIONTYPE version);

typedef struct
{
  SetExifParamsFunc_t SetExifParams;
} Extradata_WrapperInterface_t;

Extradata_WrapperInterface_t* GetExtradataWrapper(void);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _EXTRADATA_WRAPPER_H_
