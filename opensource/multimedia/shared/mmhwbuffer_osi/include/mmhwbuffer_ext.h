/*
 * Copyright (C) ST-Ericsson SA 2010. All rights reserved.
 * This code is ST-Ericsson proprietary and confidential.
 * Any use of the code for whatever purpose is subject to
 * specific written permission of ST-Ericsson SA.
 */

#ifndef _MMHWBUFFER_EXT_H_
#define _MMHWBUFFER_EXT_H_

#include <OMX_Types.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef enum OMX_MMHWBUFFER_INDEXTYPE {
    OMX_OsiIndexConfigSharedChunkMetadata                = 0x7FD19E5C, /**< reference: OMX_OSI_CONFIG_SHARED_CHUNK_METADATA */
    OMX_google_android_index_enableAndroidNativeBuffers  = 0x7FFF0000, /**< reference: EnableAndroidNativeBuffersParams */
    OMX_google_android_index_getAndroidNativeBufferUsage = 0x7FFF0001, /**< reference: GetAndroidNativeBufferUsageParams */
    OMX_google_android_index_useAndroidNativeBuffer      = 0x7FFF0002, /**< reference: UseAndroidNativeBufferParams */
    OMX_google_android_index_storeMetaDataInBuffers      = 0x7FFF0003, /**< reference: StoreMetaDataInBuffersParams */
} OMX_MMHWBUFFER_INDEXTYPE;

/**
 * The string that the OpenMAX IL component will translate into a 32-bit
 * OpenMAX IL index to support the communication of OSI shared chunk
 * metadata with a tunnelled component
 */
#define sOmxOsiSharedChunkMetadata "OMX.OSI.INDEX.CONFIG.SHAREDCHUNKMETADATA"

typedef struct OMX_OSI_CONFIG_SHARED_CHUNK_METADATA {
    OMX_U32 nSize;            /** Size of this structure, in Bytes */
    OMX_VERSIONTYPE nVersion; /** OMX specification version information */
    OMX_U32 nPortIndex;       /** Port that this structure applies to */
    OMX_U32 nHandleId;        /** Handle ID of the HW buffer pool */
    OMX_U64 nOwnerPID;        /** Process ID of the HW buffer pool  */
    OMX_U32 nFd;              /** The file descriptor associated to the HW buffer pool */
    OMX_U32 nBaseLogicalAddr; /** The logical baseaddress associated to the HW buffer pool */
    OMX_U32 nHandleName;      /** Exported name of buffer handle */
    OMX_U32 nBufferCount;     /** Number of Buffers in the HW buffer pool */
    OMX_U32 nBufferSize;      /** Size of Buffers in the HW buffer pool */
    OMX_U32 nCacheAttr;       /** Cache Attribute of the HW buffer pool */
    OMX_U32 nChunkSize;       /** Total Size of Chunk */
    OMX_U32 nAlignment;       /** Alignment Attribute of the HW buffer pool */
} OMX_OSI_CONFIG_SHARED_CHUNK_METADATA;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _MMHWBUFFER_EXT_H_ */
