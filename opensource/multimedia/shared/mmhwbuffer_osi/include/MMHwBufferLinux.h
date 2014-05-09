/*
 * Copyright (C) ST-Ericsson SA 2011. All rights reserved.
 * This code is ST-Ericsson proprietary and confidential.
 * Any use of the code for whatever purpose is subject to
 * specific written permission of ST-Ericsson SA.
 */

#ifndef _MMHWBUFFEROSI_H_
#define _MMHWBUFFEROSI_H_

#include <pthread.h>

#include "BSD_list.h"
#include "mmhwbuffer.h"
#include "omxilosalservices.h"
#include "MMHwBufferPool.h"
#include "MMHwBufferList.h"

class MMHwBufferLinux;

typedef struct MMHwBufferLinux_list_elem {
    LIST_ENTRY(MMHwBufferLinux_list_elem) list_entry;
    MMHwBufferLinux *mHwBuffer;
} MMHwBufferLinux_list_elem_t;

typedef LIST_HEAD (MMHwBufferLinux_list_head,
                   MMHwBufferLinux_list_elem) MMHwBufferLinux_list_head_t;

/**
 * The Linux/Android specific implementation used by the OS independent MM framework
 */
class MMHwBufferLinux : public MMHwBuffer {

public:

    /*
     * MMHwBuffer API members
     */

    virtual OMX_ERRORTYPE AddBufferInfo(OMX_U32 aBufferIndex,
                                        OMX_U32 aLogAddr,
                                        OMX_U32 aSize);

    virtual OMX_ERRORTYPE SetConfigExtension(OMX_HANDLETYPE aTunneledComponent,
                                             OMX_U32 aPortIndex);

    virtual OMX_ERRORTYPE GetConfigExtension(OMX_INDEXTYPE aIndex, OMX_PTR apConf,
                                             OMX_U32 aPortIndex) const;

    virtual OMX_ERRORTYPE BufferInfo(OMX_U32 aBufferIndex,
                                     TBufferInfo &aInfo);

    virtual OMX_ERRORTYPE CacheSync(TSyncCacheOperation aOp,
                                    OMX_U32 aLogAddr,
                                    OMX_U32 aSize,
                                    OMX_U32 &aPhysAddr);

    virtual OMX_ERRORTYPE CacheSync(TSyncCacheOperation aOp,
                                    OMX_U32 aBufferIndex,
                                    OMX_U32 &aPhysAddr);

    virtual OMX_ERRORTYPE PhysAddress(OMX_U32 aLogAddr,
                                      OMX_U32 aSize,
                                      OMX_U32 &aPhyAddr);

    virtual OMX_ERRORTYPE LogAddress(OMX_U32 aPhyAddr,
                                     OMX_U32 aSize,
                                     OMX_U32 &aLogAddr);

    /*
     * MMHwBufferLinux specific members
     */

    MMHwBufferLinux(MMHwBufferPool *aPool, OMX_HANDLETYPE aComponent);
    MMHwBufferLinux(MMHwBufferList *aList, OMX_HANDLETYPE aComponent, OMX_U32 aPortIndex);
    /* !!! Global lock must be held when destroying object !!! */
    ~MMHwBufferLinux();

    static inline void GlobalLock() { pthread_mutex_lock(&MMHwBufferLinux::globalMutex); }
    static inline void GlobalUnlock() { pthread_mutex_unlock(&MMHwBufferLinux::globalMutex); }

    /* Lock/Unlock is used to ensure atomic increment/decrement */
    inline OMX_U32 increaseNbRef() { Lock(); OMX_U32 nbref = ++mNbRef; Unlock(); return nbref; }
    inline OMX_U32 decreaseNbRef() { Lock(); OMX_U32 nbref = --mNbRef; Unlock(); return nbref; }

    static MMHwBufferLinux * getMMHwBufferLinux(OMX_HANDLETYPE aComponent, OMX_U32 aPortIndex);

    /** Dump all the allocated buffer */
    static void dumpMMHwBufferLinux();

    inline MMHwBufferList * getBufferList() { return mBufferList; }

    static OMX_ERRORTYPE GetAllocatedSize(OMX_HANDLETYPE aComponent, OMX_U32 &aAllocatedSize);

    /* Return true is handle is valid : existing and not yet Destroyed.
       !!! Global lock must be held !!! */
    static bool isValidHandle(MMHwBufferLinux *handle);

private:
    inline void Lock() { pthread_mutex_lock(&mutex); }
    inline void Unlock() { pthread_mutex_unlock(&mutex); }

    /** Global mutex to protect the MMHwBufferLinux list */
    static pthread_mutex_t globalMutex;
    /** The MMHwBufferLinux list */
    static MMHwBufferLinux_list_head_t mMMHwBufferList;

    /** The list structure to register this class to the mMMHwBufferList */
    MMHwBufferLinux_list_elem_t list_elem;

    /** Mutex to protect mNbRef member */
    pthread_mutex_t mutex;
    /** The ref counter */
    OMX_U32 mNbRef;

    /** The MMHwBufferLinux class is wrapping either a MMHwBufferPool or a MMHwBufferList */
    MMHwBufferPool *mBufferPool;
    MMHwBufferList *mBufferList;

    /* The OMX handle associated with the creation of this object */
    OMX_HANDLETYPE mComponent;
    /* The port index the BufferList is associated with */
    OMX_U32 mPortIndex;

    OMX_U32 getTotalSize();
    OMX_U32 dump();
    void addSelfToList();
};

#endif

