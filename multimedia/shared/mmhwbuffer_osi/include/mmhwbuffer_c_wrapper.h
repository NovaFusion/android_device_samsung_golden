/*
 * Copyright (C) ST-Ericsson SA 2010. All rights reserved.
 * This code is ST-Ericsson proprietary and confidential.
 * Any use of the code for whatever purpose is subject to
 * specific written permission of ST-Ericsson SA.
 */

#ifndef _MMHWBUFFER_C_WRAPPER_H_
#define _MMHWBUFFER_C_WRAPPER_H_

#include "mmhwbuffer.h"
#include "MMNativeBuffer.h"

#ifdef __cplusplus
extern "C" {
#endif
OMX_ERRORTYPE MMHwBuffer_Create_CWrapper(const MMHwBuffer::TBufferPoolCreationAttributes &
                                         aBuffPoolAttrs,
                                         OMX_HANDLETYPE aComponent,
                                         MMHwBuffer * &apMMHwBufferHandle);

OMX_ERRORTYPE MMHwBuffer_Destroy_CWrapper(MMHwBuffer * &apMMHwBufferHandle);

typedef OMX_ERRORTYPE (*PFMMHwBufferCreate)(const MMHwBuffer::TBufferPoolCreationAttributes &
                                            aBuffPoolAttrs,
                                            OMX_HANDLETYPE aComponent,
                                            MMHwBuffer * &apMMHwBufferHandle);

typedef OMX_ERRORTYPE (*PFMMHwBufferDestroy)(MMHwBuffer * &apMMHwBufferHandle);

MMNativeBuffer * create_mmnative_buffer(void *pBuffer);
void destroy_mmnative_buffer(MMNativeBuffer *pNativeBuffer);
int get_mm_file_handle(void *pBuffer, int *size);

typedef MMNativeBuffer * (*PFMMCreateNativeBuffer)(void *pBuffer);
typedef void (*PFMMDestroyNativeBuffer)(MMNativeBuffer *pNativeBuffer);
typedef int (*PFMMGetFileHandle)(void *pBuffer, int *size);

#ifdef __cplusplus
}
#endif

#endif /* _MMHWBUFFER_C_WRAPPER_H_ */

