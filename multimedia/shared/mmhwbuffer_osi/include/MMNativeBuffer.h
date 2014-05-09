/*
 * Copyright (C) ST-Ericsson SA 2011. All rights reserved.
 * This code is ST-Ericsson proprietary and confidential.
 * Any use of the code for whatever purpose is subject to
 * specific written permission of ST-Ericsson SA.
 */

#ifndef _MM_NATIVE_BUFFER_H_
#define _MM_NATIVE_BUFFER_H_

#include <sys/types.h>

class MMNativeBuffer {
private:
    unsigned long mNativeBuffer;
    int mExportName;
    int mBufferSize;
    int mHwmemFd;
    int mBufferWidth;
    int mBufferHeight;
    int mBufferFormat;
    int mBufferType;
    int mBufferOffset;
    unsigned long mLogicalAddress;
    unsigned long mPhysicalAddress;
    bool mInitialized;

public:
    // pBuffer is actually supposed to be a buffer_handle_t
    MMNativeBuffer(void *pBuffer);
    virtual ~MMNativeBuffer();

    // return the file descriptor associated with the buffer
    // don't call twice on the same buffer but rather cache it
    // so that we dont use multiple fds for the same buffer
    // the function also provides the size of the buffer if
    // anyone is interested to know
    static int getFileHandle(void *pBuffer, int *pBufferSize);

    // calling init() is necessary before calling any other
    // member function. init() interacts with the gralloc
    // API and the underlying hardware driver to obtain the
    // logical and physical address of the buffer
    virtual int init();

    // Open a native window handle
    // In case of success, the file handle is returned in nativefd
    // If a name is provided then import this name and return
    // the buffer in pBufferSize
    // 0 is returned in case of success and -1 in case of error
    static int Open(int *nativefd, int *name, int *pBufferSize);

    // Map the buffer described by the file handle for the given size
    // return the virtual address or MAP_FAILED in case of error
    static void *Map(int nativefd, size_t size);
    static int UnMap(void *addr, size_t length);

    // used to retrieve the physical address associated with
    // nativefd
    static int Pin(int nativefd, unsigned long *physAddr);
    static int UnPin(int nativefd);

    // getter functions
    virtual int getBuffer();
    virtual int getHandle();
    virtual int getExportName();
    virtual unsigned long getLogicalAddress();
    virtual unsigned long getPhysicalAddress();
    virtual int getBufferSize();
    virtual int getBufferWidth();
    virtual int getBufferHeight();
    virtual int getBufferFormat();
    virtual int getBufferOffset();
    virtual int getBufferType();
};

#endif // _MM_NATIVE_BUFFER_H_
