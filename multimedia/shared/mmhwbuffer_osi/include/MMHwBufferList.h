/*
 * Copyright (C) ST-Ericsson SA 2011. All rights reserved.
 * This code is ST-Ericsson proprietary and confidential.
 * Any use of the code for whatever purpose is subject to
 * specific written permission of ST-Ericsson SA.
 */

#ifndef _MMHWBUFFERLIST_H_
#define _MMHWBUFFERLIST_H_

#include "BSD_list.h"
#include "mmhwbuffer.h"
#include "mmhwbuffer_ext.h"
#include "MMHwBufferPool.h"

#ifdef ANDROID
  #include "media/hardware/HardwareAPI.h"
  using namespace android;
#endif //ANDROID

class MMHwBufferList {

private:
    /**
     * The list of MMHwBufferPool
     * Object are created and added to the end of the list as they are registered,
     * as such the buffer index will be the same as the order in the list
     */
    MMHwBufferPool_list_head mList;

public:

    MMHwBufferList();
    ~MMHwBufferList();

    OMX_ERRORTYPE Init();

    /* Return the last member of the list */
    MMHwBufferPool * getLastBufferPool();

    OMX_ERRORTYPE BufferInfoInd(OMX_U32 aBufferIndex, MMHwBuffer::TBufferInfo &aInfo);

    OMX_ERRORTYPE BufferInfoLog(OMX_U32 aLogAddr, OMX_U32 aSize, MMHwBuffer::TBufferInfo &aInfo);

    OMX_ERRORTYPE CacheSync(MMHwBuffer::TSyncCacheOperation aOp,
                            OMX_U32 aLogAddr,
                            OMX_U32 aSize,
                            OMX_U32 &aPhysAddr);

    OMX_U32 getTotalSize();

    OMX_U32 dump();

#ifdef ANDROID
    OMX_ERRORTYPE addAndroidNativeBuffer(UseAndroidNativeBufferParams *params);
#endif //ANDROID
};

#endif  /* #ifndef _MMHWBUFFERLIST_H_ */
