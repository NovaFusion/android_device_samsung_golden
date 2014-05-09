/*
 * Copyright (C) ST-Ericsson SA 2011. All rights reserved.
 * This code is ST-Ericsson proprietary and confidential.
 * Any use of the code for whatever purpose is subject to
 * specific written permission of ST-Ericsson SA.
 */

#ifndef _MMHWBUFFERPOOL_H_
#define _MMHWBUFFERPOOL_H_

#include <pthread.h>

#include "BSD_list.h"
#include "mmhwbuffer.h"
#include "mmhwbuffer_ext.h"
#include "MMNativeBuffer.h"

class MMHwBufferPool;

struct MMHwBufferPool_list_elem {
    CIRCLEQ_ENTRY(MMHwBufferPool_list_elem) list_entry;
    MMHwBufferPool *mPool;
};

CIRCLEQ_HEAD(MMHwBufferPool_list_head, MMHwBufferPool_list_elem);

class MMHwBufferPool {

private:
    /** The file descriptor to access the hwmem object used by this BufferPool */
    OMX_S32 hwmemfd;
    /** The identifier to export across processes */
    OMX_S32 mHandleName;
    /** The attributes as requested by buffer client */
    MMHwBuffer::TBufferPoolCreationAttributes mBuffPoolAttrs;
    /** True when map has been performed  */
    bool mapDone;
    /** Actual buffer size to meat size and alignment constraints */
    OMX_U32 mBufferSize;
    /** Total allocation size for this buffer pool */
    OMX_U32 mTotalSize;
    /** Base offset to fulfill alignment constraints */
    OMX_U32 mBaseOffset;
    /** Logical address of the start of the buffer pool */
    OMX_U32 mLogicalAddress;
    /** Physical address of the start of the buffer pool */
    OMX_U32 mPhysicalAddress;

    MMNativeBuffer *mNativeBuffer;

    inline OMX_U32 getIndexLog(OMX_U32 aLogAddr)
    {
        return (aLogAddr - (mLogicalAddress + mBaseOffset)) / mBufferSize;
    }

    inline OMX_U32 getIndexPhy(OMX_U32 aPhyAddr)
    {
        return (aPhyAddr - (mPhysicalAddress + mBaseOffset)) / mBufferSize;
    }

    /** Global mutex to protect the allocation counter */
    static pthread_mutex_t globalMutex;
    static inline void Lock() { pthread_mutex_lock(&MMHwBufferPool::globalMutex); }
    static inline void Unlock() { pthread_mutex_unlock(&MMHwBufferPool::globalMutex); }

    /** The amount of memory allocated by video decoders */
    static OMX_U32 mTotalVideoDecoder;

    /** The amount of memory allocated from the global mTotalVideoDecoder budget */
    OMX_U32 mVideoDecoderSize;

    /* Return true in case there is no memory limitation for this component */
    bool isAllowedToAllocate(OMX_HANDLETYPE aComponent);

public:

    /** Member to be able to manage a list of this class */
    MMHwBufferPool_list_elem list_elem;

    MMHwBufferPool();
    MMHwBufferPool(void *pNativeBuffer);
    ~MMHwBufferPool();

    /** Open a non allocated buffer pool handle */
    OMX_ERRORTYPE Open();

    /** Open an already allocated buffer pool handle  */
    OMX_ERRORTYPE Open(OMX_OSI_CONFIG_SHARED_CHUNK_METADATA *sharedConf);

    /** Allocate the memory as per requirements */
    OMX_ERRORTYPE allocate(const MMHwBuffer::TBufferPoolCreationAttributes &aBuffPoolAttrs,
                           OMX_HANDLETYPE aComponent);

    /** close the memory pool - this will free the allocate memory in case allocate was done */
    OMX_ERRORTYPE Close();

    /** Map the memory to be used in this process */
    OMX_ERRORTYPE map();

    /** Unmap the memory to be used in this process */
    OMX_ERRORTYPE unmap();

    /** prepare as per requirements : open, allocate and map*/
    OMX_ERRORTYPE prepare(const MMHwBuffer::TBufferPoolCreationAttributes &aBuffPoolAttrs,
                          OMX_HANDLETYPE aComponent);

    OMX_ERRORTYPE CacheSync(MMHwBuffer::TSyncCacheOperation aOp,
                            OMX_U32 aLogAddr,
                            OMX_U32 aSize,
                            OMX_U32 &aPhysAddr);

    void SetConfigExtension(OMX_OSI_CONFIG_SHARED_CHUNK_METADATA *conf);

    OMX_ERRORTYPE BufferInfoInd(OMX_U32 aBufferIndex, MMHwBuffer::TBufferInfo &aInfo);

    OMX_U32 getTotalSize();

    /** Dump to logcat the information related to this buffer pool
     * Return the total allocated size */
    OMX_U32 dump();

    static OMX_ERRORTYPE CacheSync(OMX_S32 hwmemfd,
                                   MMHwBuffer::TSyncCacheOperation aOp,
                                   OMX_U32 offset,
                                   OMX_U32 size);

    inline OMX_ERRORTYPE BufferInfoLog(OMX_U32 aLogAddr,
                                       OMX_U32 aSize,
                                       MMHwBuffer::TBufferInfo &aInfo)
    {
        return BufferInfoInd(getIndexLog(aLogAddr), aInfo);
    }

    OMX_ERRORTYPE BufferInfoPhy(OMX_U32 aPhyAddr,
                                OMX_U32 aSize,
                                MMHwBuffer::TBufferInfo &aInfo)
    {
        return BufferInfoInd(getIndexPhy(aPhyAddr), aInfo);
    }
};

#endif  /* #ifndef _MMHWBUFFERPOOL_H_ */
